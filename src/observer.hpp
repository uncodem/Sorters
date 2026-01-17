#ifndef OBSERVER_HPP_
#define OBSERVER_HPP_

#include <cstddef>
#include <vector>

class Observer {
   public:
    virtual ~Observer() = default;
    virtual void onSwap(size_t, size_t) = 0;
    virtual void onCompare(size_t, size_t) = 0;
    virtual void onCompareValue(size_t, int) = 0;
    virtual void onPop(size_t) = 0;
    virtual void onPush(size_t, int) = 0;
    virtual void onShuffle(const std::vector<int> &) = 0;
};

#endif
