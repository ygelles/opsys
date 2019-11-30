//
// Created by yuvalgelles on 30/11/2019.
//
#include <vector>
#include <string>
#include <iostream>
#ifndef SMASH_FIFO50_H
#define SMASH_FIFO50_H
using namespace std;

//a data structure for FIFO
template <typename T,int max>
class Fifo : public vector<T> {
public:

    void push(const T &value) {
      if (this->size() == max) {
        this->erase(this->begin());
      }
      this->push_back(value);
    }

    void print() {
      if (this->empty()) {
        return;
      }
      for (unsigned int i = 0; i < this->size(); ++i) {
        cout << this->at(i);
      }
    }
};



#endif //SMASH_FIFO50_H
