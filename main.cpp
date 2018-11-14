#include <iostream>

#include "matrix.h"
#include <random>
#include <chrono>

#define WIDTH 2
#define HEIGHT 2

using namespace std;

int main(int argc, char *argv[]) {
    Matrix<int> a(WIDTH, HEIGHT), b(HEIGHT, WIDTH);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);

    uniform_int_distribution<int> distribution(0, 5);

    a.iterator([&] (int x, int y) -> void {
      cout << a(x, y) << endl;
    }, 'n', 'd');

    a.iterator([&] (int x, int y) {
      if (x % 2 || y % 2)
        a.set(x, y, distribution(generator));
    });
    b.iterator([&] (int x, int y) {
      if (x % 2 || y % 2)
        b.set(x, y, distribution(generator));
    });

    cout << "a =" << endl;

    a.print();

    cout << "b =" << endl;

    b.print();

    cout << "a\' =" << endl;

    a.transposed().print();

    cout << "a * 10 =" << endl;

    (a * 10).print();

    cout << "a' + b =" << endl;

    (a.transposed() + b).print();

    cout << "a' - b =" << endl;

    (a.transposed() - b).print();

    cout << "a * b =" << endl;

    (a * b).print();

    cout << endl << endl;

    system("PAUSE");
    return EXIT_SUCCESS;
}
