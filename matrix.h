#ifndef MATRIX_H
#define MATRIX_H

#include "node.h"

using namespace std;

template <typename T>
class Matrix {
    private:
        Node <T> *hRows;
        Node <T> *hColumns;
        int columns;
        int rows;

    public:
        Matrix() : hRows(nullptr), hColumns(nullptr) {};
        Matrix(int sizeX, int sizeY) : columns(sizeX), rows(sizeY) {
          if (sizeX <= 0 || sizeY <= 0) {
            throw 1;
          }

          Node <T> **currentPtr = &this->hColumns;

          while (sizeX--) {
            *currentPtr = new Node <T>();
            currentPtr = &((*currentPtr)->next);
          }

          currentPtr = &this->hRows;

          while (sizeY--) {
            *currentPtr = new Node <T>();
            currentPtr = &((*currentPtr)->down);
          }
        }
        Matrix(Matrix<T> &other) : Matrix(other.columns, other.rows) {
          other.iterator([&] (int x, int y) {
            this->set(x, y, other(x, y));
          });
        };

        void set(int x, int y, T data) {
          if (x < 0 || x >= this->columns || y < 0 || y >= this->rows) {
            throw 1;
          }

          Node <T> **current = &this->hColumns;

          for (int i = 0; i < x; i++) {
            current = &((*current)->next);
          }

          current = &((*current)->down);


          while (*current && (*current)->y < y) {
            current = &((*current)->down);
          }





          Node <T> *tmpNode, *newNode;




          if (*current && (*current)->y == y) {
            if (data) {
              (*current)->data = data;
              return;
            }

            *current = (*current)->down;
          } else if (data) {
            tmpNode = *current ? (*current)->down : nullptr;
            newNode = *current = new Node <T>(x, y, data);

            newNode->down = tmpNode;
          }




          current = &this->hRows;

          for (int i = 0; i < y; i++) {
            current = &((*current)->down);
          }

          current = &((*current)->next);

          while (*current && (*current)->x < x) {
            current = &((*current)->next);
          }





          if (*current && (*current)->x == x && !data) {
            tmpNode = *current;
            *current = (*current)->next;
            delete *current;
          } else if (data) {
            tmpNode = *current ? (*current)->next : nullptr;
            *current = newNode;

            newNode->next = tmpNode;
          }
        }
        T operator()(int x, int y) {
          if (x < 0 || x >= this->columns || y < 0 || y >= this->rows) {
            throw 1;
          }

          Node <T> *current = this->hColumns;

          while (x--) {
            current = current->next;
          }

          while (current) {
            if (current->y == y) {
              return current->data;
            }

            current = current->down;
          }

          return 0;
        }
        Matrix<T> operator*(Matrix<T> other) {
          if (other.rows != this->columns) {
            throw 1;
          }

          Matrix<T> result(other.columns, this->rows);

          result.iterator([&] (int x, int y) {
            int sum = 0;

            for (int i = 0; i < this->columns; i++) {
              sum += (*this)(i, y) * other(x, i);
            }

            result.set(x, y, sum);
          });

          return result;
        }
        Matrix<T> operator*(T scalar) {
          Matrix<T> result(*this);

          if (scalar) {
            result.iterator([&] (int x, int y) -> void {
              result.set(x, y, result(x, y) * scalar);
            }, 'n');
          }

          return result;
        }
        Matrix<T> operator+(Matrix<T> other) {
          if (this->columns != other.columns || this->rows != other.rows) {
            throw 1;
          }

          Matrix<T> result(*this);

          result.iterator([&] (int x, int y) -> void {
            result.set(x, y, (*this)(x, y) + other(x, y));
          });

          return result;
        }
        Matrix<T> operator-(Matrix<T> other) {
          if (this->columns != other.columns || this->rows != other.rows) {
            throw 1;
          }

          Matrix<T> result(*this);

          result.iterator([&] (int x, int y) -> void {
            result.set(x, y, (*this)(x, y) - other(x, y));
          });

          return result;
        }
        Matrix<T> transposed() {
          Matrix<T> result(this->rows, this->columns);

          iterator([&] (int x, int y) -> void {
            result.set(y, x, (*this)(x, y));
          });

          return result;
        }
        Matrix<T> operator=(Matrix<T> other) {
          this->clear();

          this->columns = other.columns;
          this->rows = other.rows;

          Node <T> **currentPtr = &this->hColumns;

          for (int i = 0; i < this->columns; i++) {
            *currentPtr = new Node <T>();
            currentPtr = &((*currentPtr)->next);
          }

          currentPtr = &this->hRows;

          for (int i = 0; i < this->rows; i++) {
            *currentPtr = new Node <T>();
            currentPtr = &((*currentPtr)->down);
          }

          this->iterator([&] (int x, int y) -> void {
            T result = other(x, y);

            if (result) {
              this->set(x, y, result);
            }
          });

          return *this;
        }
        void clear() {
          Node <T> *current = this->hColumns, *prev = this->hRows;

          while (current) {
            if (prev) {
              prev->killSelf();
            }

            prev = current;
            current = current->next;
          }


          if (prev) {
            prev->killSelf();
          }
        }

        void print() {
          cout << endl;

          for (int i = 0; i < this->rows; i++) {
            cout << '|';

            for (int j = 0; j < this->columns; j++) {
              cout << ' ' << (*this)(j, i);
            }

            cout << " |" << endl;
          }
        }
        void iterator(function <void (int, int)> step = [] (int x, int y) -> void {},
                      char mode = 'f',
                      char direction = 'n') {
          if (mode == 'f') { // Mode: Full iteration. Every cell in the matrix is covered
            if (direction == 'n') { // Direction: Next first
              for (int row = 0; row < this->rows; row++) {
                for (int column = 0; column < this->columns; column++) {
                  step(column, row);
                }
              }
            } else if (direction == 'd') { // Direction: Down first
              for (int column = 0; column < this->columns; column++) {
                for (int row = 0; row < this->rows; row++) {
                  step(column, row);
                }
              }
            } else {
              throw 1;
            }
          } else if (mode == 'n') { // Mode: Node iteration. Every node in the matrix (avoids 0-cells)
            if (direction == 'n') { // Direction: Next first
              for (Node <T> *currentDown = hRows; currentDown; currentDown = currentDown->down) {
                for (Node <T> *currentNext = currentDown->next; currentNext; currentNext = currentNext->next) {
                  step(currentNext->x, currentNext->y);
                }
              }
            } else if (direction == 'd') { // Direction: Down first
              for (Node <T> *currentNext = hColumns; currentNext; currentNext = currentNext->next) {
                for (Node <T> *currentDown = currentNext->down; currentDown; currentDown = currentDown->down) {
                  step(currentDown->x, currentDown->y);
                }
              }
            } else {
              throw 1;
            }
          } else {
            throw 1;
          }
        }

        ~Matrix() {
          this->clear();
        }
};

#endif
