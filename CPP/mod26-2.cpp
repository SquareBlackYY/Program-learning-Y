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

void solveEquations(int x1, int y1, int x2, int y2) {
    int a, b;
    int m = 26;

    int xDiff = (x1 - x2 + m) % m;
    int yDiff = (y1 - y2 + m) % m;

    int aInverse = modInverse(xDiff, m);
    if (aInverse == -1) {
        std::cout << "No solution exists." << std::endl;
        return;
    }

    a = (yDiff * aInverse) % m;
    b = (y1 - (a * x1) % m + m) % m;

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
}

int main() {
    int x1, y1, x2, y2;

    std::cout << "Enter x1: ";
    std::cin >> x1;
    std::cout << "Enter y1: ";
    std::cin >> y1;
    std::cout << "Enter x2: ";
    std::cin >> x2;
    std::cout << "Enter y2: ";
    std::cin >> y2;

    solveEquations(x1, y1, x2, y2);

    return 0;
}
