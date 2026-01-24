#include "array.hpp"
#include "sorter.hpp"

BubbleSort::BubbleSort(bool reversed) : reversed(reversed) {}

const char *BubbleSort::name() const { return "Bubble Sort"; }

void BubbleSort::sort(Array &data) {
    bool swapped = true;
    size_t n = data.size();
    while (swapped) {
        swapped = false;
        for (size_t i = 1; i < n; i++) {
            int comparison = data.compare(i - 1, i);
            if ((!this->reversed && comparison > 0) ||
                (this->reversed && comparison < 0)) {
                data.swap(i - 1, i);
                swapped = true;
            }
        }
        --n;
    }
}

SelectionSort::SelectionSort(bool reversed) : reversed(reversed) {}
const char *SelectionSort::name() const { return "Selection Sort"; }

void SelectionSort::sort(Array &data) {
    size_t n = data.size();
    size_t i = 0;
    while (i < n) {
        size_t min = i;
        for (size_t j = i + 1; j < n; j++) {
            int comparison = data.compare(min, j);
            if ((!this->reversed && comparison > 0) ||
                (this->reversed && comparison < 0))
                min = j;
        }
        data.swap(i, min);
        ++i;
    }
}

BogoSort::BogoSort(bool reversed) : reversed(reversed) {}
const char *BogoSort::name() const { return "Bogo Sort"; }

void BogoSort::sort(Array &data) {
    bool sorted = false;
    size_t n = data.size();
    while (!sorted) {
        sorted = true;
        for (size_t i = 0; i < n - 1; i++) {
            int cmp = data.compare(i, i + 1);
            if ((!reversed && cmp > 0) || (reversed && cmp < 0)) {
                sorted = false;
                break;
            }
        }
        if (!sorted) data.shuffle();
    }
}

CocktailSort::CocktailSort(bool reversed) : reversed(reversed) {}
const char *CocktailSort::name() const { return "Cocktail Shaker Sort"; }

void CocktailSort::sort(Array &data) {
    if (data.size() < 2) return;
    bool swapped = true;
    size_t start = 0;
    size_t end = data.size() - 1;

    while (swapped) {
        swapped = false;
        for (size_t i = start; i < end; i++) {
            int cmp = data.compare(i, i + 1);
            if ((!reversed && cmp > 0) || (reversed && cmp < 0)) {
                data.swap(i, i + 1);
                swapped = true;
            }
        }

        if (!swapped) break;

        --end;
        for (size_t i = end; i-- > start;) {
            int cmp = data.compare(i, i + 1);
            if ((!reversed && cmp > 0) || (reversed && cmp < 0)) {
                data.swap(i, i + 1);
                swapped = true;
            }
            if (i == 0) break;
        }

        ++start;
    }
}

QuickSort::QuickSort(bool reversed) : reversed(reversed) {}
const char *QuickSort::name() const { return "QuickSort"; }

size_t QuickSort::partition(Array &data, int low, int high) {
    int pivotIdx = high;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        int cmp = data.compare(j, pivotIdx);
        if ((!reversed && cmp < 0) || (reversed && cmp > 0)) {
            ++i;
            data.swap(i, j);
        }
    }
    data.swap(i + 1, high);
    return i + 1;
}

void QuickSort::quickSort(Array &data, int low, int high) {
    if (low < high) {
        size_t pi = partition(data, low, high);
        quickSort(data, low, static_cast<int>(pi) - 1);
        quickSort(data, static_cast<int>(pi) + 1, high);
    }
}

void QuickSort::sort(Array &data) {
    if (data.size() == 0 || data.size() == 1) return;
    quickSort(data, 0, static_cast<int>(data.size()) - 1);
}

HeapSort::HeapSort(bool reversed) : reversed(reversed) {}
const char *HeapSort::name() const { return "Heap Sort"; }

void HeapSort::heapify(Array &data, size_t n, size_t i) {
    size_t largest = i;
    size_t left = 2 * i + 1;
    size_t right = 2 * i + 2;

    if (left < n) {
        int cmp = data.compare(left, largest);
        if ((!reversed && cmp > 0) || (reversed && cmp < 0)) { largest = left; }
    }

    if (right < n) {
        int cmp = data.compare(right, largest);
        if ((!reversed && cmp > 0) || (reversed && cmp < 0)) {
            largest = right;
        }
    }

    if (largest != i) {
        data.swap(i, largest);
        heapify(data, n, largest);
    }
}

void HeapSort::sort(Array &data) {
    size_t n = data.size();
    if (n == 0 || n == 1) return;
    for (int i = n / 2; i-- > 0;) { heapify(data, n, i); }

    for (size_t i = n - 1; i > 0; i--) {
        data.swap(0, i);
        heapify(data, i, 0);
    }
}
