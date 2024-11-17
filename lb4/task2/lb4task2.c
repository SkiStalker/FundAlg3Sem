#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define START_HASH_SIZE 128

#define FREE_STR(str) \
    if (str)          \
    {                 \
        free(str);    \
        str = NULL;   \
    }

typedef enum ErrorCode {
	SUCCESS,
	INPUT_ERROR,
	MEMORY_ERROR,
	OVERFLOW_ERROR,
	MISSING,
	FILE_OPEN_ERROR
} ErrorCode;


typedef struct Arr {
	char name;
	int *data;
    size_t sz;
    size_t capacity;
} Arr;


ErrorCode string_to_int(const char* str, long *res) {
	char *end;
	*res = strtol(str, &end, 10);
	if (*res == LONG_MAX || *res == LONG_MIN || *end != '\0') {
		return INPUT_ERROR;
	}
	return SUCCESS;
}


ErrorCode string_to_size_t(const char* str, size_t * res) {
	char* end;
	*res = (size_t)strtol(str, &end, 10);
	if (*res == ULONG_MAX || *end != '\0') {
		return INPUT_ERROR;
	}
	return SUCCESS;
}


void init_arr(Arr* arr, char name) {
	if (!arr) {
		return;
	}
	arr->name = name;
	arr->sz = arr->capacity = 0;
	arr->data = NULL;
}


void free_arr(Arr* arr) {
	if (!arr) {
		return;
	}
	free(arr->data);
	arr->data = NULL;
	arr->capacity = arr->sz = 0;
}


ErrorCode load_arr(Arr* arr, FILE* input) {
	size_t sz = 0, capacity = 2;
	int* new_data;
	char buf[BUFSIZ];
	long num;
	if (arr == NULL || input == NULL) {
		return MEMORY_ERROR;
	}
	new_data = (int*)malloc(sizeof(int) * capacity);
	if (new_data == NULL) {
		return MEMORY_ERROR;
	}
	while ((fscanf(input, "%s", buf)) == 1) {
		if (string_to_int(buf, &num)) {
			free(new_data);
			return MEMORY_ERROR;
		}
		if (sz == capacity) {
			int* realloc_data;
			capacity *= 2;
			realloc_data = (int*)realloc(new_data, sizeof(int) * capacity);
			if (realloc_data == NULL) {
				free(new_data);
				return MEMORY_ERROR;
			}
			new_data = realloc_data;
		}
		new_data[sz++] = (int)num;
	}
	if (!feof(input)) {
		free(new_data);
		return INPUT_ERROR;
	}
	free(arr->data);
	arr->data = new_data;
	arr->sz = sz;
	arr->capacity = capacity;
	return SUCCESS;
}


ErrorCode save_arr(Arr* arr, FILE* output){
	size_t i;
	if (!arr || !output)
		return MEMORY_ERROR;

	for (i = 0; i < arr->sz; ++i){
		fprintf(output, "%d ", arr->data[i]);
	}
	return SUCCESS;
}


ErrorCode rand_arr(Arr* arr, int count, int lb, int rb) {
	int *new_data;
	size_t i;
	size_t capacity = count;
	size_t sz = count;
	if (arr == NULL) {
		return MEMORY_ERROR;
	}
	if (lb > rb || count <= 0) {
		return INPUT_ERROR;
	}
	new_data = (int*)malloc(sizeof(int) * capacity);
	if (new_data == NULL) {
		return MEMORY_ERROR;
	}
	for (i = 0; i < sz; ++i) {
		new_data[i] = lb + rand() % (rb - lb + 1);
	}

	free(arr->data);
	arr->data = new_data;
	arr->sz = sz;
	arr->capacity = capacity;
	return SUCCESS;
}



ErrorCode concat_arr(Arr* arr_1, const Arr* arr_2) {
    if (arr_1 == NULL || arr_2 == NULL || arr_1->data == NULL || arr_2->data == NULL) {
        return MEMORY_ERROR;
    }
    size_t new_len = arr_1->sz + arr_2->sz;
    if (new_len == 0) {
        return SUCCESS; 
    }
    int* new_data = (int*)malloc(sizeof(int) * new_len);
    if (new_data == NULL) {
        return MEMORY_ERROR; 
    }
    for (size_t i = 0; i < arr_1->sz; ++i) {
        new_data[i] = arr_1->data[i];
    }
    for (size_t i = 0; i < arr_2->sz; ++i) {
        new_data[arr_1->sz + i] = arr_2->data[i];
    }
    free(arr_1->data);
    arr_1->data = new_data;
    arr_1->capacity = new_len;
    arr_1->sz = new_len;

    return SUCCESS;
}



ErrorCode remove_arr(Arr* arr, size_t x, size_t count) {
    if (arr == NULL || arr->data == NULL) {
        return MEMORY_ERROR; 
    }

    if (x >= arr->sz) {
        return INPUT_ERROR;
    }
    if (count == 0 || x >= arr->sz) {
        return SUCCESS;
    }
    if (x + count > arr->sz) {
        count = arr->sz - x;
    }
    for (size_t i = x; i + count < arr->sz; ++i) {
        arr->data[i] = arr->data[i + count];
    }
    arr->sz -= count;
    if (arr->sz < arr->capacity) {
        int* resized_data = (int*)realloc(arr->data, sizeof(int) * arr->sz);
        if (resized_data == NULL) {
            return MEMORY_ERROR; 
        }
        arr->data = resized_data;
        arr->capacity = arr->sz;
    }

    return SUCCESS;
}



ErrorCode copy_arr(Arr *dest, Arr *sourse, size_t lb, size_t rb)
{
	size_t i, sz, capacity = rb - lb + 1;
	int* new_data;
	if (!dest || !sourse)
		return MEMORY_ERROR;

	new_data = (int*)malloc(sizeof(int) * capacity);
	if (!new_data)
		return MEMORY_ERROR;


	sz = 0;
	for (i = lb; i < sourse->sz; ++i)
	{
		if (i > rb)
			break;
		new_data[sz++] = sourse->data[i];
	}
	free(dest->data);
	dest->data = new_data;
	dest->capacity = capacity;
	dest->sz = sz;
	return SUCCESS;
}


int compare_a(const void* a, const void* b){
	return (*(int*)a - *(int*)b);
}

int compare_d(const void* a, const void* b){
	return (*(int*)b - *(int*)a);
}

int compare_s(const void* a, const void* b) {
    (void)a;
    (void)b;  
    return rand() % 3 - 1;
}



ErrorCode print_one_element(Arr* arr, size_t idx){
	if (arr == NULL) {
		return MEMORY_ERROR;
	}
	if (idx >= arr->sz) {
		return INPUT_ERROR;
	}
	printf("%d\n", arr->data[idx]);
	return SUCCESS;
}


ErrorCode print_all(Arr* arr){
	size_t i;
	if (arr == NULL) {
		return MEMORY_ERROR;
	}
	if (arr->sz == 0){
		printf("Print: Array is empty\n");
		return SUCCESS;
	}
	for (i = 0; i < arr->sz; ++i) {
		printf("%d ", arr->data[i]);
	}
	printf("\n");
	return SUCCESS;
}

ErrorCode print_section(Arr* arr, size_t lb, size_t rb)
{
	size_t i;
	if (arr == NULL) {
		return MEMORY_ERROR;
	}
	if (arr->sz == 0){
		printf("Print: Array is empty\n");
		return SUCCESS;
	}

	rb = (rb < arr->sz) ? (rb) : (arr->sz - 1);
	for (i = lb; i <= rb; ++i) {
		printf("%d ", arr->data[i]);
	}
	printf("\n");
	return SUCCESS;
}

ErrorCode print_stats(Arr* arr){
	int* copy_data, count = 1, most_freq_num, max_count = -1;
	double avg = 0, deviation;
	size_t i;
	if (arr == NULL) {
		return MEMORY_ERROR;
	}
	if (arr->sz == 0){
		printf("Stats: Arr is empty\n");
		return SUCCESS;
	}
	copy_data = (int*)malloc(sizeof(int) * arr->capacity);
	memcpy(copy_data, arr->data, sizeof(int) * arr->sz);
	qsort(copy_data, arr->sz, sizeof(int), compare_d);

	for (i = 0; i < arr->sz - 1; ++i)
	{
		if (copy_data[i] == copy_data[i + 1])
			count += 1;
		else
		{
			if (count > max_count)
			{
				max_count = count;
				most_freq_num = copy_data[i];
			}
			count = 1;
		}
		avg += ((double)copy_data[i] / arr->sz);
	}
	avg += ((double)copy_data[i] / arr->sz);
	deviation = (fabs(copy_data[0] - avg) > fabs(copy_data[arr->sz - 1] - avg)) ? fabs(copy_data[0] - avg) : fabs(copy_data[arr->sz - 1] - avg);
	printf("----------------------------------------\n");
	printf("Stats %c:\n", arr->name);
	printf("Array size: %zu;\n", arr->sz);
	printf("Max: %d; Min: %d;\n", copy_data[0], copy_data[arr->sz - 1]);
	printf("The most frequent: %d, time(s): %d\n", most_freq_num, max_count);
	printf("Avg: %.3lf; Max deviation: %.3lf\n", avg, deviation);
	printf("----------------------------------------\n");
	free(copy_data);
	return SUCCESS;
}

ErrorCode read_command (char** command, FILE* input) {
	char* str, ch;
	size_t sz = 0, capacity = 2;
	if (!command || !input) {
		return MEMORY_ERROR;
	}
	*command = NULL;
	ch = getc(input);
    while (ch != EOF && isspace(ch)) {
        ch = getc(input);
    }
	if (ch == EOF) {
		return SUCCESS;
	}
	str = (char*)malloc(sizeof(char) * capacity);
	if (!str) {
		return MEMORY_ERROR;
	}
	while (ch != ';')
	{
		if (ch == EOF)
		{
			free(str);
			return INPUT_ERROR;
		}
		if (!isspace(ch))
		{
			if (sz + 2 == capacity)
			{
				char* for_real;
				capacity *= 2;
				for_real = (char*)realloc(str, capacity * sizeof(char));
				if (!for_real)
				{
					free(str);
					return MEMORY_ERROR;
				}
				str = for_real;
			}
			str[sz++] = ch;
		}
		ch = getc(input);
	}
	str[sz++] = ';';
	str[sz] = '\0';
	*command = str;
	return SUCCESS;
    
}

void delete_all(Arr *arr){
	size_t i;
	if (arr == NULL) {
		return;
	}
	for (i = 0; i < 31; ++i){
		free_arr(&arr[i]);
	}
}


int main(int argc, char** argv)
{
    char* command;
    int line = 0;
    size_t i;
    ErrorCode state;
    Arr arrays[31];
    FILE* input;

    if (argc != 2)
    {
        printf("Input error. Enter <program_name><your_program_file>\n");
        return INPUT_ERROR;
    }
    input = fopen(argv[1], "r");
    if (!input)
    {
        printf("File open error\n");
        return FILE_OPEN_ERROR;
    }

    for (i = 0; i < 31; ++i)
    {
        init_arr(&arrays[i], 'A' + i);
    }

    srand(time(NULL));

    while (!feof(input))
    {
        state = read_command(&command, input);
        line++;
        if (state == INPUT_ERROR)
        {
            printf("Input error, line: %d\n", line);
            FREE_STR(command);
            delete_all(arrays);
            fclose(input);
            return INPUT_ERROR;
        }
        if (state == MEMORY_ERROR)
        {
            printf("Memory error\n");
            FREE_STR(command);
            fclose(input);
            return MEMORY_ERROR;
        }
        if (command == NULL)
            break;

        if (strstr(command, "Load"))
        {
            char arr_name, check;
            char file_path[1024];
            FILE* read_file;
            if (sscanf(command, "Load%c,%1023[^;]%c", &arr_name, file_path, &check) != 3 || !isalpha(arr_name) || check != ';')
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
            if (!(read_file = fopen(file_path, "r")))
            {
                printf("File open error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return FILE_OPEN_ERROR;
            }
            arr_name = toupper(arr_name);
            state = load_arr(&arrays[arr_name - 'A'], read_file);
            fclose(read_file);
            if (state == MEMORY_ERROR)
            {
                printf("Memory error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return MEMORY_ERROR;
            }
            if (state == INPUT_ERROR)
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
        }

        else if (strstr(command, "Save"))
        {
            char arr_name, check;
            char file_path[1024];
            FILE* save_file;
            if (sscanf(command, "Save%c,%1023[^;]%c", &arr_name, file_path, &check) != 3 || !isalpha(arr_name) || check != ';')
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
            if (!(save_file = fopen(file_path, "w")))
            {
                printf("File open error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return FILE_OPEN_ERROR;
            }
            arr_name = toupper(arr_name);
            state = save_arr(&arrays[arr_name - 'A'], save_file);
            fclose(save_file);
        }

        else if (strstr(command, "Concat"))
        {
            char name_1, name_2, check;

            if (sscanf(command, "Concat%c,%c%c", &name_1, &name_2, &check) != 3 || !isalpha(name_1) || !isalpha(name_2) || check != ';')
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                return INPUT_ERROR;
            }
            name_1 = toupper(name_1);
            name_2 = toupper(name_2);

            if (concat_arr(&arrays[name_1 - 'A'], &arrays[name_2 - 'A']))
            {
                printf("Memory error, line: %d\n", line);
                delete_all(arrays);
                fclose(input);
                FREE_STR(command);
                return MEMORY_ERROR;
            }
        }

        else if (strstr(command, "Free"))
        {
            char name_1, check;

            if (sscanf(command, "Free(%c)%c", &name_1, &check) != 2 || check != ';' || !isalpha(name_1))
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                fclose(input);
                FREE_STR(command);
                return INPUT_ERROR;
            }
            name_1 = toupper(name_1);

            free_arr(&arrays[name_1 - 'A']);
        }
        else if (strstr(command, "Sort"))
        {
            char name_1, op, check;

            if (sscanf(command, "Sort%c%c%c", &name_1, &op, &check) != 3 || check != ';' || !isalpha(name_1) || (op != '+' && op != '-'))
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
            name_1 = toupper(name_1);

            qsort(arrays[name_1 - 'A'].data, arrays[name_1 - 'A'].sz, sizeof(int), (op == '+') ? compare_a : compare_d);
        }
        else if (strstr(command, "Shuffle"))
        {
            char name_1, check;

            if (sscanf(command, "Shuffle%c%c", &name_1, &check) != 2 || check != ';' || !isalpha(name_1))
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
            name_1 = toupper(name_1);

            qsort(arrays[name_1 - 'A'].data, arrays[name_1 - 'A'].sz, sizeof(int), compare_s);
        }
        else if (strstr(command, "Stats"))
        {
            char name_1, check;

            if (sscanf(command, "Stats%c%c", &name_1, &check) != 2 || check != ';' || !isalpha(name_1))
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
            name_1 = toupper(name_1);

            if (print_stats(&arrays[name_1 - 'A']) == MEMORY_ERROR)
            {
                printf("Memory error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
        }
        else if (strstr(command, "Rand"))
        {
            char name_1, check;
            char _lb[256], _rb[256], _count[256];
            long lb, rb, count;

            if (sscanf(command, "Rand%c,%255[0-9],%255[0-9-],%255[0-9-]%c", &name_1, _count, _lb, _rb, &check) != 5 || check != ';' || !isalpha(name_1))
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);

                fclose(input);
                return INPUT_ERROR;
            }
            name_1 = toupper(name_1);

            if (string_to_int(_lb, &lb) || string_to_int(_rb, &rb) || string_to_int(_count, &count))
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
            if (rand_arr(&arrays[name_1 - 'A'], (int)count, (int)lb, (int)rb) == MEMORY_ERROR)
            {
                printf("Memory error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
        }
        else if (strstr(command, "Remove"))
        {
            char name_1, check;
            char _lb[256], _count[256];
            size_t lb, count;

            if (sscanf(command, "Remove%c,%255[0-9],%255[0-9]%c", &name_1, _count, _lb, &check) != 4 || check != ';' || !isalpha(name_1))
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
            name_1 = toupper(name_1);

            if (string_to_size_t(_lb, &lb) || string_to_size_t(_count, &count))
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
            if (remove_arr(&arrays[name_1 - 'A'], lb, count) == MEMORY_ERROR)
            {
                printf("Memory error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
        }
        else if (strstr(command, "Copy"))
        {
            char name_1, check, name_2;
            char _lb[256], _rb[256];
            size_t lb, rb;

            if (sscanf(command, "Copy%c,%255[0-9],%255[0-9],%c%c", &name_1, _lb, _rb, &name_2, &check) != 5 || check != ';' || !isalpha(name_1) || !isalpha(name_2))
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
            name_1 = toupper(name_1);
            name_2 = toupper(name_2);

            if (string_to_size_t(_lb, &lb) || string_to_size_t(_rb, &rb) || lb > rb)
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
            if (copy_arr(&arrays[name_2 - 'A'], &arrays[name_1 - 'A'], lb, rb) == MEMORY_ERROR)
            {
                printf("Memory error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
        }
        else if (strstr(command, "Print"))
        {
            char name_1, check;
            char _lb[256], _rb[256], _pos[256];
            size_t lb, rb, pos;

            if (sscanf(command, "Print%c,%255[0-9]%c", &name_1, _pos, &check) == 3 && check == ';' && isalpha(name_1))
            {
                name_1 = toupper(name_1);
                if (string_to_size_t(_pos, &pos) || print_one_element(&arrays[name_1 - 'A'], pos) == INPUT_ERROR)
                {
                    printf("Input error, line: %d\n", line);
                    delete_all(arrays);
                    FREE_STR(command);
                    fclose(input);
                    return INPUT_ERROR;
                }
            }
            else if (sscanf(command, "Print%c,%255[0-9],%255[0-9]%c", &name_1, _lb, _rb, &check) == 4 && check == ';' && isalpha(name_1))
            {
                name_1 = toupper(name_1);
                if (string_to_size_t(_lb, &lb) || string_to_size_t(_rb, &rb) || rb < lb)
                {
                    printf("Input error, line: %d\n", line);
                    delete_all(arrays);
                    FREE_STR(command);
                    fclose(input);
                    return INPUT_ERROR;
                }
                print_section(&arrays[name_1 - 'A'], lb, rb);
            }
            else if (sscanf(command, "Print%c,all%c", &name_1, &check) == 2 && check == ';' && isalpha(name_1))
            {
                name_1 = toupper(name_1);
                print_all(&arrays[name_1 - 'A']);
            }
            else
            {
                printf("Input error, line: %d\n", line);
                delete_all(arrays);
                FREE_STR(command);
                fclose(input);
                return INPUT_ERROR;
            }
        }
        else
        {
            printf("Input error, line: %d\n", line);
            delete_all(arrays);
            fclose(input);
            FREE_STR(command);
            return INPUT_ERROR;
        }
        FREE_STR(command);
    }
    FREE_STR(command);
    delete_all(arrays);
    fclose(input);
}