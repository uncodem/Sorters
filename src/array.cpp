#include "array.hpp"

#include <algorithm>
#include <iostream>
#include <random>

#include "observer.hpp"

Array::Array(size_t n, Observer *obs) : data(n), observer(obs) {
    for (size_t i = 0; i < n; i++) { this->data[i] = i; }
    this->shuffle();
}

std::vector<int> Array::getCopy() const {
    std::lock_guard<std::mutex> lock(this->mtx);
    std::vector<int> ret(this->data);
    return ret;
}

void Array::shuffle() {
    Observer *obs;
    std::vector<int> ret;
    {
        std::lock_guard<std::mutex> lock(this->mtx);
        auto rd = std::random_device{};
        auto rng = std::default_random_engine{rd()};
        std::shuffle(this->data.begin(), this->data.end(), rng);
        obs = this->observer;
        ret = std::vector<int>(this->data);
    }
    if (obs != nullptr) obs->onShuffle(ret);
}

void Array::swap(size_t i, size_t j) {
    Observer *obs;
    {
        std::lock_guard<std::mutex> lock(this->mtx);
        std::swap(this->data[i], this->data[j]);
        obs = this->observer;
    }
    if (obs != nullptr) obs->onSwap(i, j);
}

void Array::setObserver(Observer *obs) {
    std::lock_guard<std::mutex> lock(this->mtx);
    this->observer = obs;
}

int Array::compare(size_t i, size_t j) const {
    Observer *obs;
    int a, b;
    {
        std::lock_guard<std::mutex> lock(this->mtx);
        a = this->data[i];
        b = this->data[j];
        obs = this->observer;
    }

    if (obs != nullptr) obs->onCompare(i, j);

    if (a == b) return 0;
    else if (a > b)
        return 1;
    else
        return -1;
}

int Array::compareValue(size_t i, int v) const {
    Observer *obs;
    int a;
    {
        std::lock_guard<std::mutex> lock(this->mtx);
        a = this->data[i];
        obs = this->observer;
    }

    if (obs != nullptr) obs->onCompareValue(i, v);

    if (a == v) return 0;
    else if (a > v)
        return 1;
    else
        return -1;
}

void Array::insert(size_t i, int v) {
    Observer *obs;
    {
        std::lock_guard<std::mutex> lock(this->mtx);
        this->data.insert(this->data.begin() + i, v);
        obs = this->observer;
    }
    if (obs != nullptr) obs->onPush(i, v);
}

void Array::insert(int v) {
    Observer *obs;
    size_t idx;
    {
        std::lock_guard<std::mutex> lock(this->mtx);
        idx = this->data.size();
        this->data.push_back(v);
        obs = this->observer;
    }
    if (obs != nullptr) obs->onPush(idx, v);
}

int Array::pop() {
    Observer *obs;
    int ret;
    size_t idx;
    {
        std::lock_guard<std::mutex> lock(this->mtx);
        idx = this->data.size() - 1;
        ret = this->data.back();
        this->data.pop_back();
        obs = this->observer;
    }
    if (obs != nullptr) obs->onPop(idx);
    return ret;
}

int Array::pop(size_t i) {
    Observer *obs;
    int ret;
    {
        std::lock_guard<std::mutex> lock(this->mtx);
        ret = this->data[i];
        obs = this->observer;
        this->data.erase(this->data.begin() + i);
    }
    if (obs != nullptr) obs->onPop(i);
    return ret;
}

int Array::at(size_t i) const {
    std::lock_guard<std::mutex> lock(this->mtx);
    return this->data[i];
}

size_t Array::size() const {
    std::lock_guard<std::mutex> lock(this->mtx);
    return this->data.size();
}

void Array::dump() const {
    std::lock_guard<std::mutex> lock(this->mtx);
    std::cout << '[';
    for (int i : this->data) { std::cout << ' ' << i; }
    std::cout << " ]" << std::endl;
}
