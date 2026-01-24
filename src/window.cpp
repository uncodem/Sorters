#include "window.hpp"

#include <SFML/Graphics.hpp>
#include <set>

#include "array.hpp"
#include "closer.hpp"

Window::Window(
    int width, int height, const char *title, const Array &data,
    const int speed)
    : SPEEDLIMIT(speed),
      window(new sf::RenderWindow(sf::VideoMode(width, height), title)),
      data(data.getCopy()) {
    window->setVerticalSyncEnabled(true);
    window->setFramerateLimit(60);
}

void Window::pushCommand(Window::Command cmd) {
    std::unique_lock<std::mutex> lock(this->qmtx);
    qcv.wait(
        lock, [this] { return commands.size() < MAX_QUEUE_SIZE || exited; });

    if (exited) { throw SorterInterrupt(); }

    commands.push(std::move(cmd));
}

void Window::onSwap(size_t i, size_t j) { pushCommand(Swap{i, j}); }

void Window::onCompare(size_t i, size_t j) { pushCommand(Compare{i, j}); }

void Window::onCompareValue(size_t i, int v) {
    pushCommand(CompareValue{i, v});
}

void Window::onPop(size_t i) { pushCommand(Pop{i}); }

void Window::onPush(size_t i, int v) { pushCommand(Push{i, v}); }

void Window::onShuffle(const std::vector<int> &vec) {
    pushCommand(Shuffle{vec});
}

void Window::mainloop() {
    float bwidth = static_cast<float>(window->getSize().x) / data.size();

    std::set<size_t> actives;
    std::set<size_t> compares;

    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                exited = true;
                qcv.notify_all();
                window->close();
            } else if (event.type == sf::Event::Resized) {
                sf::FloatRect visible(
                    0, 0, event.size.width, event.size.height);
                window->setView(sf::View(visible));
                if (!data.empty()) {
                    bwidth = static_cast<float>(event.size.width) / data.size();
                }
            }
        }
        actives.clear();
        compares.clear();

        int processed = 0;

        {
            std::unique_lock<std::mutex> lock(qmtx);
            while (!commands.empty() && processed < SPEEDLIMIT) {
                Command cmd = std::move(commands.front());
                commands.pop();
                std::visit(
                    [this, &actives, &compares](auto &&arg) {
                        using T = std::decay_t<decltype(arg)>;
                        if constexpr (std::is_same_v<T, Swap>) {
                            std::swap(this->data[arg.i], this->data[arg.j]);
                            actives.insert(arg.i);
                            actives.insert(arg.j);
                        } else if constexpr (std::is_same_v<T, Compare>) {
                            compares.insert(arg.i);
                            compares.insert(arg.j);
                        } else if constexpr (std::is_same_v<T, CompareValue>) {
                            compares.insert(arg.i);
                        } else if constexpr (std::is_same_v<T, Pop>) {
                            this->data.erase(this->data.begin() + arg.i);
                            actives.insert(arg.i);
                        } else if constexpr (std::is_same_v<T, Push>) {
                            this->data.insert(
                                this->data.begin() + arg.i, arg.v);
                            actives.insert(arg.i);
                        } else if constexpr (std::is_same_v<T, Shuffle>) {
                            this->data = arg.data;
                        }
                    },
                    cmd);
                ++processed;
            }
            qcv.notify_all();
        }

        window->clear(sf::Color::Black);

        if (!data.empty()) {
            bwidth = static_cast<float>(window->getSize().x) / data.size();
        }

        float maxHeight = static_cast<float>(window->getSize().y);
        int maxVal =
            data.empty() ? 1 : *std::max_element(data.begin(), data.end());
        if (maxVal == 0) maxVal = 1;

        for (size_t i = 0; i < data.size(); ++i) {
            float h = (static_cast<float>(data[i]) / maxVal) * maxHeight;

            sf::RectangleShape bar(sf::Vector2f(bwidth - 1.0f, h));
            bar.setPosition(i * bwidth, maxHeight - h);

            if (actives.count(i)) {
                bar.setFillColor(sf::Color::Red);
            } else if (compares.count(i)) {
                bar.setFillColor(sf::Color::Green);
            } else {
                bar.setFillColor(sf::Color::White);
            }

            window->draw(bar);
        }

        window->display();
    }
}
