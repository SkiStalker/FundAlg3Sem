#include <cstdint>
#include <iterator>
#include <vector>
#include <stdexcept>
#include <iostream>

//
// Так как у нас нет ковариантности по возвращаемому значению для НЕ
// указателей (только указатель можно привести к родительскому классу),
// то сделать методы *begin и *end у родителя и наследника нельзя
//

template <class T>
class MyContainer
{
public:
    MyContainer() = default;
    virtual ~MyContainer() = default;

    virtual std::size_t length() = 0;

    virtual void clear() = 0;

    virtual T& at(std::size_t ind) = 0;
    virtual const T& at(std::size_t ind) const = 0;

    virtual T& operator[](std::size_t ind) = 0;
    virtual const T& operator[](std::size_t ind) const = 0;

    virtual T &append(const T &value) = 0;
    virtual T remove(const T &value) = 0;
};

template <class T>
class DynamicArray : public MyContainer<T>
{
protected:
    using NoConstT = std::remove_const_t<T>;

    template <class IterType>
    class DynamicArrayIterator
    {
    protected:
        friend class DynamicArray;
        using NoConstIterType = std::remove_const_t<IterType>;
        NoConstIterType *ptr = nullptr;
        DynamicArrayIterator(NoConstIterType *ptr) : ptr(ptr) {}
        DynamicArrayIterator(const DynamicArrayIterator &other) = default;

    public:
        DynamicArrayIterator(DynamicArrayIterator &&other) = default;
        DynamicArrayIterator operator++()
        {
            this->ptr++;
            return *this;
        }

        DynamicArrayIterator operator++(int)
        {
            DynamicArrayIterator tmp = *this;
            ++this->ptr;
            return tmp;
        }

        DynamicArrayIterator operator--()
        {
            this->ptr--;
            return *this;
        }

        DynamicArrayIterator operator--(int)
        {
            DynamicArrayIterator tmp = *this;
            --this->ptr;
            return tmp;
        }

        bool operator==(const DynamicArrayIterator &other) const
        {
            return this->ptr == other.ptr;
        }
        bool operator!=(const DynamicArrayIterator &other) const
        {
            return !(*this == other);
        };
        IterType operator*()
        {
            return *this->ptr;
        }
    };

    template <class IterType>
    class DynamicArrayReverseIterator
    {
    protected:
        friend class DynamicArray;
        using NoConstIterType = std::remove_const_t<IterType>;
        NoConstIterType *ptr = nullptr;
        DynamicArrayReverseIterator(NoConstIterType *ptr) : ptr(ptr) {}
        DynamicArrayReverseIterator(const DynamicArrayReverseIterator &other) = default;

    public:
        DynamicArrayReverseIterator(DynamicArrayReverseIterator &&other) = default;
        DynamicArrayReverseIterator operator++() override
        {
            this->ptr--;
            return *this;
        }
        DynamicArrayReverseIterator operator++(int) override
        {
            DynamicArrayReverseIterator tmp = *this;
            --this->ptr;
            return tmp;
        }

        DynamicArrayReverseIterator operator--() override
        {
            this->ptr++;
            return *this;
        }

        DynamicArrayReverseIterator operator--(int) override
        {
            DynamicArrayReverseIterator tmp = *this;
            ++this->ptr;
            return tmp;
        }

        bool operator==(const DynamicArrayReverseIterator &other) const override
        {
            return this->ptr == other.ptr;
        }
        bool operator!=(const DynamicArrayReverseIterator &other) const override
        {
            return !(*this == other);
        };
        IterType operator*() override
        {
            return *this->ptr;
        }
    };

    std::size_t len = 0;
    std::size_t cap = 0;
    NoConstT *arr = nullptr;

public:
    using Iterator = DynamicArrayIterator<T>;
    using ConstIterator = DynamicArrayIterator<const T>;

    using ReverseIterator = DynamicArrayReverseIterator<T>;
    using ConstReverseIterator = DynamicArrayReverseIterator<const T>;

    DynamicArray() : len(0), cap(20), arr(new NoConstT[this->cap]) {}
    ~DynamicArray()
    {
        delete[] this->arr;
    }

    std::size_t length() override
    {
        return this->len;
    }

    void clear() override
    {
        this->len = 0;
    }

    T& at(std::size_t ind) override
    {
        if (ind < this->len)
        {
            return this->arr[ind];
        }
        else
        {
            throw std::out_of_range("ind");
        }
    }

    const T& at(std::size_t ind) const override
    {

        if (ind < this->len)
        {
            return this->arr[ind];
        }
        else
        {
            throw std::out_of_range("ind");
        }
    }

    T& operator[](std::size_t ind) override
    {
        return this->arr[ind];
    }
    const T& operator[](std::size_t ind) const override
    {
        return this->arr[ind];
    }

    T &append(const T &value) override
    {
        this->arr[this->len] = value;
        this->len++;
        if (this->len >= this->cap)
        {
            this->cap *= 2;
            NoConstT *tmp_arr = static_cast<NoConstT *>(realloc(this->arr, this->cap * sizeof(T)));
            if (!tmp_arr)
            {
                throw std::bad_alloc();
            }
            this->arr = tmp_arr;
        }
        return this->arr[this->len - 1];
    }
    T remove(const T &value) override
    {
        std::size_t ind = this->len;
        for (std::size_t i = 0; i < this->len; ++i)
        {
            if (this->arr[i] == value)
            {
                ind = i;
                break;
            }
        }

        if (ind == this->len)
        {
            throw std::out_of_range("value");
        }

        T remove_item = this->arr[ind];

        for (std::size_t i = ind; i < this->len - 1; ++i)
        {
            this->arr[i] = this->arr[i + 1];
        }

        if (this->cap > 10 && this->len < (this->cap / 2))
        {
            this->cap /= 2;
            NoConstT *tmp_arr = static_cast<NoConstT *>(realloc(this->arr, this->cap * sizeof(T)));
            if (!tmp_arr)
            {
                throw std::bad_alloc();
            }
            this->arr = tmp_arr;
        }

        return remove_item;
    }

    Iterator begin()
    {
        return Iterator(this->arr);
    }
    Iterator end()
    {
        return Iterator(this->arr + this->len);
    }

    ConstIterator cbegin()
    {
        return ConstIterator(this->arr);
    }
    ConstIterator cend()
    {
        return ConstIterator(this->arr + this->len);
    }

    ReverseIterator rbegin()
    {
        return ReverseIterator(this->arr + this->len - 1);
    }
    ReverseIterator rend()
    {
        return ReverseIterator(this->arr - 1);
    }

    ConstReverseIterator crbegin()
    {
        return ConstReverseIterator(this->arr + this->len - 1);
    }
    ConstReverseIterator crend()
    {
        return ConstReverseIterator(this->arr - 1);
    }
};

int main()
{
    DynamicArray<const int> d;

    d.append(1);
    d.append(1);
    d.append(1);
    d.append(1);
    d.append(1);
    d.append(1);

    for (DynamicArray<const int>::ConstIterator iter = d.begin(); iter != d.end(); ++iter) {
        std::cout << *iter << std::endl;
    }

    return 0;
}