#include <iostream>

int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1;
}

void solveEquations(int x1, int x2, int x3, int y1, int y2, int y3) {
    int a, b;
    int m = 26;

    int x = (x3 - x1 + m) % m;
    int y = (y3 - y1 + m) % m;

    int xInverse = modInverse(x, m);

    if (xInverse == -1) {
        std::cout << "No solution exists." << std::endl;
        return;
    }

    a = (xInverse * y) % m;
    b = ((y3 - y2 * a + m) % m) * modInverse(x2, m) % m;

    std::cout << "a = " << a << ", b = " << b << std::endl;
}

int main() {
    int x1, x2, x3, y1, y2, y3;

    std::cout << "Enter the values of x1, x2, x3, y1, y2, y3: ";
    std::cin >> x1 >> x2 >> x3 >> y1 >> y2 >> y3;

    solveEquations(x1, x2, x3, y1, y2, y3);

    return 0;
}
