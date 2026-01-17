#ifndef CLOSER_HPP_
#define CLOSER_HPP_

#include <exception>

class SorterInterrupt : public std::exception {
   public:
    const char *what() const noexcept override {
        return "Sorter stopped by user.";
    }
};

#endif
