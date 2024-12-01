#include <iostream>
#include <cmath>
#include <vector>
#include <initializer_list>
#include <algorithm>

using namespace std;

class CustomVector final
{
private:
    double* dataArray;
    size_t currentSize;
    size_t capacity;

    void resizeArray(size_t newCapacity)
    {
        double* newArray = new double[newCapacity];
        size_t elementsToCopy = min(currentSize, newCapacity);
        copy(dataArray, dataArray + elementsToCopy, newArray);
        delete[] dataArray;
        dataArray = newArray;
        capacity = newCapacity;
        currentSize = elementsToCopy;
    }

public:
    class Iterator final
    {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = double;
        using difference_type = std::ptrdiff_t;
        using pointer = double*;
        using reference = double&;

    private:
        double* _ptr;

    public:
        Iterator(double* ptr) : _ptr(ptr) {}

        reference operator*() { return *_ptr; }
        const value_type& operator*() const { return *_ptr; }

        Iterator& operator++()
        {
            ++_ptr;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator temp = *this;
            ++_ptr;
            return temp;
        }

        Iterator& operator--()
        {
            --_ptr;
            return *this;
        }

        Iterator operator--(int)
        {
            Iterator temp = *this;
            --_ptr;
            return temp;
        }

        Iterator operator+(difference_type shift) const { return Iterator(_ptr + shift); }
        Iterator& operator+=(difference_type shift)
        {
            _ptr += shift;
            return *this;
        }

        Iterator operator-(difference_type shift) const { return Iterator(_ptr - shift); }
        Iterator& operator-=(difference_type shift)
        {
            _ptr -= shift;
            return *this;
        }

        difference_type operator-(const Iterator& other) const { return _ptr - other._ptr; }

        bool operator==(const Iterator& other) const { return _ptr == other._ptr; }
        bool operator!=(const Iterator& other) const { return _ptr != other._ptr; }
        bool operator<(const Iterator& other) const { return _ptr < other._ptr; }
        bool operator>(const Iterator& other) const { return _ptr > other._ptr; }
        bool operator<=(const Iterator& other) const { return _ptr <= other._ptr; }
        bool operator>=(const Iterator& other) const { return _ptr >= other._ptr; }
    };


    CustomVector(size_t elementCount, double defaultValue) : dataArray{ new double[elementCount] },
        currentSize(elementCount),
        capacity(elementCount)
    {
        fill_n(dataArray, elementCount, defaultValue);
    }

    CustomVector(size_t elementCount) : CustomVector(elementCount, 0.0) {}

    template <typename IteratorType>
    CustomVector(IteratorType start, IteratorType end)
    {
        size_t elementCount = end - start;
        dataArray = new double[elementCount];
        capacity = currentSize = elementCount;
        copy(start, end, dataArray);
    }

   CustomVector(initializer_list<double> list) : 
                                               dataArray{ new double[list.size()] },
                                               currentSize(list.size()),
                                               capacity(list.size())
{
    copy(list.begin(), list.end(), dataArray);
}

    CustomVector(const CustomVector& other) : CustomVector(other.begin(), other.end()) {}

    double& getAt(size_t index)
    {
        if (index >= currentSize)
            throw out_of_range("Index out of bounds");
        return dataArray[index];
    }

    double const& getAt(size_t index) const
    {
        if (index >= currentSize)
            throw out_of_range("Index out of bounds");
        return dataArray[index];
    }

    double& getFirst()
    {
        if (currentSize == 0)
            throw out_of_range("Vector is empty");
        return dataArray[0];
    }

    double const& getFirst() const
    {
        if (currentSize == 0)
            throw out_of_range("Vector is empty");
        return dataArray[0];
    }

    double& getLast()
    {
        if (currentSize == 0)
            throw out_of_range("Vector is empty");
        return dataArray[currentSize - 1];
    }

    double const& getLast() const
    {
        if (currentSize == 0)
            throw out_of_range("Vector is empty");
        return dataArray[currentSize - 1];
    }

    double* getRawData() { return dataArray; }

    double const* getRawData() const { return dataArray; }

    bool isEmpty() const { return currentSize == 0; }

    void allocateSpace(size_t newCapacity)
    {
        if (newCapacity > capacity)
            resizeArray(newCapacity);
    }

    size_t getCapacity() const { return capacity; }

    size_t getSize() const { return currentSize; }

    void compactToFit()
    {
        if (capacity > currentSize)
            resizeArray(currentSize);
    }

    void clear() { currentSize = 0; }

    void append(double value)
    {
        if (currentSize == capacity)
            resizeArray(capacity * 2);
        dataArray[currentSize++] = value;
    }

    void removeLast()
    {
        if (currentSize > 0)
            --currentSize;
    }

    void adjustSize(size_t newSize, double fillValue)
    {
        size_t oldSize = currentSize;
        resizeArray(newSize);
        if (newSize > oldSize)
            fill(dataArray + oldSize, dataArray + newSize, fillValue);
        currentSize = newSize;
    }

    void removeAt(size_t index)
    {
        if (index >= currentSize)
            return;

        for (size_t i = index; i < currentSize - 1; ++i)
        {
            dataArray[i] = dataArray[i + 1];
        }
        --currentSize;
    }

    void insertAt(size_t index, double value)
    {
        if (index > currentSize)
            throw out_of_range("Index out of bounds");

        if (currentSize == capacity)
            resizeArray(capacity * 2);

        ++currentSize;
        for (size_t i = currentSize - 1; i > index; --i)
        {
            dataArray[i] = dataArray[i - 1];
        }
        dataArray[index] = value;
    }

    bool operator==(const CustomVector& other) const
    {
        if (currentSize != other.currentSize)
            return false;
        return equal(dataArray, dataArray + currentSize, other.dataArray);
    }

    int operator<=>(const CustomVector& other) const
    {
        for (size_t i = 0; i < currentSize && i < other.currentSize; ++i)
        {
            if (dataArray[i] < other.dataArray[i]) return -1;
            if (dataArray[i] > other.dataArray[i]) return 1;
        }
        if (currentSize < other.currentSize) return -1;
        if (currentSize > other.currentSize) return 1;
        return 0;
    }

    CustomVector& operator=(const CustomVector& other)
    {
        if (this != &other)
        {
            CustomVector temp(other.begin(), other.end());
            swap(temp.dataArray, dataArray);
            swap(temp.capacity, capacity);
            swap(temp.currentSize, currentSize);
        }
        return *this;
    }

    Iterator begin() { return Iterator(dataArray); }
    Iterator end() { return Iterator(dataArray + currentSize); }

    const Iterator begin() const { return Iterator(dataArray); }
    const Iterator end() const { return Iterator(dataArray + currentSize); }

    ~CustomVector() { delete[] dataArray; }
};

int main()
{
    CustomVector vec1{ 5, 10.0 }; 
    vec1.append(20.0);     

    vec1.insertAt(2, 15.0);  
    vec1.removeAt(3);          

    cout << "Vector 1 elements after operations:" << endl;
    for (auto val : vec1)
        cout << val << " ";
    cout << endl;

    CustomVector vec2{ 1.1, 2.2, 3.3, 4.4 };
    cout << "Vector 2 size: " << vec2.getSize() << endl;

    vec2.removeLast(); 
    vec2.append(5.5);  
    vec2.adjustSize(6, 0.0); 

    cout << "Vector 2 elements:" << endl;
    for (auto it = vec2.begin(); it != vec2.end(); ++it)
        cout << *it << " ";
    cout << endl;

    cout << "Vector 2 first element: " << vec2.getFirst() << endl;
    cout << "Vector 2 last element: " << vec2.getLast() << endl;

    vec1.clear();
    cout << "Is Vector 1 empty after clear? " << (vec1.isEmpty() ? "Yes" : "No") << endl;

    return 0;
}
