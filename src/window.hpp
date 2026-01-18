#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <variant>
#include <vector>
#include <SFML/Graphics.hpp>

#include "observer.hpp"

class Array;

class Window : public Observer {
   public:
    explicit Window(
        int width, int height, const char *title, const Array &data, const int speed);
    void onSwap(size_t i, size_t j) override;
    void onCompare(size_t i, size_t j) override;
    void onCompareValue(size_t i, int v) override;
    void onPop(size_t i) override;
    void onPush(size_t i, int v) override;
    void onShuffle(const std::vector<int> &vec) override;

    void mainloop();

   private:
    using Swap = struct {
        size_t i;
        size_t j;
    };
    using Compare = struct {
        size_t i;
        size_t j;
    };
    using CompareValue = struct {
        size_t i;
        int v;
    };
    using Pop = struct {
        size_t i;
    };
    using Push = struct {
        size_t i;
        int v;
    };
    using Shuffle = struct {
        std::vector<int> data;
    };

    using Command =
        std::variant<Swap, Compare, CompareValue, Pop, Push, Shuffle>;

    const size_t MAX_QUEUE_SIZE = 100;
    const int SPEEDLIMIT;

    void pushCommand(Command cmd);

    std::atomic<bool> exited{false};

    std::queue<Command> commands;
    std::mutex qmtx;
    std::condition_variable qcv;

    std::unique_ptr<sf::RenderWindow> window;
    std::vector<int> data;

};

#endif
