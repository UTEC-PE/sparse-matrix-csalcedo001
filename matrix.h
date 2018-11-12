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

          if (*current && (*current)->y == y) {
            if (data) {
              (*current)->data = data;
              return;
            }

            *current = (*current)->down;
          }

          Node <T> *tmpNode = *current ? (*current)->down : nullptr;
          Node <T> *newNode = *current = new Node <T>(x, y, data);

          newNode->down = tmpNode;


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
          }

          tmpNode = *current ? (*current)->next : nullptr;

          newNode->next = tmpNode;
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
        Matrix<T> operator*(Matrix<T> other);
        Matrix<T> operator*(T scalar);
        Matrix<T> operator+(Matrix<T> other);
        Matrix<T> operator-(Matrix<T> other);
        Matrix<T> transposed();
        Matrix<T> operator=(Matrix<T> other);
        void print() {
          for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->columns; j++) {
              cout << (*this)(j, i) << ' ';
            }
            cout << endl;
          }
        }
        void iterator(function<void (int, int)> step = [] (int x, int y) -> void {}) {
          for (int row = 0; row < this->rows; row++) {
            for (int column = 0; column < this->columns; column++) {
              step(column, row);
            }
          }
        }

        ~Matrix() {
          Node <T> *current = this->hColumns, *prev = this->hRows;

          while (current) {
            prev->killSelf();
            prev = current;
            current = current->next;
          }

          prev->killSelf();
        }
};

#endif
