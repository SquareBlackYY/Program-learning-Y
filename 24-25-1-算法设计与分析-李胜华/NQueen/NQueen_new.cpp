#include <vector>
#include <string>
#include <iostream>
#include <chrono>
using namespace std;

// 全局变量，用于存储所有解法
vector<vector<string>> solutions;

double time1 = 0, time2 = 0, time3 = 0;

// 方法一：使用递归 + 回溯
void solveNQueens_Backtracking(int n, int row, vector<int>& queens, vector<bool>& columns, vector<bool>& diag1, vector<bool>& diag2) {
    if (row == n) {  // 完成所有行的皇后放置
        vector<string> board(n, string(n, '.'));
        for (int i = 0; i < n; i++)
            board[i][queens[i]] = 'Q';
        solutions.push_back(board);  // 直接覆盖写入solutions
        return;
    }
    for (int col = 0; col < n; col++) {
        // 检查当前位置是否被其他皇后攻击
        if (!columns[col] && !diag1[row - col + n - 1] && !diag2[row + col]) {
            // 设置状态
            queens[row] = col;
            columns[col] = diag1[row - col + n - 1] = diag2[row + col] = true;
            // 递归放置下一行的皇后
            solveNQueens_Backtracking(n, row + 1, queens, columns, diag1, diag2);
            // 撤销状态
            columns[col] = diag1[row - col + n - 1] = diag2[row + col] = false;
        }
    }
}

// 方法二：使用迭代法模拟回溯
void solveNQueens_Iterative(int n) {
    vector<int> queens(n, -1);  // 初始化皇后位置
    vector<bool> columns(n, false), diag1(2 * n - 1, false), diag2(2 * n - 1, false);
    int row = 0, col = 0;
    while (row >= 0) {
        while (col < n && (columns[col] || diag1[row - col + n - 1] || diag2[row + col])) {
            col++;  // 查找可以放置的位置
        }
        if (col < n) {  // 放置皇后
            queens[row] = col;
            columns[col] = diag1[row - col + n - 1] = diag2[row + col] = true;
            if (row == n - 1) {  // 如果完成了放置
                vector<string> board(n, string(n, '.'));
                for (int i = 0; i < n; i++)
                    board[i][queens[i]] = 'Q';
                solutions.push_back(board);  // 直接覆盖写入solutions
            } else {
                row++;  // 移动到下一行
                col = 0;
                continue;
            }
        }
        // 回溯
        col = queens[row];
        columns[col] = diag1[row - col + n - 1] = diag2[row + col] = false;
        queens[row] = -1;
        row--;
        col++;
    }
}

// 方法三：位运算方法
void solveNQueens_Bitwise(int n, int row, int cols, int diags1, int diags2, vector<int>& queens) {
    if (row == n) {
        vector<string> board(n, string(n, '.'));
        for (int i = 0; i < n; i++)
            board[i][queens[i]] = 'Q';
        solutions.push_back(board);  // 直接覆盖写入solutions
        return;
    }
    int availablePositions = ((1 << n) - 1) & ~(cols | diags1 | diags2);  // 找到所有可以放置的位置
    while (availablePositions) {
        int position = availablePositions & -availablePositions;  // 选择最低位的可用位置
        availablePositions &= availablePositions - 1;  // 清除最低位的1
        int col = __builtin_ctz(position);  // 计算列号
        queens[row] = col;
        solveNQueens_Bitwise(n, row + 1, cols | position, (diags1 | position) << 1, (diags2 | position) >> 1, queens);
    }
}

// 计时并调用各方法
void solveNQueens(int n) {
    vector<int> queens(n, -1);

    solutions.clear();
    auto start = chrono::high_resolution_clock::now();
    vector<bool> columns(n, false), diag1(2 * n - 1, false), diag2(2 * n - 1, false);
    solveNQueens_Backtracking(n, 0, queens, columns, diag1, diag2);
    auto end = chrono::high_resolution_clock::now();
    time1 += chrono::duration<double, milli>(end - start).count();
    cout << "方法一：递归 + 回溯，时间: " << chrono::duration<double, milli>(end - start).count() << " ms" << endl;

    solutions.clear();
    start = chrono::high_resolution_clock::now();
    solveNQueens_Iterative(n);
    end = chrono::high_resolution_clock::now();
    time2 += chrono::duration<double, milli>(end - start).count();
    cout << "方法二：迭代法，时间: " << chrono::duration<double, milli>(end - start).count() << " ms" << endl;

    solutions.clear();
    start = chrono::high_resolution_clock::now();
    solveNQueens_Bitwise(n, 0, 0, 0, 0, queens);
    end = chrono::high_resolution_clock::now();
    time3 += chrono::duration<double, milli>(end - start).count();
    cout << "方法三：位运算，时间: " << chrono::duration<double, milli>(end - start).count() << " ms" << endl;
}

int main() {
    int n = 10; // 更大规模的 N 值以增加运行时间

    int num = 5;
    for (int i = 0; i < num; i++) { // 多次调用函数进行测试
        cout << "测试第 " << i + 1 << " 次:" << endl;
        solveNQueens(n);
        cout << endl;
    }

    cout << "方法一 平均时间：" << time1 / num << " ms" << endl;
    cout << "方法二 平均时间：" << time2 / num << " ms" << endl;
    cout << "方法三 平均时间：" << time3 / num << " ms" << endl;

    return 0;
}