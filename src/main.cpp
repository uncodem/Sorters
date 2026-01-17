#include <iostream>
#include <thread>

#include "closer.hpp"
#include "array.hpp"
#include "sorter.hpp"
#include "window.hpp"

void runSorter(Sorter *sorter, Array &arr) {
    std::thread worker([sorter, &arr](){
        try {
            sorter->sort(arr);
        } catch (const SorterInterrupt &e) {
            return;
        } catch (const std::exception &e) {
            std::cerr << "Sorting error : " << e.what() << std::endl;
        }
    });
    worker.detach();
}

int main() {
    Array arr(1000);

    Window win(800, 600, "SFML sorter viewer", arr);
    arr.setObserver(&win);

    QuickSort sorter(true);

    runSorter(&sorter, arr);
    win.mainloop();

    return 0;
}
