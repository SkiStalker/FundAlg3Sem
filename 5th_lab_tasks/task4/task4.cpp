#include <iostream>
#include <cmath>

class complex final
{
private:
    double _real, _im;

public:
    complex(double real = 0, double im = 0) : _real(real), _im(im) {};

    double module() const { return sqrt(_real * _real + _im * _im); }

    void add(const complex &value)
    {
        _real += value._real;
        _im += value._im;
    }

    void sub(const complex &value)
    {
        _real -= value._real;
        _im -= value._im;
    }

    void mult(const complex &value)
    {
        double old_real = _real;
        _real = _real * value._real - _im * value._im;
        _im = old_real * value._im + _im * value._real;
    }

    void div(const complex &value)
    {
        double divider = value._im * value._im + value._real * value._real;
        double old_real = _real;
        if (fabs(divider) < __DBL_EPSILON__)
            throw std::overflow_error("Divide by zero exception");
        _real = (_real * value._real + _im * value._im) / divider;
        _im = (_im * value._real - old_real * value._im) / divider;
    }

    double argument() const { return atan(_im / _real) * 180.0 / M_PI; }

    double im_accessor() const { return _im; }
    double real_accerssor() const { return _real; }
    void info() const
    {
        std::cout << this->real_accerssor() << " + " << this->im_accessor() << "i; Argument = "
                  << this->argument() << "°; Module = " << this->module() << std::endl;
    }
};

int main()
{
    complex a(9.21, 1.21);
    complex b(0, 0);
    std::cout << "a = ";
    a.info();
    std::cout << "b = ";
    b.info();

    std::cout << "\nTest div:\n"
              << std::endl;
    try
    {
        a.div(b);
    }
    catch (std::overflow_error const &e)
    {
        std::cout << e.what() << std::endl;
    }

    a.info();
    b = complex(1.22, 1421.22);
    std::cout << "new b = ";
    b.info();

    try
    {
        a.div(b);
        std::cout << "a = ";
        a.info();
    }
    catch (std::overflow_error const &e)
    {
        std::cout << e.what() << std::endl;
    }

    a = complex(100, 100);
    b = complex(10, 15);
    std::cout << "\nnew a = ";
    a.info();
    std::cout << "new b = ";
    b.info();

    std::cout << "\nTest add:"
              << std::endl;
    a.add(b);
    std::cout << "a = ";
    a.info();
    std::cout << "\nTest sub:"
              << std::endl;
    a.sub(b);
    std::cout << "a = ";
    a.info();
    std::cout << "\nTest mult:"
              << std::endl;
    a.mult(b);
    std::cout << "a = ";
    a.info();
    return 0;
}