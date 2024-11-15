#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>

typedef enum return_code {
    OK,
    INVALID_INPUT,
    MEMORY_ALLOCATION_ERROR,
    OVERFLOW_ERROR,
    DEREFERENCING_NULL_POINTER_ATTEMPT,
    FILE_PATH_LENGTH_ERROR,
    FILE_OPENNING_ERROR
} return_code;

typedef struct Student {
    unsigned int id;
    char name[100];
    char last_name[100];
    char group[100];
    unsigned char* marks;
} Student;

return_code upload_info(Student** output_info, int* size, FILE* input_file);
return_code is_correct_input(Student student);
return_code get_student_info_by_parameter(Student** results, int* found_count, char* parameter, char* search_str, Student* students, int size);

int comp_by_id(const void *a, const void *b);
int comp_by_name(const void *a, const void *b);
int comp_by_lname(const void *a, const void *b);
int comp_by_group(const void *a, const void *b);

return_code sort(char* parameter, Student* students, int size);

double get_average_of_marks(Student student);

return_code append_info_in_file(FILE* output_file, Student student, char* request);

void interactive_dialog(Student* students, int size, FILE* output_file);

void print_student_info(Student* student);


