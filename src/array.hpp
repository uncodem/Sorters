#ifndef ARRAY_HPP_
#define ARRAY_HPP_

#include <mutex>
#include <vector>

class Observer;

class Array {
   public:
    explicit Array(size_t n, Observer *obs = nullptr);
    void shuffle();

    void swap(size_t i, size_t j);
    void setObserver(Observer *obs);

    void insert(size_t i, int v);
    void insert(int v);

    int pop(size_t i);
    int pop();

    int compare(size_t i, size_t j) const;
    int compareValue(size_t i, int v) const;

    int at(size_t i) const;
    size_t size() const;
    void dump() const;

    std::vector<int> getCopy() const;

   private:
    std::vector<int> data;
    mutable Observer *observer;
    mutable std::mutex mtx;
};

#endif
