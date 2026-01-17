#include "default_observer.hpp"

#include <iostream>

#include "array.hpp"

DefaultObserver::DefaultObserver(const Array &arr) : operations(0), data(arr) {}

void DefaultObserver::onSwap(size_t i, size_t j) {
    int a = this->data.at(i), b = this->data.at(j);
    std::cout << "Swapping : " << a << " & " << b << std::endl;
    ++this->operations;
}

void DefaultObserver::onCompare(size_t i, size_t j) {
    int a = this->data.at(i), b = this->data.at(j);
    std::cout << "Comparing : " << a << " & " << b << std::endl;
    ++this->operations;
}

void DefaultObserver::onCompareValue(size_t i, int v) {
    int a = this->data.at(i);
    std::cout << "Comparing : " << a << " & " << v << std::endl;
    ++this->operations;
}

void DefaultObserver::onPop(size_t i) {
    int a = this->data.at(i);
    std::cout << "Popped " << a << " from " << i << std::endl;
    ++this->operations;
}

void DefaultObserver::onPush(size_t i, int v) {
    std::cout << "Pushed " << v << " to " << i << std::endl;
    ++this->operations;
}

void DefaultObserver::onShuffle(const std::vector<int> &) {
    std::cout << "Shuffled" << std::endl;
    ++this->operations;
}

int DefaultObserver::getOperations() const { return this->operations; }
