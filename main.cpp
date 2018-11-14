#include <iostream>

#include "matrix.h"

#define WIDTH 2
#define HEIGHT 3

using namespace std;

int main(int argc, char *argv[]) {
    Matrix<int> a(WIDTH, HEIGHT), b(HEIGHT, WIDTH);

    a.iterator([&] (int x, int y) -> void {
      cout << a(x, y) << endl;
    }, 'n', 'd');

    a.iterator([&a] (int x, int y) {
      a.set(x, y, x + y * WIDTH);
    });
    b.iterator([&b] (int x, int y) {
      b.set(x, y, x + y * HEIGHT);
    });

    a.print();
    b.print();

    Matrix<int> c;
    c = a * b;

    c.print();

    c = c * 4;

    c.print();

    Matrix<int> d = c;

    d = c + c + c;

    d.print();

    d = d - c;

    d.print();

    d = d.transposed();

    d.print();

    cout << endl << endl;

    system("PAUSE");
    return EXIT_SUCCESS;
}
