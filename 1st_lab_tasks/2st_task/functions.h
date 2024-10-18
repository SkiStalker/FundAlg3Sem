#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

enum ret_type_t{
    SUCCESS,
    ERROR_FULL,
    ERROR,
    ERROR_NAN
};
typedef double(*callback)(double);

//Перевод из строкового представления в double-овое
double chToD(char* x){
    double xD = 0;
    double m = 10;
    double mBuf = m;
    int flag = 1, flagMin = 0;
    

    for (int i = 0; i < (int)strlen(x); ++i){
        if (x[i] == '.'){
            flag = 0;
            m = 10.0;
            mBuf = m;
            continue;
        }
        if (x[i] == '-'){
            flagMin = 1;
            continue;
        }
        if (flag){
            xD = xD * m + (x[i] - '0');
        }
        else{
            xD = xD + ((double)(x[i] - '0')) / m;
            m *= 10.0;
            if (mBuf > m){
                return ERROR_FULL;
            }
            mBuf = m;
        }
    }
    if (flagMin){
        xD *= -1;
    }
    return xD;
}

//Проверка на простоту числа
int prime(int x){
    if (x <= 1){
        return 0;
    }
    else{
        for (int i = 2; i < (int)sqrt(x) + 1; ++i){
            if (x % i == 0){
                return 0;
            }
        }
    }
    return 1;
}

//Считает факториал
double fact(double x){
    if (x <= 1.0){
        return  1.0;
    }
    else{
        double sum = 1.0;
        double prevSum = sum;
        for (int i = 2; i <= x; i++){
            sum *= (double)i;
            if (sum < prevSum){
                return prevSum;
            }
        }
        return sum;
    }
}

//Формула для высчета сочетаний
double combination(int k, int n){
    return (double)fact(n)/(double)(fact(k)*fact(n-k));
}

//Уравнение для экспоненты из условия задания
double eExpression(double x){
    return log(x);
}

//Уравнение для числа пи из условия задания
double piExpression(double x){
    return cos(x);
}

//Уравнение для ln(2) из условия задания
double lnExpression(double x){
    return exp(x);
}

//Уравнение для sqrt(2) из условия задания
double sqrtTwoExpression(double x){
    return x * x;
}

//Уравнение для гаммы из условия задания
double gExpression(double x){
    return exp(-x);
}

//Решает уравнение через дихотомию
double dihotom(double left,double right, callback uravn, double answer, double eps){
    while ((right - left) >= eps){
        double half = (left + right) / 2.0;
        if (answer < uravn(half)){
            right = half;
        }
        else{
            left = half;
        }
    }
    return (left + right) / 2.0;
}

//Решает уравнение через тернарный поиск
double ternar(double left,double right, callback uravn, double eps){
    double m1 = 0, m2 = 0;
    while (right-left > eps){
        m1 = left+(right-left)/3.0;
        m2 = right-(right-left)/3.0;
        if (uravn(m1) > uravn(m2)){
            left = m1;
        }
        else{
            right = m2;
        }
    }
    return (m2+m1)/2.0;
}

//Считает экспоненту по пределу
double eLim(double eps){
    double prev = 2.0;

    for (int i = 2; i < 1000000; i++){
        double now = (double)pow(1.0 + 1.0/(double)i, i);
        if (fabs(now - prev) < eps/10.0){
            return now;
        }
        prev = now;
    }
    return ERROR;
}

//Считает экспоненту по Тейлору
double eTaylor(double eps){
    double prevEl = 1.0;
    double prevSum = prevEl;
    double nowSum = prevEl;

    for (int i = 1; i < 1000000; i++){
        double nowEl = prevEl/(double)i;
        nowSum += nowEl;
        if (fabs(prevSum - nowSum) < eps){
            return nowSum;
        }
        prevSum = nowSum;
        prevEl = nowEl;
    }
    return ERROR;
}

//Считает экспоненту по уравнению
double eUravn(double eps){
    double answer = 1.0;
    double left = 2.0, right = 3.0;
    return dihotom(left, right, &eExpression, answer, eps);
}

//Считает число пи по  пределу
double piLim(double eps){
    double prevEl = 4.0;

    for (int i = 2; i < 1000000; i++){
        double nowEl = prevEl * ((4.0 * (double)((i-1) * (i-1)) + 4.0 * (i-1))/(4.0 * (double)((i-1) * (i-1)) + 4.0 * (double)(i-1) + 1.0));
        if (fabs(prevEl - nowEl) < eps){
            return nowEl;
        }
        prevEl = nowEl;
    }
    return ERROR;
}

//Считает число пи по Тейлору
double piTaylor(double eps){
    double prevEl = 4.0;
    double prevSum = prevEl;
    double nowSum = prevEl;

    for (int i = 2; i < 1000000; i++){
        double nowEl = 4.0 * ((double)pow(-1.0, i-1) / (2.0 * (double)i - 1.0));
        nowSum += nowEl;
        if ((fabs(prevSum - nowSum) < eps) || (fabs(nowEl - prevEl) < eps)){
            return nowSum;
        }
        prevSum = nowSum;
        prevEl = nowEl;
    }
    return nowSum;
}

//Считает число пи по уравнению
double piUravn(double eps){
    // double answer = -1.0;
    double left = 3.0, right = 4.0;
    return ternar(left, right, &piExpression, eps);
}

//Считает натуральный логарифм 2 по пределу
double lnLim(double eps){
    double prev = 1.0;
    for (int i = 2; i < 1000000; i++){
        double now = (double)i*(pow(2, (double)1/i) - 1);
        if (fabs(now - prev) < eps){
            return now;
        }
        prev = now;
    }
    return ERROR;    
}

//Считает натуральный логарифм 2 по Тейлору
double lnTaylor(double eps){
    double prevEl = 1.0;
    // double prevSum = prevEl;
    double nowSum = prevEl;

    for (int i = 2; i < 1000000; i++){
        double nowEl;
        if (i % 2 == 1){
            nowEl = 1.0 / (double)i;
        }
        else{
            nowEl = (-1.0) / (double)i;
        }
        nowSum += nowEl;
        if (fabs(prevEl - nowEl) < eps){
            return nowSum;
        }
        // prevSum = nowSum;
        prevEl = nowEl;
    }
    return nowSum;
}

//Считает натуральный логарифм 2 по уравнению
double lnUravn(double eps){
    double answer = 2.0;
    double left = 0.0, right = 1.0;
    return dihotom(left, right, &lnExpression, answer, eps);
}

//Считает корень двух по пределу
double sqrtTwoLim(double eps){
    double prev = -0.5;
    for (int i = 1; i < 1000000; i++){
        double now = prev - (prev * prev) / 2.0 + 1;
        if (fabs(now - prev) < eps){
            return now;
        }
        prev = now;
    }
    return ERROR;    
}

//Считает корень двух по Тейлору
double sqrtTwoTaylor(double eps){
    double prevEl = pow(2.0, 0.25);
    double prevSum = prevEl;
    double nowSum = prevEl;

    for (int i = 3; i < 1000000; i++){
        double nowEl = pow(prevEl, 0.5);
        nowSum *= nowEl;
        if (fabs(prevSum - nowSum) < eps){
            return nowSum;
        }
        prevSum = nowSum;
        prevEl = nowEl;
    }
    return ERROR;
}

//Считает корень двух по уравнению
double sqrtTwoUravn(double eps){
    double answer = 2.0;
    double left = 1.0, right = 2.0;
    return dihotom(left, right, &sqrtTwoExpression, answer, eps);
}

//Считает гамму по пределу
double gLim(double eps){
    double sum = 0.0/eps;
    double m = 47;
    for (int k = 1; k <= m; k++)
    {
        double binomial_coefficient = tgamma(m + 1) / (tgamma(k + 1) * tgamma(m - k + 1));
        double term = binomial_coefficient * pow(-1, k) * lgamma(k + 1) / k;
        if (isnan(term) || isinf(term))
        {
            return -1;
        }
        sum += term;
    }
    return sum;
}

//Считает гамму по Тейлору
double gTaylor(double eps){
    double gamma = -piTaylor(eps) * piTaylor(eps) / 6.0;
    for (int i = 2; i < 1000000; i++)
    {
        double term = (1.0 / pow(floor(sqrt(i)), 2)) - (1.0 / (double)i);
        gamma += term;
    }
    return gamma;
}

//Считает гамму по уравнению
double gUravn(double eps){
    double product = 1.0 + 0/eps;
    for (int p = 2; p <= 1000000; p++)
    {
        if (prime(p))
        {
            product *= pow((p - 1.0) / p, 1.0 / p);
        }
    }
    return exp(-product);
}

//Проверяет является ли строковое представление вещественным числом
int checkNumber(char* x){
    int flagDot = 0, flagMin = 0;
    for (int i = 0; i < (int)strlen(x); i++){
        if ((x[i] < '0' || x[i] > '9') && (x[i] != '.') && (x[i] != '-')){
            return ERROR_NAN;
        }
        if (x[i] == '.'){
            flagDot++;
            if (flagDot > 1){
                return ERROR_NAN;
            }
        }
        if (x[i] == '-'){
            flagMin++;
            if (flagMin > 1){
                return ERROR_NAN;
            }
        }
    }
    return SUCCESS;
}