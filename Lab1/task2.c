#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>
#include <limits.h>

#define M_PI 3.14159265358979323846
#define MAX_COUNT_FACTORIAL 45
#define MAX_COUNT 1000000

int is_prime(int number)
{
    if (number < 2)
        return 0;
    for (int i = 2; i <= sqrt(number); i++)
    {
        if (number % i == 0)
            return 0;
    }
    return 1;
}

double equation_ln_x(double x)
{
    return log(x) - 1.0;
}

double equation_cos_x(double x)
{
    return cos(x) + 1.0;
}

double equation_pi(double x)
{
    return sin(x);
}

double equation_e_pow_x(double x)
{
    return exp(x) - 2.0;
}

double equation_sqrt2(double x)
{
    return x * x - 2.0;
}

double function_e_limit(double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0;
    }
    double new_value = 0.0;
    double value = -1.0;
    int count = 1;

    while (fabs(value - new_value) > epsilon && count < MAX_COUNT)
    {
        value = new_value;
        value = pow(1.0 + (1.0 / count), count);
        if (count == INT_MAX)
        {
            return -1.0;
        }
        count++;
    }

    return value;
}

double function_e_series(double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0;
    }
    double value = 0.0;
    double current = 1.0;
    int count = 1;

    while (fabs(current) > epsilon && count < MAX_COUNT)
    {
        value += current;
        current /= count;
        count++;
    }

    return value;
}

double function_pi_limit(double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0;
    }

    double new_value = 0.0;
    int count = 1;

    while (1)
    {
        double value = new_value;

        double numerator = pow(pow(2.0, count) * tgamma(count + 1), 4);
        double denominator = count * pow(tgamma(2 * count + 1), 2);

        new_value = numerator / denominator;

        if (fabs(new_value - value) < epsilon || count > MAX_COUNT)
        {
            return new_value;
        }
        count++;
    }
}

double function_pi_series(double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0;
    }

    int count = 0;
    double value = 0.0;
    double current = 0.0;

    while ((fabs(current) > epsilon || current == 0.0))
    {
        current = 4.0 * pow(-1.0, count) / (2 * count + 1);
        value += current;
        count++;
    }

    return value;
}

double function_ln2_limit(double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0;
    }
    double new_value = 0.0;
    double value = -1.0;
    int count = 1;

    while (fabs(new_value - value) > epsilon && count < MAX_COUNT)
    {
        value = new_value;
        new_value = count * (pow(2.0, 1.0 / count) - 1.0);
        count++;
    }

    return new_value;
}

double function_ln2_series(double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0;
    }
    double value = 0.0;
    double current = 0.0;
    int count = 1;

    while ((fabs(current) > epsilon || current == 0.0) && count < MAX_COUNT)
    {
        current = pow(-1.0, count - 1) / count;
        value += current;
        count++;
    }

    return value;
}

double function_sqrt2_limit(double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0;
    }

    double new_value;
    double value = -0.5;

    do
    {
        new_value = value - (value * value / 2) + 1;
        if (fabs(new_value - value) < epsilon)
        {
            break;
        }
        value = new_value;
    } while (1);

    return new_value;
}

double function_sqrt2_series(double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0;
    }

    double result = 1.0;
    int k = 2;
    double current;

    while (k < MAX_COUNT)
    {
        current = result;
        result *= pow(2.0, pow(2.0, -k));
        if (fabs(result - current) < epsilon)
        {
            break;
        }
        k++;
    }

    return result;
}

double function_gamma_limit(int maximum_iterations)
{
    double sum = 0.0;
    for (int k = 1; k <= maximum_iterations; k++)
    {
        double binomial_coeff = tgamma(maximum_iterations + 1) / (tgamma(k + 1) * tgamma(maximum_iterations - k + 1));
        double current = binomial_coeff * pow(-1, k) * lgamma(k + 1) / k;
        if (isnan(current) || isinf(current))
        {
            return -1;
        }
        sum += current;
    }
    return sum;
}

double function_gamma_series(int maximum_iterations)
{
    double result = -M_PI * M_PI / 6.0;
    for (int k = 2; k <= maximum_iterations; k++)
    {
        double current = (1.0 / pow(floor(sqrt(k)), 2)) - (1.0 / k);
        result += current;
    }
    return result;
}

double function_gamma_equation(int maximum_iterations)
{
    double result = 1.0;
    for (int p = 2; p <= maximum_iterations; p++)
    {
        if (is_prime(p))
        {
            result *= pow((p - 1.0) / p, 1.0 / p);
        }
    }
    return exp(-result);
}

double function_find_root(double (*f)(double), double a, double b, double epsilon)
{
    if (epsilon <= 0)
    {
        return -1.0;
    }
    if (f(a) * f(b) >= 0)
    {
        return NAN;
    }

    int iterations = 0;
    double center_point;

    while (fabs(b - a) > epsilon && iterations < MAX_COUNT)
    {
        center_point = (a + b) / 2.0;
        if (f(center_point) * f(a) < 0)
        {
            b = center_point;
        }
        else
        {
            a = center_point;
        }
        iterations++;
    }

    if (iterations >= INT_MAX)
    {
        return NAN;
    }

    return center_point;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("ERROR: Incorrect input\n");
        return -1;
    }

    double epsilon = atof(argv[1]);

    if (epsilon <= 0.0 || epsilon >= 1)
    {
        printf("ERROR: Epsilon must be a positive number and less then 1.0\n");
        return -1;
    }

    printf("Using epsilon >> %f\n\n", epsilon);

    printf("Calculating limit for e: %.8f\n", function_e_limit(epsilon));
    printf("Calculating series for e: %.8f\n", function_e_series(epsilon));
    double bisection_e = function_find_root(equation_ln_x, 2.0, 3.0, epsilon);
    if (isnan(bisection_e))
    {
        printf("ERROR in e equation\n");
    }
    else
    {
        printf("Calculating equation for e: %.8f\n\n", bisection_e);
    }

    printf("Calculating limit for pi: %.8f\n", function_pi_limit(epsilon));
    printf("Calculating series for pi: %.8f\n", function_pi_series(epsilon));
    double bisection_pi = function_find_root(equation_pi, 3.0, 3.5, epsilon);
    if (isnan(bisection_pi))
    {
        printf("ERROR in pi equation\n");
    }
    else
    {
        printf("Calculating eqaution for pi: %.8f\n\n", bisection_pi);
    }

    printf("Calculating limit for ln2: %.8f\n", function_ln2_limit(epsilon));
    printf("Calculating series for ln2: %.8f\n", function_ln2_series(epsilon));
    double bisection_ln2 = function_find_root(equation_e_pow_x, 0.5, 1.0, epsilon);
    if (isnan(bisection_ln2))
    {
        printf("ERROR in pi equation\n");
    }
    else
    {
        printf("Calculating eqaution for ln2: %.8f\n\n", bisection_ln2);
    }

    printf("Calculating limit for sqrt2: %.8f\n", function_sqrt2_limit(epsilon));
    printf("Calculating series for sqrt2: %.8f\n", function_sqrt2_series(epsilon));
    double bisection_sqrt2 = function_find_root(equation_sqrt2, 1.0, 2.0, epsilon);
    if (isnan(bisection_sqrt2))
    {
        printf("ERROR in pi equation\n");
    }
    else
    {
        printf("Calculating eqaution for sqrt2: %.8f\n\n", bisection_sqrt2);
    }

    printf("Calculating limits for gamma: %.8f\n", function_gamma_limit(MAX_COUNT_FACTORIAL));
    printf("Calculating series for gamma: %.8f\n", function_gamma_series(MAX_COUNT));
    printf("Calculating eqaution for gamma: %.8f\n", function_gamma_equation(MAX_COUNT));

    return 0;
}
