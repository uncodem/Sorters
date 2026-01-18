#include <iostream>
#include <thread>
#include <string>
#include <memory>
#include <cctype>
#include <algorithm>

#include "closer.hpp"
#include "array.hpp"
#include "sorter.hpp"
#include "window.hpp"

#define ARGV_SHIFT(argc_ptr, argv_ptr) ( \
    --(*(argc_ptr)),                     \
    *(*(argv_ptr))++                     \
)

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

std::unique_ptr<Sorter> makeSorter(std::string sorter, bool is_reversed) {
    Sorter *ret = nullptr;
    if (sorter == "QuickSort") ret = new QuickSort(is_reversed);
    else if (sorter == "Bubble") ret = new BubbleSort(is_reversed);
    else if (sorter == "Cocktail") ret = new CocktailSort(is_reversed);
    else if (sorter == "Bogo") ret = new BogoSort(is_reversed);
    else if (sorter == "Selection") ret = new SelectionSort(is_reversed);
    else if (sorter == "Heap") ret = new HeapSort(is_reversed);
    return std::unique_ptr<Sorter>(ret);
}

int parseNumber(const char *arg) {
    try {
        int ret = std::stoi(arg);
        return ret;
    } catch (const std::exception &e) {
        std::cerr << "Error : " << e.what() << std::endl;
        return -1;
    }
}

void print_usage(const char *program_name) {
    std::cout << "Usage : " << program_name << " [sorting algorithm] [no. of elements] [actions per frame] [reversed = t/f]" << std::endl;
    const char* sorters[] = {
        "Bubble",
        "Cocktail",
        "Selection",
        "Bogo",
        "Heap",
        "QuickSort",
    };
    const size_t sorter_count = 6;

    std::cout << "Available sorting algorithms : " << std::endl;
    for (size_t i = 0; i < sorter_count; i++)
        std::cout << sorters[i] << std::endl;
}

int main(int argc, char **argv) {
    if (argc != 5) {
        print_usage(argv[0]);
        return 1;
    }

    const char* program_name = ARGV_SHIFT(&argc, &argv);

    int n = 0;

    std::string sorter_name = ARGV_SHIFT(&argc, &argv);

    if ((n = parseNumber(ARGV_SHIFT(&argc, &argv))) == -1) {
        print_usage(program_name);
        return 1;
    }

    size_t elements = n;

    if ((n = parseNumber(ARGV_SHIFT(&argc, &argv))) == -1) {
        print_usage(program_name);
        return 1;
    }
    int speed = n;

    std::string reversed = ARGV_SHIFT(&argc, &argv);
    std::transform(reversed.begin(), reversed.end(), reversed.begin(), [](unsigned char c) {return std::tolower(c);});

    if (reversed != "t" && reversed != "f") {
        print_usage(program_name);
        return 1;
    }

    Array arr(elements);

    Window win(800, 600, "SFML sorter viewer", arr, speed);
    arr.setObserver(&win);

    std::unique_ptr<Sorter> sorter = makeSorter(sorter_name, reversed=="t");
    if (sorter == nullptr) {
        print_usage(program_name);
        return 1;
    }

    runSorter(sorter.get(), arr);
    win.mainloop();

    return 0;
}
