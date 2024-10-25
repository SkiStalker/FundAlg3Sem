#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

double sr_gem(int n, ...){
    va_list factor;
    va_start(factor, n);
    double res = 1;
    for (int i = 0; i < n; i++)
    {
        res *= va_arg(factor, double);
    }
    va_end(factor);
    return pow(res, 1.0/(double)n);
}

double rec_pow(double _x, int _y){
    if(_y == 0){
        return 1.0;
    }
    if(_y % 2 == 1){
        return rec_pow(_x, _y - 1) * _x;
    }
    else{
        double _z = rec_pow(_x, _y / 2);
        return _z * _z;
    }
}

int main(){
    double args[5];
    scanf("%lf %lf %lf %lf %lf", &args[0], &args[1], &args[2], &args[3], &args[4]);
    double _x;
    int y;
    scanf("%lf %d", &_x, &y);
    printf("%f\n", sr_gem(5, args[0], args[1], args[2], args[3], args[4]));
    printf("%lf\n", rec_pow(_x, y));
}