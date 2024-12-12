#include <iostream>
#include <cfloat>
#include <cmath>
using namespace std;

class vector {
private:
    size_t cur_size, max_size;
    double* arr;
public:
    vector() {
        cur_size = 0, max_size = 0, arr = NULL;
    }
    vector(size_t cnt, double val) {
        cur_size = cnt, max_size = cnt;
        if (!cnt)
            arr = NULL;
        else {
            arr = new double[max_size];
            if (!arr)
                throw("Memory allocation error");
            for (size_t i = 0; i < cnt; i++)
                arr[i] = val;
        }
    }
    vector(size_t cnt) : vector(cnt, 0.0) {}
    void info() const {
        cout << "Current size = " << cur_size << ", max size = " << max_size << endl;
        for (int i = 0; i < cur_size; i++)
            cout << arr[i] << ' ';
        cout << endl;
    }
    bool empty() const {
        return !cur_size;
    }
    double& at(size_t index) const {
        if (!cur_size) {
            throw("Vector is empty");
        }
        if (index >= cur_size)
            throw("Index out of range");
        return arr[index];
    }
    double& front() const {
        return at(0);
    }
    double& back() const {
        return at(cur_size - 1);
    }
    double* data() const {
        return arr;
    }
    size_t size() const {
        return cur_size;
    }
    size_t capacity() const {
        return max_size;
    }
    void reserve(size_t new_size) {
        if (new_size > max_size) {
            double* new_arr = new double[new_size];
            if (!new_arr) {
                throw("Memory allocation error");
            }
            max_size = new_size;
            for (int i = 0; i < cur_size; i++)
                new_arr[i] = arr[i];
            delete[] arr;
            arr = new_arr;
        }
    }
    void clear() {
        delete[] arr;
        arr = NULL;
        cur_size = 0;
        max_size = 0;
    }
    void shrink_to_fit() {
        if (max_size > cur_size) {
            double* new_arr = new double[cur_size];
            if (!new_arr) {
                throw("Memory allocation error");
            }
            max_size = cur_size;
            for (int i = 0; i < cur_size; i++)
                new_arr[i] = arr[i];
            delete[] arr;
            arr = new_arr;
        }
    }
    void push_back(double elem) {
        if (cur_size == max_size) {
            try {
                reserve(1 + 2 * max_size);
            }
            catch(const char* e){
                throw(e);
            }
        }
        arr[cur_size] = elem;
        cur_size++;
    }
    void pop_back() {
        if (empty())
            throw("Vector is empty");
        cur_size--;
    }
    void resize(size_t new_size, double elem) {
        if (new_size < cur_size)
            cur_size = new_size;
        else {
            try {
                reserve(new_size);
            }
            catch (const char* e) {
                throw(e);
            }
            while (cur_size < max_size) {
                arr[cur_size] = elem;
                cur_size++;
            }
        }
    }
    void insert(size_t index, double elem) {
        if (index > cur_size)
            throw("Index out of range");
        if (++cur_size > max_size) {
            max_size = cur_size;
            double* new_arr = new double[max_size];
            if (!new_arr)
                throw("Memory alloation error");
            for (size_t i = 0; i < index; i++)
                new_arr[i] = arr[i];
            new_arr[index] = elem;
            for (size_t i = index; i < cur_size; i++)
                new_arr[i + 1] = arr[i];
            delete[] arr;
            arr = new_arr;
        }
        else {
            for (size_t i = cur_size - 1; i > index; i--)
                arr[i] = arr[i - 1];
            arr[index] = elem;
        }
    }
    void erase(size_t index) {
        if (!cur_size)
            throw("Vector is empty");
        if (index > cur_size)
            throw("Index out of range");
        for (size_t i = index; i < cur_size - 1; i++)
            arr[i] = arr[i + 1];
        cur_size--;
    }
    bool operator ==(const vector& v) const {
        if (cur_size != v.cur_size || max_size != v.max_size)
            return false;
        for (int i = 0; i < cur_size; i++) {
            if (arr[i] != v.arr[i])
                return false;
        }
        return true;
    }
    auto operator <=>(const vector& v) const {
        if (v == *this)
            return strong_ordering::equivalent;
        if (cur_size < v.cur_size)
            return strong_ordering::less;
        if (cur_size > v.cur_size)
            return strong_ordering::greater;
        for (size_t i = 0; i < cur_size; i++) {
            if (arr[i] < v.arr[i])
                return strong_ordering::less;
            if (arr[i] > v.arr[i])
                return strong_ordering::greater;
        }
        return strong_ordering::equivalent;
    }
    class iterator {
    private:
        double* ptr;
    public:
        iterator(double* ptr) : ptr(ptr) {}
        double* get() const {
            return ptr;
        }
        double& operator*() const{
            return *ptr;
        }
        iterator operator+(size_t x) const {
            return iterator(ptr + x);
        }
        iterator operator-(size_t x) const {
            return iterator(ptr - x);
        }
        bool operator==(const iterator& i) const {
            return ptr == i.ptr;
        }
        bool operator!=(const iterator& i) const {
            return ptr != i.ptr;
        }
    };
    iterator begin() {
        return iterator(arr);
    }
    iterator end() {
        return iterator(arr + cur_size);
    }
    vector(iterator first, iterator last) {
        cur_size = last.get() - first.get();
        max_size = cur_size;
        arr = new double[cur_size];
        if (!arr)
            throw("Memory allocation error");
        for (size_t i = 0; i < cur_size; i++)
            arr[i] = *(first + i);
    }
    vector(initializer_list<double> lst){
        cur_size = lst.end() - lst.begin();
        max_size = cur_size;
        arr = new double[cur_size];
        if (!arr)
            throw("Memory allocation error");
        for (size_t i = 0; i < cur_size; i++)
            arr[i] = *(lst.begin() + i);
    }
};



int main() {

    vector v1(3), v2(2, 0.7);
    auto res = v1 <=> v2;
    if (res > 0)
        cout << "Greater" << endl;
    v1.clear();
    v2.clear();

    /*initializer_list<double> lst{1, 2, 3, 4};
    vector v(lst);
    v.info();
    v.clear();*/

    /*vector v1(3, 1.4);
    v1.info();
    vector v2(v1.begin(), v1.end());
    v2.info();
    v2.clear();
    v1.clear();*/

    /*vector v(2, 3.5);
    v.push_back(5);
    v.push_back(8.8);
    v.info();
    v.pop_back();
    v.info();
    v.clear();*/

    /*vector v(3);
    v.insert(1.5, 1);
    v.info();
    v.erase(1);
    v.info();
    v.clear();*/

    return 0;
}
