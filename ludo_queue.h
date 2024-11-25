#ifndef LUDO_QUEUE
#define LUDO_QUEUE

#include <iostream>
#include <queue>

template <class T>
class ludo_queue {
private:
    std::queue<T> q;
    int maxSize;

public:
    ludo_queue(int maxSize) : maxSize(maxSize) {}

    void push(T value) {
        if (q.size() < maxSize) {
            q.push(value);
        } else {
            std::cout << "Queue is full!" << std::endl;
        }
    }

    void pop() {
        q.pop();
    }

    T front() {
        return q.front();
    }

    bool empty() {
        return q.empty();
    }

    bool full() {
        return q.size() == maxSize;
    }

    int size() {
        return q.size();
    }
};

#endif