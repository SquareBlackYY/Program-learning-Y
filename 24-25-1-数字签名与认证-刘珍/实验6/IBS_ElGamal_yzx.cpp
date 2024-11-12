#include <iostream>
#include <pbc/pbc.h>
#include <gmpxx.h>
#include <openssl/sha.h>
#include <openssl/evp.h>


struct Signature {
    element_t R, S;                 // 签名包含的两个元素 R 和 S

    Signature(pairing_t& pairing);  // 构造函数初始化 R 和 S
    ~Signature();                   // 析构函数清理 R 和 S

    friend std::ostream& operator<<(std::ostream& os, const Signature& sig);    // 重载输出操作符
};

class System {
public:
    pairing_t pairing;          // 配对参数
    element_t G1, G2, P, P_pub; // 群元素
    element_t q, s;             // 群的阶数 q 和系统私钥 s

    System();                   // 构造函数初始化配对和系统参数
    ~System();                  // 析构函数释放资源
};

class User {
public:
    const char* id;         // 用户 ID
    System* system;         // 指向系统的指针
    element_t Q_ID, d_ID;   // 用户的公私钥

    User(const char* userId, System* sys);                                      // 构造函数生成用户公私钥
    void createSignature(const char* message, Signature& sig);                  // 生成签名
    bool verifySignature(User& prover, const char* message, Signature& sig);    // 验证签名

private:
    void hash_to_element(const unsigned char* input, int length, element_t& output);    // 哈希到群元素
};

void elgamalSignatureID();  // ElGamal签名生成和验证测试


int main() {
    elgamalSignatureID();
    return 0;
}


// ElGamal 签名和验证测试
void elgamalSignatureID() {
    // 初始化系统和用户
    System system;
    User prover("Alice", &system);
    User verifier("Bob", &system);

    // 需要签名的消息
    const char* message = "这只是一个测试";

    // 创建签名并输出签名
    Signature sig(system.pairing);
    prover.createSignature(message, sig);

    std::cout << "签名为: " << sig << std::endl;

    // 验证签名并输出结果
    if (verifier.verifySignature(prover, message, sig)) {
        std::cout << "验签成功" << std::endl;
    } else {
        std::cout << "验签失败" << std::endl;
    }
}

// 构造函数：初始化 R 和 S
Signature::Signature(pairing_t& pairing) {
    element_init_G1(R, pairing);
    element_init_G1(S, pairing);
}

// 析构函数：清理 R 和 S
Signature::~Signature() {
    element_clear(R);
    element_clear(S);
}

// 重载输出操作符：输出签名 R 和 S
std::ostream& operator<<(std::ostream& os, const Signature& sig) {
    os << "Signature R: ";
    element_printf("%B\n", sig.R);
    os << "Signature S: ";
    element_printf("%B\n", sig.S);
    return os;
}

// 构造函数：初始化配对和系统参数
System::System() {
    // 1. 初始化配对参数
    pbc_param_t param;
    pbc_param_init_a_gen(param, 160, 512);
    pairing_init_pbc_param(pairing, param); // 使用生成的配对参数初始化 pairing
    pbc_param_clear(param);                 // 清理参数

    // 2. 初始化群的阶数 q 和生成元 G1、G2
    element_init_Zr(q, pairing);            // 初始化 q 为 Zr 群中的元素
    element_init_G1(G1, pairing);
    element_init_G1(G2, pairing);
    element_random(G1);                     // 生成随机元素 G1
    element_random(G2);                     // 生成随机元素 G2

    // 3. 生成系统私钥 s
    element_init_Zr(s, pairing);            // 初始化 s
    element_random(s);                      // 在 Zr 群中生成随机私钥 s

    // 4. 生成生成元 P 和公钥 P_pub = s * P
    element_init_G1(P, pairing);
    element_random(P);                      // 随机生成生成元 P

    element_init_G1(P_pub, pairing);
    element_pow_zn(P_pub, P, s);            // 计算 P_pub = s * P
}

// 析构函数：释放系统资源
System::~System() {
    element_clear(G1);
    element_clear(G2);
    element_clear(P);
    element_clear(P_pub);
    element_clear(q);
    element_clear(s);
    pairing_clear(pairing);
}

// 构造函数：使用 ID 生成用户的公私钥
User::User(const char* userId, System* sys) : id(userId), system(sys) {
    // 1. 初始化 Q_ID，并将 ID 哈希为群元素 Q_ID
    element_init_G1(Q_ID, system->pairing);
    hash_to_element(reinterpret_cast<const unsigned char*>(id), strlen(id), Q_ID);

    // 2. 计算私钥 d_ID = s * Q_ID
    element_init_G1(d_ID, system->pairing);
    element_pow_zn(d_ID, Q_ID, system->s);  // d_ID = s * Q_ID
}

// 生成签名函数：基于消息生成签名
void User::createSignature(const char* message, Signature& sig) {
    // 1. 选择随机数 k
    element_t k;
    element_init_Zr(k, system->pairing);
    element_random(k);                      // 在 Zr 群中随机生成 k

    // 2. 计算 R = k * P 并将其存储在签名结构体中
    element_pow_zn(sig.R, system->P, k);

    // 3. 计算 H2(m) * P
    element_t H2_mP;
    element_init_G1(H2_mP, system->pairing);
    element_t H2_m;
    element_init_Zr(H2_m, system->pairing);
    hash_to_element(reinterpret_cast<const unsigned char*>(message), strlen(message), H2_m);    // 计算 H2(m)
    element_pow_zn(H2_mP, system->P, H2_m); // 计算 H2(m) * P

    // 4. 提取 R 的横坐标并将其转化为 Zr 群中的元素 xR
    element_t xR;
    element_init_Zr(xR, system->pairing);
    int length = element_length_in_bytes(sig.R);
    char buffer[length];
    element_to_bytes(reinterpret_cast<unsigned char*>(buffer), sig.R);
    hash_to_element(reinterpret_cast<const unsigned char*>(buffer), length, xR);                // 将 R 的横坐标映射到 Zr 群

    // 5. 计算 S = k^-1 * (H2(m) * P + xR * d_ID)
    element_t xR_dID, temp;
    element_init_G1(xR_dID, system->pairing);
    element_pow_zn(xR_dID, d_ID, xR);       // 计算 xR * d_ID

    element_init_G1(temp, system->pairing);
    element_add(temp, H2_mP, xR_dID);       // 计算 H2(m) * P + xR * d_ID

    // 计算 k 的逆元
    element_t k_inv;
    element_init_Zr(k_inv, system->pairing);
    element_invert(k_inv, k);               // 计算 k 的逆元 k^-1

    element_pow_zn(sig.S, temp, k_inv);     // 计算 S = k^-1 * (H2(m) * P + xR * d_ID)

    // 清理临时变量
    element_clear(k);
    element_clear(H2_mP);
    element_clear(H2_m);
    element_clear(xR);
    element_clear(xR_dID);
    element_clear(temp);
    element_clear(k_inv);
}

// 验证签名函数：验证指定的签名是否有效
bool User::verifySignature(User& prover, const char* message, Signature& sig) {
    element_t H2_m, H2_mP, left, right, temp;

    // 1. 计算 H2(m) 并将其乘以 P 得到 H2(m) * P
    element_init_Zr(H2_m, system->pairing);
    element_init_G1(H2_mP, system->pairing);
    hash_to_element(reinterpret_cast<const unsigned char*>(message), strlen(message), H2_m);    // 计算 H2(m)
    element_pow_zn(H2_mP, system->P, H2_m); // 计算 H2(m) * P

    // 2. 提取 R 的横坐标并将其映射到 Zr 群中作为 xR
    element_t xR;
    element_init_Zr(xR, system->pairing);
    int length = element_length_in_bytes(sig.R);
    char buffer[length];
    element_to_bytes(reinterpret_cast<unsigned char*>(buffer), sig.R);
    hash_to_element(reinterpret_cast<const unsigned char*>(buffer), length, xR);                // 将 R 的横坐标映射到 Zr 群

    // 3. 左侧计算：e(R, S)
    element_init_GT(left, system->pairing);
    pairing_apply(left, sig.R, sig.S, system->pairing);                 // 计算 e(R, S)

    // 4. 右侧计算：e(P, H2(m) * P + Q_ID^xR)
    element_init_GT(right, system->pairing);
    pairing_apply(right, system->P, H2_mP, system->pairing);            // 计算 e(P, H2(m) * P)

    element_init_GT(temp, system->pairing);
    pairing_apply(temp, system->P_pub, prover.Q_ID, system->pairing);   // 计算 e(P_pub, Q_ID)
    element_pow_zn(temp, temp, xR);         // 计算 Q_ID^xR

    element_mul(right, right, temp);        // 计算右侧 e(P, H2(m) * P) * e(P_pub, Q_ID)^xR

    // 5. 比较左侧和右侧是否相等
    bool is_valid = (element_cmp(left, right) == 0);

    // 清理临时变量
    element_clear(H2_m);
    element_clear(H2_mP);
    element_clear(left);
    element_clear(right);
    element_clear(temp);
    element_clear(xR);

    return is_valid;
}

// 哈希函数：将输入数据哈希到群元素
void User::hash_to_element(const unsigned char* input, int length, element_t& output) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    const EVP_MD* md = EVP_sha256();
    EVP_DigestInit_ex(context, md, nullptr);
    EVP_DigestUpdate(context, input, length);
    EVP_DigestFinal_ex(context, hash, nullptr);
    EVP_MD_CTX_free(context);
    element_from_hash(output, hash, SHA256_DIGEST_LENGTH);
}
