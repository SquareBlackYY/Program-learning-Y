#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DISK_SIZE 100           // 虚拟磁盘块数
#define MAX_USERS 10            // 最大用户数量
#define MAX_FILES_PER_USER 50   // 每个用户的最大文件数量
#define MAX_DIRS_PER_USER 10    // 每个用户的最大目录数量

// 权限类型(BitMap)
typedef enum {
    NONE,           // ---
    EXECUTE,        // --x
    WRITE,          // -w-
    WRITE_EXECUTE,  // -wx
    READ,           // r--
    READ_EXECUTE,   // r-x
    READ_WRITE,     // rw-
    ALL             // rwx
} Permission;

// 文件结构体
typedef struct File {
    char name[100];     // 文件名
    int size;           // 文件大小（块数）
    int start;          // 文件起始块
    Permission perm;    // 文件权限
} File;

// 目录结构体
typedef struct Directory {
    char name[100];                                 // 目录名
    File *files[MAX_FILES_PER_USER];                // 目录内文件
    struct Directory *subDirs[MAX_DIRS_PER_USER];   // 子目录
    int fileCount;                                  // 文件数量
    int subDirCount;                                // 子目录数量
} Directory;

// 用户结构体
typedef struct User {
    char username[100]; // 用户名
    Directory rootDir;  // 用户的根目录
} User;

// 文件系统结构体
typedef struct FileSystem {
    int disk[DISK_SIZE];    // 磁盘块数组，0表示空闲，1表示占用
    User users[MAX_USERS];  // 用户数组
    int userCount;          // 当前用户数量
} FileSystem;

void initFileSystem(FileSystem *fs);

void createUser(FileSystem *fs, const char *username);

void createDirectory(FileSystem *fs, const char *username, const char *parentPath, const char *dirName);

void createFile(FileSystem *fs, const char *username, const char *dirPath, const char *name, int size, Permission perm);

void deleteUser(FileSystem *fs, const char *username);

void deleteDirectory(FileSystem *fs, const char *username, const char *dirPath);

void deleteFile(FileSystem *fs, const char *username, const char *dirPath, const char *name);

void renameFile(FileSystem *fs, const char *username, const char *dirPath, const char *oldName, const char *newName);

void moveFile(FileSystem *fs, const char *username, const char *srcPath, const char *destPath, const char *fileName);

void listDirectory(FileSystem *fs, const char *username, const char *dirPath);

void formatDisk(FileSystem *fs);

void printDiskStatus(FileSystem *fs);

User *findUser(FileSystem *fs, const char *username);

Directory *navigateToDirectory(Directory *root, const char *path);

void freeDirectory(FileSystem *fs, Directory *dir);

static int compareFiles(const void *a, const void *b);

int getFreeSpace(const FileSystem *fs);

void inputString(char *buffer, size_t size, const char *prompt);

void formatPermissions(char *buffer, int permissions);

int main() {
    // 初始化文件系统及相关参数
    FileSystem fs;
    initFileSystem(&fs);
    char username[100], dirPath[100], fileName[100], newName[100], parentPath[100], dirName[100];
    int choice, size, confirm;

    while (1) {
        printf("================文件管理系统================\n");
        printf("[用户操作]\t 1.创建用户\t 2.删除用户\n");
        printf("[目录操作]\t 3.创建目录\t 4.删除目录\n");
        printf("[文件操作]\t 5.创建文件\t 6.删除文件\n");
        printf("\t\t 7.重命名\t 8.移动文件\n");
        printf("[系统操作]\t 9.列出目录\t10.格式化\n");
        printf("\t\t11.磁盘状态\t 0.退出系统\n");
        printf("--------------------------------------------\n");
        printf("请选择操作 > ");
        scanf("%d", &choice);
        getchar(); // 清除换行符
        printf("--------------------------------------------\n");

        switch (choice) {
            case 1: // 创建用户
                inputString(username, sizeof(username), "输入用户名：");
                createUser(&fs, username);
                break;

            case 2: // 删除用户
                inputString(username, sizeof(username), "输入要删除的用户名：");
                deleteUser(&fs, username);
                break;

            case 3: // 创建目录
                inputString(username, sizeof(username), "输入用户名：");
                inputString(parentPath, sizeof(parentPath), "输入父目录路径：");
                inputString(dirName, sizeof(dirName), "输入新目录名：");
                createDirectory(&fs, username, parentPath, dirName);
                break;

            case 4: // 删除目录
                inputString(username, sizeof(username), "输入用户名：");
                inputString(dirPath, sizeof(dirPath), "输入要删除的目录路径：");
                deleteDirectory(&fs, username, dirPath);
                break;

            case 5: // 创建文件
                inputString(username, sizeof(username), "输入用户名：");
                inputString(dirPath, sizeof(dirPath), "输入目录路径：");
                inputString(fileName, sizeof(fileName), "输入文件名：");

                char size_str[10];
                inputString(size_str, sizeof(size_str), "输入文件大小（块数）：");
                size = atoi(size_str);

                printf("\n权限选项：\n");
                printf("0 - 无权限 (---)\n");
                printf("1 - 仅执行 (--x)\n");
                printf("2 - 仅写入 (-w-)\n");
                printf("3 - 写入执行 (-wx)\n");
                printf("4 - 仅读取 (r--)\n");
                printf("5 - 读取执行 (r-x)\n");
                printf("6 - 读取写入 (rw-)\n");
                printf("7 - 所有权限 (rwx)\n");

                char perm_str[10];
                inputString(perm_str, sizeof(perm_str), "请选择权限（0-7）：");
                int perm_val = atoi(perm_str);
                if (perm_val < 0 || perm_val > 7) {
                    printf("无效的权限值！使用默认权限（只读）\n");
                    perm_val = READ;
                }

                createFile(&fs, username, dirPath, fileName, size, perm_val);
                break;

            case 6: // 删除文件
                inputString(username, sizeof(username), "输入用户名：");
                inputString(dirPath, sizeof(dirPath), "输入目录路径：");
                inputString(fileName, sizeof(fileName), "输入要删除的文件名：");
                deleteFile(&fs, username, dirPath, fileName);
                break;

            case 7: // 重命名文件
                inputString(username, sizeof(username), "输入用户名：");
                inputString(dirPath, sizeof(dirPath), "输入目录路径：");
                inputString(fileName, sizeof(fileName), "输入原文件名：");
                inputString(newName, sizeof(newName), "输入新文件名：");
                renameFile(&fs, username, dirPath, fileName, newName);
                break;

            case 8: // 移动文件
                inputString(username, sizeof(username), "输入用户名：");
                inputString(dirPath, sizeof(dirPath), "输入源目录路径：");
                inputString(newName, sizeof(newName), "输入目标目录路径：");
                inputString(fileName, sizeof(fileName), "输入要移动的文件名：");
                moveFile(&fs, username, dirPath, newName, fileName);
                break;

            case 9: // 列出目录
                inputString(username, sizeof(username), "输入用户名：");
                inputString(dirPath, sizeof(dirPath), "输入目录路径：");
                listDirectory(&fs, username, dirPath);
                break;

            case 10: // 格式化磁盘
                printf("确认要格式化磁盘吗？(1:是 0:否)：");
                scanf("%d", &confirm);
                getchar(); // 清除换行符
                if (confirm == 1) {
                    formatDisk(&fs);
                    printf("磁盘已格式化完成。\n");
                }
                break;

            case 11: // 显示磁盘状态
                printDiskStatus(&fs);
                break;

            case 0: // 退出
                printf("再见！\n");
                return 0;

            default: // 无效选项
                printf("请重试。\n");
                break;
        }
    }
}

// 初始化文件系统
void initFileSystem(FileSystem *fs) {
    memset(fs->disk, 0, sizeof(fs->disk)); // 初始化所有磁盘块为空闲
    fs->userCount = 0; // 初始化用户数量为0
}

// 创建用户
void createUser(FileSystem *fs, const char *username) {
    // 检查用户数量是否已达上限
    if (fs->userCount >= MAX_USERS) {
        printf("用户数量已达上限。\n");
        return;
    }

    // 检查用户名是否已存在
    if (findUser(fs, username) != NULL) {
        printf("用户 %s 已存在。\n", username);
        return;
    }

    // 创建新用户并初始化用户信息
    User *newUser = &fs->users[fs->userCount++]; // 获取新用户的指针并增加用户计数
    strcpy(newUser->username, username); // 设置用户名
    strcpy(newUser->rootDir.name, "/"); // 设置根目录名为 "/"
    newUser->rootDir.fileCount = 0; // 初始化根目录文件数量为 0
    newUser->rootDir.subDirCount = 0; // 初始化根目录子目录数量为 0
    printf("用户创建成功：%s\n", username); // 输出用户创建成功信息
}

// 创建目录
void createDirectory(FileSystem *fs, const char *username, const char *parentPath, const char *dirName) {
    User *user = findUser(fs, username);
    if (user == NULL) {
        printf("用户不存在：%s\n", username);
        return;
    }

    Directory *parentDir = navigateToDirectory(&user->rootDir, parentPath);
    if (!parentDir) {
        printf("父目录不存在：%s\n", parentPath);
        return;
    }

    for (int i = 0; i < parentDir->subDirCount; i++) {
        if (strcmp(parentDir->subDirs[i]->name, dirName) == 0) {
            printf("目录已存在：%s\n", dirName);
            return;
        }
    }

    if (parentDir->subDirCount >= MAX_DIRS_PER_USER) {
        printf("子目录数量已达上限。\n");
        return;
    }

    Directory *newDir = (Directory *) malloc(sizeof(Directory));
    strcpy(newDir->name, dirName);
    newDir->fileCount = 0;
    newDir->subDirCount = 0;

    parentDir->subDirs[parentDir->subDirCount++] = newDir;
    printf("目录创建成功：%s\n", dirName);
}

// 创建文件
void createFile(FileSystem *fs, const char *username, const char *dirPath, const char *name, const int size,
                const Permission perm) {
    // 查找用户
    User *user = findUser(fs, username);
    if (user == NULL) {
        printf("用户不存在：%s\n", username);
        return;
    }

    // 导航到指定目录
    Directory *dir = navigateToDirectory(&user->rootDir, dirPath);
    if (!dir) {
        printf("目录不存在：%s\n", dirPath);
        return;
    }

    // 检查文件是否已存在
    for (int i = 0; i < dir->fileCount; i++) {
        if (strcmp(dir->files[i]->name, name) == 0) {
            printf("文件已存在：%s\n", name);
            return;
        }
    }

    // 检查磁盘空间
    int freeBlocks = 0;
    for (int i = 0; i < DISK_SIZE; i++) {
        if (fs->disk[i] == 0) freeBlocks++;
    }
    if (freeBlocks < size) {
        printf("磁盘空间不足。\n");
        return;
    }

    // 检查目录文件数量是否已达上限
    if (dir->fileCount >= MAX_FILES_PER_USER) {
        printf("目录文件数量已达上限。\n");
        return;
    }

    // 创建新文件
    File *newFile = (File *) malloc(sizeof(File));
    strcpy(newFile->name, name);
    newFile->size = size;
    newFile->perm = perm;

    // 分配磁盘块
    int allocated = 0;
    for (int i = 0; i < DISK_SIZE && allocated < size; i++) {
        if (fs->disk[i] == 0) {
            fs->disk[i] = 1;
            if (allocated == 0) newFile->start = i;
            allocated++;
        }
    }

    // 将新文件添加到目录
    dir->files[dir->fileCount++] = newFile;

    // 显示创建成功信息，包含权限
    char permStr[4];
    formatPermissions(permStr, perm);
    printf("文件创建成功：%s (权限：%s)\n", name, permStr);
}

// 删除用户
void deleteUser(FileSystem *fs, const char *username) {
    // 直接开始查找用户
    for (int i = 0; i < fs->userCount; i++) {
        if (!strcmp(fs->users[i].username, username)) {
            // 递归删除用户根目录下的所有内容
            Directory *root = &fs->users[i].rootDir;
            freeDirectory(fs, root);

            // 移动后面的用户前移
            memmove(&fs->users[i], &fs->users[i + 1], (fs->userCount - i - 1) * sizeof(User));
            fs->userCount--;
            printf("用户删除成功：%s\n", username);
            return;
        }
    }
    printf("用户不存在：%s\n", username);
}

// 删除目录的主函数
void deleteDirectory(FileSystem *fs, const char *username, const char *dirPath) {
    // 查找用户
    User *user = findUser(fs, username);
    if (!user) {
        printf("用户不存在：%s\n", username);
        return;
    }

    // 不允许删除根目录
    if (strcmp(dirPath, "/") == 0) {
        printf("不能删除根目录！\n");
        return;
    }

    // 获取目录路径的父目录和目标目录名
    char parentPath[100];
    char targetName[100];

    // 分割路径获取父目录路径和目标目录名
    strcpy(parentPath, dirPath);
    char *lastSlash = strrchr(parentPath, '/');
    if (lastSlash == parentPath) {
        // 如果是根目录下的目录
        strcpy(parentPath, "/");
        strcpy(targetName, dirPath + 1);
    } else {
        *lastSlash = '\0'; // 截断父目录路径
        strcpy(targetName, lastSlash + 1);
    }

    // 找到父目录
    Directory *parentDir = navigateToDirectory(&user->rootDir, parentPath);
    if (!parentDir) {
        printf("父目录不存在：%s\n", parentPath);
        return;
    }

    // 在父目录中查找要删除的目录
    int dirIndex = -1;
    Directory *dirToDelete = NULL;
    for (int i = 0; i < parentDir->subDirCount; i++) {
        if (strcmp(parentDir->subDirs[i]->name, targetName) == 0) {
            dirToDelete = parentDir->subDirs[i];
            dirIndex = i;
            break;
        }
    }

    if (dirIndex == -1) {
        printf("目录不存在：%s\n", targetName);
        return;
    }

    // 递归删除目录内容
    freeDirectory(fs, dirToDelete);

    // 从父目录中移除这个目录
    free(dirToDelete);
    memmove(&parentDir->subDirs[dirIndex], &parentDir->subDirs[dirIndex + 1], (parentDir->subDirCount - dirIndex - 1) * sizeof(Directory *));
    parentDir->subDirCount--;

    printf("目录删除成功：%s\n", dirPath);
}

// 删除文件
void deleteFile(FileSystem *fs, const char *username, const char *dirPath, const char *name) {
    // 查找用户
    User *user = findUser(fs, username);
    if (user == NULL) {
        printf("用户不存在：%s\n", username);
        return;
    }

    // 导航到指定目录
    Directory *dir = navigateToDirectory(&user->rootDir, dirPath);
    if (!dir) {
        printf("目录不存在：%s\n", dirPath);
        return;
    }

    // 查找文件并删除
    for (int i = 0; i < dir->fileCount; i++) {
        if (strcmp(dir->files[i]->name, name) == 0) {
            File *file = dir->files[i];
            // 释放文件占用的磁盘块
            memset(&fs->disk[file->start], 0, file->size * sizeof(int));
            free(file);

            // 移动后面的文件前移
            memmove(&dir->files[i], &dir->files[i + 1], (dir->fileCount - i - 1) * sizeof(File *));
            dir->fileCount--;
            printf("文件删除成功：%s\n", name);
            return;
        }
    }
    // 文件不存在
    printf("文件不存在：%s\n", name);
}

// 重命名文件
void renameFile(FileSystem *fs, const char *username, const char *dirPath, const char *oldName, const char *newName) {
    // 查找用户
    User *user = findUser(fs, username);
    if (user == NULL) {
        printf("用户不存在：%s\n", username);
        return;
    }

    // 导航到指定目录
    Directory *dir = navigateToDirectory(&user->rootDir, dirPath);
    if (!dir) {
        printf("目录不存在：%s\n", dirPath);
        return;
    }

    // 查找文件并重命名
    for (int i = 0; i < dir->fileCount; i++) {
        if (strcmp(dir->files[i]->name, oldName) == 0) {
            strcpy(dir->files[i]->name, newName);
            printf("文件重命名成功：%s -> %s\n", oldName, newName);
            return;
        }
    }
    // 文件不存在
    printf("文件不存在：%s\n", oldName);
}

// 移动文件
void moveFile(FileSystem *fs, const char *username, const char *srcPath, const char *destPath, const char *fileName) {
    // 查找用户
    User *user = findUser(fs, username);
    if (user == NULL) {
        printf("用户不存在：%s\n", username);
        return;
    }

    // 导航到源目录和目标目录
    Directory *srcDir = navigateToDirectory(&user->rootDir, srcPath);
    Directory *destDir = navigateToDirectory(&user->rootDir, destPath);

    // 检查源目录和目标目录是否存在
    if (!srcDir || !destDir) {
        printf("源目录或目标目录不存在\n");
        return;
    }

    File *fileToMove = NULL;
    int fileIndex = -1;

    // 查找源文件
    for (int i = 0; i < srcDir->fileCount; i++) {
        if (!strcmp(srcDir->files[i]->name, fileName)) {
            fileToMove = srcDir->files[i];
            fileIndex = i;
            break;
        }
    }

    // 检查文件是否存在
    if (!fileToMove) {
        printf("源文件不存在\n");
        return;
    }

    // 检查目标目录是否有空间
    if (destDir->fileCount >= MAX_FILES_PER_USER) {
        printf("目标目录已满\n");
        return;
    }

    // 从源目录移除文件
    memmove(&srcDir->files[fileIndex], &srcDir->files[fileIndex + 1], (srcDir->fileCount - fileIndex - 1) * sizeof(File *));
    srcDir->fileCount--;

    // 添加文件到目标目录
    destDir->files[destDir->fileCount++] = fileToMove;
    printf("文件移动成功：%s\n", fileName);
}

// 列出目录内容
void listDirectory(FileSystem *fs, const char *username, const char *dirPath) {
    // 查找用户
    User *user = findUser(fs, username);
    if (user == NULL) {
        printf("用户不存在：%s\n", username);
        return;
    }

    // 导航到指定目录
    Directory *dir = navigateToDirectory(&user->rootDir, dirPath);
    if (!dir) {
        printf("目录不存在：%s\n", dirPath);
        return;
    }

    printf("\n目录内容：%s\n", dirPath);
    printf("--------------------------------------------\n");

    // 显示子目录
    if (dir->subDirCount > 0) {
        printf("子目录：\n");
        for (int i = 0; i < dir->subDirCount; i++) {
            printf("  [DIR] %s\n", dir->subDirs[i]->name);
        }
        printf("--------------------------------------------\n");
    }

    // 使用qsort对文件按起始块号排序
    qsort(dir->files, dir->fileCount, sizeof(File *), compareFiles);

    // 显示文件（按起始块号排序）
    if (dir->fileCount > 0) {
        printf("文件列表（按起始块号排序）：\n");
        printf("--------------------------------------------\n");
        printf("文件名\t\t权限\t大小(块)\t起始块号\n");
        printf("--------------------------------------------\n");
        for (int i = 0; i < dir->fileCount; i++) {
            File *file = dir->files[i];
            char permStr[4];
            formatPermissions(permStr, file->perm);
            printf("%-16s%-8s%-16d%-16d\n",
                   file->name,
                   permStr,
                   file->size,
                   file->start);
        }
        printf("--------------------------------------------\n");
    }

    // 如果目录为空，显示空目录信息
    if (dir->fileCount == 0 && dir->subDirCount == 0) {
        printf("(空目录)\n");
        printf("--------------------------------------------\n");
    }
}

// 磁盘格式化功能
void formatDisk(FileSystem *fs) {
    memset(fs->disk, 0, sizeof(fs->disk)); // 清空磁盘块
    fs->userCount = 0; // 重置用户数量
    printf("磁盘已格式化。\n");
}

// 打印磁盘状态
void printDiskStatus(FileSystem *fs) {
    const int freeBlocks = getFreeSpace(fs);
    printf("磁盘状态：\n");
    printf("总块数：%d\n", DISK_SIZE);
    printf("已用块数：%d\n", DISK_SIZE - freeBlocks);
    printf("空闲块数：%d\n", freeBlocks);
    printf("使用率：%.2f%%\n", (float) (DISK_SIZE - freeBlocks) / DISK_SIZE * 100);
}

// 查找用户，返回指向用户结构体的指针
User *findUser(FileSystem *fs, const char *username) {
    for (int i = 0; i < fs->userCount; i++) {
        if (strcmp(fs->users[i].username, username) == 0) {
            return &fs->users[i];
        }
    }
    return NULL;
}

// 导航到指定目录
Directory *navigateToDirectory(Directory *root, const char *path) {
    if (strcmp(path, "/") == 0) return root; // 根目录

    char tempPath[100];
    strcpy(tempPath, path);
    const char *token = strtok(tempPath, "/");
    Directory *current = root;

    while (token) {
        int found = 0;
        for (int i = 0; i < current->subDirCount; i++) {
            if (strcmp(current->subDirs[i]->name, token) == 0) {
                current = current->subDirs[i];
                found = 1;
                break;
            }
        }
        if (!found) return NULL;
        token = strtok(NULL, "/");
    }
    return current;
}

// 递归删除目录及其内容的辅助函数
void freeDirectory(FileSystem *fs, Directory *dir) {
    // 先递归删除所有子目录
    for (int i = 0; i < dir->subDirCount; i++) {
        freeDirectory(fs, dir->subDirs[i]);
        free(dir->subDirs[i]);
    }

    // 删除目录中的所有文件，并释放对应的磁盘块
    for (int i = 0; i < dir->fileCount; i++) {
        File *file = dir->files[i];
        // 释放文件占用的磁盘块
        memset(&fs->disk[file->start], 0, file->size * sizeof(int));
        free(file);
    }

    // 重置目录的计数器
    dir->fileCount = 0;
    dir->subDirCount = 0;
}

// 比较函数，用于qsort
static int compareFiles(const void *a, const void *b) {
    const File *fileA = *(File **) a;
    const File *fileB = *(File **) b;
    return fileA->start - fileB->start;
}

// 获取空闲空间
int getFreeSpace(const FileSystem *fs) {
    int freeBlocks = 0;
    for (int i = 0; i < DISK_SIZE; i++) {
        if (fs->disk[i] == 0) freeBlocks++;
    }
    return freeBlocks;
}

// 读取字符串
void inputString(char *buffer, size_t size, const char *prompt) {
    printf("%s", prompt);
    if (getline(&buffer, &size, stdin) != -1) {
        buffer[strcspn(buffer, "\n")] = 0; // 去除换行符
    }
}

// 将权限数字转换为 rwx 格式的字符串
void formatPermissions(char *buffer, const int permissions) {
    // buffer 必须至少有 4 个字符的空间（3个权限字符 + 结束符）
    buffer[0] = permissions & 4 ? 'r' : '-'; // 检查读取权限（100）
    buffer[1] = permissions & 2 ? 'w' : '-'; // 检查写入权限（010）
    buffer[2] = permissions & 1 ? 'x' : '-'; // 检查执行权限（001）
    buffer[3] = '\0'; // 字符串结束符
}
