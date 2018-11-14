#ifndef NODE_H
#define NODE_H

using namespace std;

template <class T>
class Node {
public:
    int x, y;
    T data;
    Node <T> *next;
    Node <T> *down;

    Node <T> () : x(-1), y(-1), data(0), next(nullptr), down(nullptr) {};
    Node <T> (int x, int y, T data) : data(data), x(x), y(y), next(nullptr), down(nullptr) {};

    void killSelf() {
      // cout << "    killSelf " << this << endl;
      if (down) {
        // cout << "    killSelfTrue " << this << endl;
        down->killSelf();
      }
      // cout << "    killSelf delete " << this << endl;

      delete this;
    }
};

#endif
