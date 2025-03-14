cmake_minimum_required(VERSION 3.28.6)
project(pkc)

set(CMAKE_CXX_STANDARD 11)

# 添加 include 目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 设置 GMP 和 GMPXX 的库路径
set(GMP_LIBRARIES /opt/homebrew/lib/libgmp.dylib /opt/homebrew/lib/libgmpxx.dylib)

# 设置 OpenSSL 库的路径
set(OPENSSL_LIBRARIES /opt/homebrew/opt/openssl/lib/libssl.dylib /opt/homebrew/opt/openssl/lib/libcrypto.dylib)

# 指定包含的头文件目录
include_directories(/opt/homebrew/include /opt/homebrew/opt/openssl/include)

# Glob 所有 src 目录下的 cpp 文件
file(GLOB SRC_FILES "src/*.cpp")

# Glob 所有 include 目录下的 h 文件
file(GLOB HEADER_FILES "include/*.h")

# 创建共享库
add_library(pkc SHARED ${SRC_FILES} ${HEADER_FILES}
        src/ElGamal_BlindSignature.cpp
        include/ElGamal_BlindSignature.h)

# 链接 GMP 和 OpenSSL 库
target_link_libraries(pkc ${GMP_LIBRARIES} ${OPENSSL_LIBRARIES})