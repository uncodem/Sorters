#ifndef DEFAULT_OBSERVER_HPP_
#define DEFAULT_OBSERVER_HPP_

#include "observer.hpp"

class Array;

class DefaultObserver : public Observer {
   public:
    explicit DefaultObserver(const Array &arr);
    void onSwap(size_t i, size_t j) override;
    void onCompare(size_t i, size_t j) override;
    void onCompareValue(size_t i, int v) override;
    void onPop(size_t i) override;
    void onPush(size_t i, int v) override;
    void onShuffle(const std::vector<int> &) override;
    int getOperations() const;

   private:
    long long operations;
    const Array &data;
};

#endif
