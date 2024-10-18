#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

enum ret_of_prog{
    SUCCESS = 0,
    ERROR = -1
};

typedef double (*callback)(int);
typedef double (*bin_callback)(double);
double sigma (double, int, callback);

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

int has_only_numbers(char* arg){
    for (int i = 0; i < (int)strlen(arg); i++)
    {
        if(!((arg[i] >= '0' && arg[i] <= '9') || arg[i] == '.' || arg[i] == '-')){
            return 0;
        }
    }
    return 1;
}

long long int fact(int number){
    if(number < 0){
        printf("Error in counting factorial! The number is below zero!\n");
        return 1;
    }
    if(number == 0 || number == 1){
        return 1;
    }
    long long int ans = 1;
    for (int i = 2; i <= number; i++)
    {
        ans = ans * i;
    }
    return ans;
}

double Exp_sum(int x){
    if(x < 0){
        printf("Factorial with the argument < 0!\n");
        return 1;
    }
    return 1.0 / fact(x);
}

double exp_lim(int n){
    return pow((1.0 + (1.0 / (double)n)), (double)n);
}

double pi_lim(double eps){
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

double ln2_lim(int x){
    return (double)x * (pow(2.0, (double)1/x) - 1.0);
}

double limit(double epsilon, callback func){

    double F2, F1 = 1.0;
    for (int i = 2; i < 1000000000; i++)
    {
        F2 = func(i);
        if(fabs(F2 - F1) < epsilon){
            return F2;
        }
        F1 = F2;
    }
    return F2;
}

double PI_sum(int x){
    if(x % 2 == 1){
        return 4.0 / (double)(2 * x - 1);
    }
    else{
        return (-4.0) / (double)(2 * x - 1);
    }
}

double ln2_sum(int x){
    if(x % 2 == 1){
        return 1.0 / (double)x;
    }
    else{
        return (-1.0) / (double)x;
    }
}

double sqrt2_bin(double x){
    return x * x;
}
double ln2_bin(double x){
    return pow(exp(1), x);
}

double pi_bin(double x){
    return cos(x);
}

double exp_bin(double x){
    return log(x);
}

double binary_search(double epsilon, double right_answer, bin_callback func){
    double right = 4.0;
    double left = 0.0;
    double mid = 0;

    while(right - left >= (epsilon / 10)){
        mid = (left + right) / 2.0;
        if(func(mid) > right_answer){
            right = mid;
        }
        else{
            left = mid;
        }
    }
    return mid;
}

double ternar_search_min(double epsilon, bin_callback func){
    
    double l = 0.0, r = 5.0;
    double m1, m2;
    while(r - l > epsilon){
        m1 = l + (r - l) / 3;
        m2 = r - (r - l) / 3;
        if(func(m1) > func(m2)){
            l = m1;
        }
        else{
            r = m2;
        }
    }
    return (m2 + m1) / 2.0;
}

double sqrt2_sum(int x){
    return pow(2, pow(2, -x));
}

double gamma(double eps){
    double gamma = -sigma(eps, 1, PI_sum) * sigma(eps, 1, PI_sum) / 6.0;
    for (int i = 2; i < 1000000; i++)
    {
        double term = (1.0 / pow(floor(sqrt(i)), 2)) - (1.0 / (double)i);
        gamma += term;
    }
    return gamma;
}

double times(double epsilon, int begin, callback func){
    double sum = 1;
    double new_sum;

    for (int i = begin; i < 1000; i++)
    {
        new_sum = func(i) * sum;
        if(fabs(sum - new_sum) < epsilon / 10){
            return new_sum;
        }
        else{
            sum = new_sum;
        }
    }
    return sum;
}

double sqrt2(double epsilon){
    double x_prev;
    double x = -0.5;
    do
    {
        x_prev = x;
        x = x_prev - ((x_prev * x_prev) / 2.0) + 1.0;
    } while (fabs(x - x_prev) > epsilon);
    return x;
}

double sigma(double epsilon, int begin, callback func){
    double sum = 0;
    double new_sum;

    for (int i = begin; i < 1000000000; i++)
    {
        new_sum = func(i);
        if(fabs(new_sum) < epsilon / 10){
            return new_sum + sum;
        }
        else{
            sum = sum + new_sum;
        }
    }
    return sum;
}

double gamma_lim(){
    double sum = 0.0;
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

double gamma_uravn(){
    double product = 1.0;
    for (int p = 2; p <= 1000000; p++)
    {
        if (prime(p))
        {
            product *= pow((p - 1.0) / p, 1.0 / p);
        }
    }
    return exp(-product);
}


int main(int argc, char* argv[]){
    
    double epsilon;
    if(argc <= 1){
        printf("No arguments given.\n");
        return ERROR;
    }
    else if(argc > 2){
        printf("Too much arguments!\n");
        return ERROR;
    }
    //printf("%s\n", argv[1]);
    if(has_only_numbers(argv[1])){
        epsilon = atof(argv[1]);
    }
    else{
        printf("Epsilon must be a number!\n");
        return ERROR;
    }


    printf("_________________________________________________________\n");
    printf("|--constants--|----limit----|--sum/times--|----binary---|\n");
    printf("|-------------|-------------|-------------|-------------|\n");
    printf("|%13s|%13f|%13f|%13f|\n", "EXP", limit(epsilon, exp_lim), sigma(epsilon, 0, Exp_sum), binary_search(epsilon, 1, exp_bin));
    printf("|%13s|%13f|%13f|%13f|\n", "PI",pi_lim(epsilon), sigma(epsilon, 1, PI_sum), ternar_search_min(epsilon, pi_bin));
    printf("|%13s|%13f|%13f|%13f|\n", "ln2", limit(epsilon, ln2_lim), sigma(epsilon, 1, ln2_sum), binary_search(epsilon, 2, ln2_bin));
    printf("|%13s|%13f|%13f|%13f|\n", "sqrt2", sqrt2(epsilon), times(epsilon, 2, sqrt2_sum), binary_search(epsilon, 2, sqrt2_bin));
    printf("|%13s|%13f|%13f|%13f|\n", "gamma", gamma_lim(), gamma(epsilon), gamma_uravn());
    printf("|_____________|_____________|_____________|_____________|\n");
    
    return SUCCESS;
    
}