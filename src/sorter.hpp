#ifndef SORTER_HPP_
#define SORTER_HPP_

#include <cstddef>

class Array;

class Sorter {
   public:
    virtual ~Sorter() = default;
    virtual void sort(Array &) = 0;
    virtual const char *name() const = 0;
};

class BubbleSort : public Sorter {
   public:
    explicit BubbleSort(bool reversed = false);
    const char *name() const override;
    void sort(Array &data) override;

   private:
    bool reversed;
};

class SelectionSort : public Sorter {
   public:
    explicit SelectionSort(bool reversed = false);
    const char *name() const override;
    void sort(Array &data) override;

   private:
    bool reversed;
};

class BogoSort : public Sorter {
public:
    explicit BogoSort(bool reversed = false);
    const char *name() const override;
    void sort(Array &data) override;
private:
    bool reversed;
};

class CocktailSort : public Sorter {
public:
    explicit CocktailSort(bool reversed = false);
    const char *name() const override;
    void sort(Array &data) override;
private:
    bool reversed;
};

class QuickSort : public Sorter {
public:
    QuickSort(bool reversed = false);
    const char *name() const override;
    void sort(Array &data) override;

private:
    bool reversed;
    size_t partition(Array &data, int low, int high);
    void quickSort(Array &data, int low, int high);
};

class HeapSort : public Sorter {
public:
    HeapSort(bool reversed = false);
    const char *name() const override;
    void sort(Array &data) override;

private:
    bool reversed;
    void heapify(Array &data, size_t n, size_t i);
};

#endif
