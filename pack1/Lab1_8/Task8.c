#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>

#define MAX_BASE 36
#define MAX_NUMBER_LEN 101

typedef enum {
	SUCCESS,
	INVALID_NUMBER,
	OVERFLOW,
	FILE_ERROR,
    ERROR
} Errors;

int find_min_base(const char* str) {
	int max_digit = 0;
    int start_index = 0;
    if (str[0] == '-') {
        start_index = 1;
    }
	for (int i = start_index; str[i] != '\0'; i++) {
		if (isdigit(str[i])) {
			if (str[i] - '0' > max_digit) {
				max_digit = str[i] - '0';
			}
		}
		else if (isalpha(str[i])) {
			if (toupper(str[i]) - 'A' + 10 > max_digit)  {
				max_digit = toupper(str[i]) - 'A' + 10;
			}
		}
		else {
            return ERROR;
		}
	}
    int min_base = 2;
	if (max_digit < 1) {
		return min_base;
	}
	else {
		return max_digit + 1;
	}
}

long long conv_to_demical(const char* str, int base) {
	long long res = 0;
    long long power = 1;
    int start_index = 0;
    int sign = 1;
    if (str[0] == '-') {
        start_index = 1;
        sign = -1;
    }

    for (int i = strlen(str) - 1; i >= start_index; i--){
		int digit;
		if (isdigit(str[i]))
		{
			digit = str[i] - '0';
		}
		else
		{
			digit = toupper(str[i]) - 'A' + 10;
		}
		if (res > LLONG_MAX / base || (res == LLONG_MAX / base && digit > LLONG_MAX % base))
		{
			return OVERFLOW;
		}

		res += digit * power;
		if (power > LLONG_MAX / base)
		{
			return OVERFLOW;
		}
		power *= base;
	}
	return sign * res;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }
    if (strcmp(argv[1], argv[2]) == 0)
    {
        printf("Error: Input and output files must be different.\n");
        return 1;
    }

    FILE* input_file = fopen(argv[1], "r");
    if (input_file == NULL)
    {
        printf("Error opening input file");
        return 0;

    }

    FILE* output_file = fopen(argv[2], "w");
    if (output_file == NULL)
    {
        printf("Error opening output file");
        fclose(input_file);
        return 0;
    }

    char number_str[MAX_NUMBER_LEN];

    while (fscanf(input_file, "%100s", number_str) == 1)
    {
        size_t length = strlen(number_str);


        if (length >= MAX_NUMBER_LEN) {
            fprintf(stderr, "Too long number: %s\n", number_str);
            continue;
        }
        int i = 0;
        while (number_str[i] == '0' && number_str[i + 1] != '\0')
        {
            i++;
        }

        if (number_str[i] == '\0')
        {
            fprintf(output_file, "0 2 0\n");
            continue;
        }

        int min_base = find_min_base(number_str + i);
        if ((min_base == ERROR && min_base != 4)|| min_base > MAX_BASE || min_base < 2)
        {
            fprintf(stderr, "Invalid number: %s\n", number_str);
            continue;
        }
        long long result = conv_to_demical(number_str, min_base);
        if (result == OVERFLOW){
            fprintf(stderr, "Overflow detected for number: %s\n", number_str);
            continue;
        }
        if (result < 0) {
            fprintf(output_file, "%s %d %lld\n", number_str + i, min_base, result);
        }
        else {
            fprintf(output_file, "%s %d %lld\n", number_str + i, min_base, result);
        }
        
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}