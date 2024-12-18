#include <iostream>
#include <stdexcept>
#include <initializer_list>
#include <compare>
#include <iterator>

class Vector
{
private:
    double *data_;
    size_t size_;
    size_t capacity_;

    void check_index(size_t index) const
    {
        if (index >= size_)
        {
            throw std::out_of_range("Trying to get access to outer limit of vector");
        }
    }

    void ensure_capacity(size_t min_capacity)
    {
        if (capacity_ < min_capacity)
        {
            size_t new_capacity = std::max(min_capacity, capacity_ * 2);
            reserve(new_capacity);
        }
    }

public:
    Vector() : data_(nullptr), size_(0), capacity_(0) {}

    Vector(size_t count, double value) : size_(count), capacity_(count)
    {
        data_ = new double[capacity_];
        std::fill(data_, data_ + size_, value);
    }

    Vector(size_t count) : Vector(count, 0.0) {}

    template <typename Iterator>
    Vector(Iterator first, Iterator last)
    {
        size_ = std::distance(first, last);
        capacity_ = size_;
        data_ = new double[capacity_];
        std::copy(first, last, data_);
    }

    Vector(std::initializer_list<double> init) : Vector(init.begin(), init.end()) {}

    ~Vector()
    {
        delete[] data_;
    }

    double &at(size_t index)
    {
        check_index(index);
        return data_[index];
    }

    const double &at(size_t index) const
    {
        check_index(index);
        return data_[index];
    }

    double &front()
    {
        return at(0);
    }

    const double &front() const
    {
        return at(0);
    }

    double &back()
    {
        return at(size_ - 1);
    }

    const double &back() const
    {
        return at(size_ - 1);
    }

    double *data()
    {
        return data_;
    }

    const double *data() const
    {
        return data_;
    }

    bool empty() const
    {
        return size_ == 0;
    }

    size_t size() const
    {
        return size_;
    }

    void reserve(size_t num)
    {
        if (num > capacity_)
        {
            double *new_data = new double[num];
            std::copy(data_, data_ + size_, new_data);
            delete[] data_;
            data_ = new_data;
            capacity_ = num;
        }
    }

    size_t capacity() const
    {
        return capacity_;
    }

    void shrink_to_fit()
    {
        if (capacity_ > size_)
        {
            double *new_data = new double[size_];
            std::copy(data_, data_ + size_, new_data);
            delete[] data_;
            data_ = new_data;
            capacity_ = size_;
        }
    }

    void clear()
    {
        size_ = 0;
    }

    void insert(size_t index, double elem)
    {
        if (index > size_)
        {
            throw std::out_of_range("Index out of bounds");
        }
        ensure_capacity(size_ + 1);
        std::move_backward(data_ + index, data_ + size_, data_ + size_ + 1);
        data_[index] = elem;
        ++size_;
    }

    void erase(size_t index)
    {
        check_index(index);
        std::move(data_ + index + 1, data_ + size_, data_ + index);
        --size_;
    }

    void push_back(double elem)
    {
        ensure_capacity(size_ + 1);
        data_[size_++] = elem;
    }

    void pop_back()
    {
        if (empty())
        {
            throw std::underflow_error("Vector is empty");
        }
        --size_;
    }

    void resize(size_t new_size, double elem = 0.0)
    {
        if (new_size > size_)
        {
            ensure_capacity(new_size);
            std::fill(data_ + size_, data_ + new_size, elem);
        }
        size_ = new_size;
    }

    auto operator<=>(const Vector &other) const
    {
        return std::lexicographical_compare_three_way(data_, data_ + size_, other.data_, other.data_ + other.size_);
    }

    bool operator==(const Vector &other) const
    {
        return size_ == other.size_ && std::equal(data_, data_ + size_, other.data_);
    }

    class Iterator
    {
    private:
        double *ptr_;

    public:
        Iterator(double *ptr) : ptr_(ptr) {}

        double &operator*()
        {
            return *ptr_;
        }

        Iterator &operator++()
        {
            ++ptr_;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator temp = *this;
            ++ptr_;
            return temp;
        }

        Iterator &operator--()
        {
            --ptr_;
            return *this;
        }

        Iterator operator--(int)
        {
            Iterator temp = *this;
            --ptr_;
            return temp;
        }

        Iterator operator+(size_t offset) const
        {
            return Iterator(ptr_ + offset);
        }

        Iterator operator-(size_t offset) const
        {
            return Iterator(ptr_ - offset);
        }

        bool operator==(const Iterator &other) const
        {
            return ptr_ == other.ptr_;
        }

        bool operator!=(const Iterator &other) const
        {
            return ptr_ != other.ptr_;
        }
    };

    Iterator begin()
    {
        return Iterator(data_);
    }

    Iterator end()
    {
        return Iterator(data_ + size_);
    }
};

int main()
{
    try
    {
        Vector v = {1.0, 2.0, 3.0};

        std::cout << "Examples of using at():" << std::endl;
        // std::cout << v.at(-1) << std::endl;
        std::cout << v.at(0) << std::endl;
        std::cout << v.at(1) << std::endl;
        std::cout << v.at(2) << std::endl;
        // std::cout << v.at(3) << std::endl << std::endl;

        std::cout << "Example of using front(): " << v.front() << std::endl;

        Vector a = {};
        // std::cout << a.front() << std::endl;

        std::cout << "Example of using back(): " << v.back() << std::endl;

        std::cout << "Example of using data(): " << v.data() << std::endl;

        std::cout << "Example of using empty(): " << v.empty() << " " << a.empty() << std::endl;

        std::cout << "Example of using size(): " << v.size() << " " << a.size() << std::endl;

        std::cout << "Example of using capacity() before reserve(10): " << v.capacity() << std::endl;
        v.reserve(10);
        std::cout << "Example of using capacity() after reserve(10): " << v.capacity() << std::endl;
        v.reserve(3);
        std::cout << "Example of using capacity() after reserve(3): " << v.capacity() << std::endl;
        v.shrink_to_fit();
        std::cout << "Example of using capacity() after shrink_to_fit(): " << v.capacity() << std::endl;

        v.clear();
        std::cout << "Example of using size() after using v.clear(): " << v.size() << " " << a.size() << std::endl;

        v.push_back(4.0);
        v.insert(1, 5.0);
        v.insert(0, 1.0);
        v.insert(2, 2.0);
        v.insert(3, 3.0);
        v.erase(2);
        v.pop_back();
        v.resize(10);
        v.resize(11, 123);

        for (const auto &elem : v)
        {
            std::cout << elem << " ";
        }
        std::cout << std::endl
                  << std::endl;

        std::cout << "Example of comparing for equality: " << (v == a) << std::endl;
        std::cout << "Example of comparing for non-equality: " << (v != a) << std::endl;
        std::cout << "Example of comparing for >: " << (v > a) << std::endl;
        std::cout << "Example of comparing for <=: " << (v <= a) << std::endl;

        return 0;
    }

    catch (const std::out_of_range &e)
    {
        std::cout << "Trying to get access to outer limit of vector" << std::endl;
    }
}