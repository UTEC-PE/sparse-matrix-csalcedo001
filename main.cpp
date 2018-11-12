#include <iostream>

#include "matrix.h"

#define WIDTH 4
#define HEIGHT 3

using namespace std;

int main(int argc, char *argv[]) {
    Matrix<int> a(WIDTH, HEIGHT);

    a.iterator([&a] (int x, int y) {
      a.set(x, y, x + y * WIDTH);
    });

    a.print();

    system("PAUSE");
    return EXIT_SUCCESS;
}
