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

int parseNumber(char ***arg) {
    std::string num = **arg++;
    try {
        int ret = std::stoi(num);
        return ret;
    } catch (const std::exception &e) {
        std::cerr << "Error : " << e.what() << std::endl;
        return -1;
    }
}

std::string parseString(char ***arg) {
    return std::string(**arg++);
}

int main(int argc, char **argv) {
    if (argc != 5) {
    print_usage:
        std::cout << "Usage : " << argv[0] << " [sorting algorithm] [no. of elements] [actions per frame] [reversed = t/f]" << std::endl;
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

        return 1;
    }

    int n = 0;

    std::string sorter_name = parseString(&argv);

    if ((n = parseNumber(&argv)) == -1) goto print_usage;
    size_t elements = n;

    if ((n = parseNumber(&argv)) == -1) goto print_usage;
    int speed = n;

    std::string reversed = parseString(&argv);
    std::transform(reversed.begin(), reversed.end(), reversed.begin(), [](unsigned char c) {return std::tolower(c);});

    if (reversed != "t" || reversed != "f") goto print_usage;

    Array arr(elements);

    Window win(800, 600, "SFML sorter viewer", arr, speed);
    arr.setObserver(&win);

    std::unique_ptr<Sorter> sorter = makeSorter(sorter_name, reversed=="t");
    if (sorter == nullptr) goto print_usage;

    runSorter(sorter.get(), arr);
    win.mainloop();

    return 0;
}
