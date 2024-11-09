#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>


typedef enum StopType { StopStart, StopMiddle, StopEnd } StopType;
typedef int (*for_check_symbol)(int);

typedef struct tm tm;

typedef struct Stop {
    float lat;
    float lon;
    StopType type;
    tm in;
    tm out;
} Stop;

typedef struct Route {
    char *id;
    size_t cap;
    size_t size;
    Stop *stops;
} Route;

typedef struct Routes {
    size_t cap;
    size_t size;
    Route *ptr;
} Routes;

/*
    void* memcpy(void *dest, const void *src, size_t n) {
        size_t i; //беззнаковый целый тип для представления объекта любого размера в памяти
        char *d = dest;
        const char *s = src;
        for (i = 0; i < n; i++) {
            d[i] = s[i];
        }
        return dest;
    }
*/

int func_for_string_parsing(char **res, char const **start, for_check_symbol m) {
    const char *end = *start;
    while (*end && !m(*end)) {
        end++;
    }
    int n = end - *start;
    *res = malloc(n + 1);
    if (!res)
        return -1;
    memcpy(*res, *start, n);
    (*res)[n] = 0;
    *start = end + 1;
    return 0;
}

int parse_field_uint(unsigned long *res, char const **start, for_check_symbol m) {
    char *ptr;
    *res = strtoul(*start, &ptr, 10);
    bool valid = *ptr == 0;
    valid = valid | m(*ptr);
    if (!valid) {
        ptr++;
        char tmp = *ptr;
        *ptr = 0;
        printf( "I can`t parse [%s] as unsigned\n", *start);
        *ptr = tmp;
        return -1;
    }

    *start = ptr + 1;
    return 0;
}

int function_for_parse_float(float *res, char const **start, for_check_symbol m) {
    char *ptr;
    *res = strtof(*start, &ptr);
    bool valid = *ptr == 0;
    valid = valid | m(*ptr);
    if (!valid) {
        ptr++;
        char tmp = *ptr;
        *ptr = 0;
        printf("I can`t parse [%s] as float\n", *start);
        *ptr = tmp;
        return -1;
    }

    *start = ptr + 1;
    return 0;
}

int parse_field_time(tm *res, const char **start, for_check_symbol m) {
    char *str;
    func_for_string_parsing(&str, start, m);
    char *last = strptime(str, "%d.%m.%Y %H:%M:%S", res);
    // if (last == NULL || *last != 0) {
    //     fprintf(stderr, "ERROR: failed to parse [%s] as date\n", str);
    //     free(str);
    //     return -1;
    // }
    res->tm_isdst = -1;
    free(str);
    return 0;
}
int stop_print(const Stop *s) {
    char in[26];
    strftime(in, 26, "%d.%m.%Y %H:%M:%S", &s->in);
    char out[26];
    strftime(out, 26, "%d.%m.%Y %H:%M:%S", &s->out);
    char type[20];
    switch (s->type) {
    case StopStart:
        strcpy(type, "start");
        break;
    case StopMiddle:
        strcpy(type, "intermediate");
        break;
    case StopEnd:
        strcpy(type, "end");
        break;
    };
    printf("%s (Stop Number: %.2f) [Time in: %s]-[Time out: %s]", type, s->lat, in, out);
    return 0;
}

int route_init(Route *r) {
    r->cap = 16;
    r->stops = malloc(r->cap * sizeof(Stop));
    if (!r->stops) {
        return -1;
    }
    return 0;
}
int route_free(Route *r) {
    free(r->stops);
    free(r->id);
    return 0;
}
int print_routes(const Route *r) {
    printf("%s:\n\t\t%zu\n", r->id, r->size);
    for (size_t i = 0; i < r->size; i++) {
        printf("\t[%02zu]: ", i);
        stop_print(&r->stops[i]);
        printf("\n");
    }
    return 0;
}

int route_push(Route *r, Stop s) {
    if (r->size >= r->cap) {
        size_t new_cap = r->cap * 2;
        Stop *t = realloc(r->stops, new_cap * sizeof(Stop));
        if (!t)
            return -1;
        r->stops = t;
        r->cap = new_cap;
    }
    r->stops[r->size] = s;
    r->size++;
    return 0;
}

int init_route(Routes *r) {
    r->cap = 16;
    r->ptr = malloc(r->cap * sizeof(Route));
    if (!r->ptr) {
        return -1;
    }
    return 0;
}
int free_route(Routes *r) {
    for (size_t i = 0; i < r->size; i++) {
        route_free(&r->ptr[i]);
    }
    free(r->ptr);
    return 0;
}

int add_route(Routes *r, Route s) {
    if (r->size >= r->cap) {
        size_t new_cap = r->cap * 2;
        Route *t = realloc(r->ptr, new_cap * sizeof(Route));
        if (!t)
            return -1;
        r->ptr = t;
        r->cap = new_cap;
    }
    r->ptr[r->size] = s;
    r->size++;
    return 0;
}

int add_route_id(Routes *r, const char *id) {
    Route ro = {0};
    int check_error = route_init(&ro);
    if (check_error) {
        printf("I can`t init route\n");
        return -1;
    }

    int n = strlen(id);
    ro.id = malloc(n + 1);
    if (ro.id == NULL)
        return -1;
    strcpy(ro.id, id);
    return add_route(r, ro);
}

int is_sep(int c) {
    return c == ';';
}

int parse_files(Routes *r, FILE *f) {
    size_t line_len = 0;
    char *line = NULL;
    int n = getline(&line, &line_len, f);
    if (n <= 0) {
        return -1;
    }

    const char *s = line;
    float lat, lon;
    int check_error = function_for_parse_float(&lat, &s, isspace);
    if (check_error) {
        free(line);
        printf("I can`t parse line\n");
        return -1;
    }

    check_error = function_for_parse_float(&lon, &s, isspace);
    if (check_error) {
        free(line);
        printf("I can`t parse line\n");
        return -1;
    }

    //все, что осталось в файле
    while (true) {
        int n = getline(&line, &line_len, f);
        if (n <= 0) {
            break;
        }

        n--;
        line[n] = 0;
        const char *s = line;

        char *id;
        tm in;
        tm out;
        unsigned long type;
        int check_error = func_for_string_parsing(&id, &s, is_sep);
        if (check_error) {
            printf("I can`t parse line\n");
            free(line);
        }
        check_error = parse_field_time(&in, &s, is_sep);
        if (check_error) {
            printf("I can`t parse line\n");
            free(line);
        }
        check_error = parse_field_time(&out, &s, is_sep);
        if (check_error) {
            printf("I can`t parse line\n");
            free(line);
        }
        check_error = parse_field_uint(&type, &s, is_sep);
        if (check_error) {
            printf("I can`t parse line\n");
            free(line);
        }
        if (*id == 0) {
            printf("Sorry, but id must not be empty\n");
            free(line);
            free(id);
            return -1;
        }
        if (type > 2) {
            printf("Sorry, but stop type can only be in [0;2] range");
            free(line);
            free(id);
            return -1;
        }

        Route *ro = NULL;
        for (size_t i = 0; i < r->size; i++) {
            if (strcmp(r->ptr[i].id, id) == 0) {
                ro = &r->ptr[i];
                break;
            }
        }
        if (ro == NULL) {
            int check_error = add_route_id(r, id);
            if (check_error) {
                printf("I can`t push route\n");
                free(line);
                free(id);
                return -1;
            }
            ro = &r->ptr[r->size - 1];
        }
        free(id);
        Stop so = {.lat = lat, .lon = lon, .in = in, .out = out, .type = type};
        check_error = route_push(ro, so);
        if (check_error) {
            printf("I can`t push stop\n");
            free(line);
            return -1;
        }
    }
    if (line) {
        free(line);
    }
    
    return 0;
}

int stop_cmp(const void *a0, const void *b0) {
    const Stop *a = a0;
    const Stop *b = b0;
    tm ta = a->in;
    tm tb = b->in;
    double diff = difftime(mktime(&ta), mktime(&tb));
    return (diff > 0) - (diff < 0);
}

int route_filter(Route *r) {
    qsort(r->stops, r->size, sizeof(Stop), stop_cmp);
    return 0;
}

int routes_filter(Routes *r) {
    for (size_t i = 0; i < r->size; i++)
        route_filter(&r->ptr[i]);
    return 0;
}

int routes_find_ext_routes_cnt(Route **res, const Routes *r, bool max) {
    *res = NULL;
    int ext_cnt = max ? 0 : INT_MAX;
    for (size_t i = 0; i < r->size; i++) {
        Route *ro = &r->ptr[i];
        int cnt = 0;
        for (size_t j = 0; j < ro->size; j++) {
            if (ro->stops[j].type != 1)
                cnt++;
        }
        cnt = (cnt + 1) / 2;
        if ((cnt > ext_cnt) ^ !max) {
            *res = ro;
            ext_cnt = cnt;
        }
    }
    return 0;
}
int routes_find_ext_path_len(Route **res, const Routes *r, bool max) {
    *res = NULL;
    double ext_dur = max ? 0 : INFINITY;
    for (size_t i = 0; i < r->size; i++) {
        Route *a = &r->ptr[i];
        double dur = 0;
        for (size_t j = 0; j < a->size - 1; j++) {
            tm out = a->stops[j].out;
            tm in = a->stops[j + 1].in;
            dur += difftime(mktime(&in), mktime(&out));
        }
        if ((dur > ext_dur) ^ !max) {
            *res = a;
            ext_dur = dur;
        }
    }
    return 0;
}
int routes_find_ext_route_len(Route **res, const Routes *r, bool max) {
    *res = NULL;
    double ext_dur = max ? 0 : INFINITY;
    for (size_t i = 0; i < r->size; i++) {
        Route *a = &r->ptr[i];
        double dur = 0;
        for (size_t j = 0; j < a->size - 1; j++) {
            if (a->stops[j].type == StopStart)
                dur = 0;
            if (a->stops[j].type == StopEnd)
                break;
            tm out = a->stops[j].out;
            tm in = a->stops[j + 1].in;
            dur += difftime(mktime(&in), mktime(&out));
        }
        if ((dur > ext_dur) ^ !max) {
            *res = a;
            ext_dur = dur;
        }
    }
    return 0;
}

int routes_find_ext_stop_dur(Route **res, const Routes *r, bool max) {
    *res = NULL;
    double ext_dur = max ? 0 : INFINITY;
    for (size_t i = 0; i < r->size; i++) {
        Route *ro = &r->ptr[i];
        double dur = max ? 0 : INFINITY;
        for (size_t j = 0; j < ro->size; j++) {
            tm a = ro->stops[j].in;
            tm b = ro->stops[j].out;
            double d = difftime(mktime(&b), mktime(&a));
            if ((d > dur) ^ !max) {
                dur = d;
            }
        }
        if ((dur > ext_dur) ^ !max) {
            *res = ro;
            ext_dur = dur;
        }
    }
    return 0;
}
int routes_find_ext_stop_total_dur(Route **res, const Routes *r, bool max) {
    *res = NULL;
    double ext_dur = max ? 0 : INFINITY;
    for (size_t i = 0; i < r->size; i++) {
        Route *ro = &r->ptr[i];
        double dur = 0;
        for (size_t j = 0; j < ro->size; j++) {
            tm a = ro->stops[j].in;
            tm b = ro->stops[j].out;
            dur += difftime(mktime(&b), mktime(&a));
        }
        if ((dur > ext_dur) ^ !max) {
            *res = ro;
            ext_dur = dur;
        }
    }
    return 0;
}

typedef int (*handle)(Routes *r);

int for_print(Routes *r) {
    for (size_t i = 0; i < r->size; i++) {
        printf("[%zu]: ", i);
        print_routes(&r->ptr[i]);
    }
    return 0;
}

int for_most_routes(Routes *r) {
    Route *ro = NULL;
    routes_find_ext_routes_cnt(&ro, r, true);
    if (ro) {
        printf("%s\n", ro->id);
    } else {
        printf("no route found\n");
    }
    return 0;
}
int for_least_routes(Routes *r) {
    Route *ro = NULL;
    routes_find_ext_routes_cnt(&ro, r, false);
    if (ro) {
        printf("%s\n", ro->id);
    } else {
        printf("no route found\n");
    }
    return 0;
}
int for_longest_path(Routes *r) {
    Route *ro = NULL;
    routes_find_ext_path_len(&ro, r, true);
    if (ro) {
        printf("%s\n", ro->id);
    } else {
        printf("no route found\n");
    }
    return 0;
}
int for_shortest_path(Routes *r) {
    Route *ro = NULL;
    routes_find_ext_path_len(&ro, r, false);
    if (ro) {
        printf("%s\n", ro->id);
    } else {
        printf("no route found\n");
    }
    return 0;
}
int for_longest_route(Routes *r) {
    Route *ro = NULL;
    routes_find_ext_route_len(&ro, r, true);
    if (ro) {
        printf("%s\n", ro->id);
    } else {
        printf("no route found\n");
    }
    return 0;
}
int for_shortest_route(Routes *r) {
    Route *ro = NULL;
    routes_find_ext_route_len(&ro, r, false);
    if (ro) {
        printf("%s\n", ro->id);
    } else {
        printf("no route found\n");
    }
    return 0;
}
int for_longest_stop(Routes *r) {
    Route *ro = NULL;
    routes_find_ext_stop_dur(&ro, r, true);
    if (ro) {
        printf("%s\n", ro->id);
    } else {
        printf("no route found\n");
    }
    return 0;
}
int for_shortest_stop(Routes *r) {
    Route *ro = NULL;
    routes_find_ext_stop_dur(&ro, r, false);
    if (ro) {
        printf("%s\n", ro->id);
    } else {
        printf("no route found\n");
    }
    return 0;
}
int for_max_wait(Routes *r) {
    Route *ro = NULL;
    routes_find_ext_stop_total_dur(&ro, r, true);
    if (ro) {
        printf("%s\n", ro->id);
    } else {
        printf("no route found\n");
    }
    return 0;

}

int main(int argc, const char *argw[]) {

    if (argc < 2) {
        printf("Usage <%s> <file1> <file2> ...\n", argw[0]);
        return 1;
    }

    const char *flags[] = {"Print", "MostRoutes", "LeastRoutes",
    "LongestPath", "ShortestPath", "LongestRoute", "ShortestRoute",
    "LongestStop", "ShortestStop", "MaxWait"};

    const handle functions[] = {for_print, for_most_routes, 
    for_least_routes, for_longest_path, for_shortest_path,
    for_longest_route, for_shortest_route, for_longest_stop,
    for_shortest_stop, for_max_wait};

    Routes r = {0};
    int error_check = init_route(&r);
    if (error_check) {
        printf("I can`t init routes\n");
        return 1;
    }
    for (int i = 1; i < argc; i++) {
        FILE *f = fopen(argw[i], "r");
        if (!f) {
            printf("I can`t open file [%s]\n", argw[i]);
            return 1;
        }
        error_check = parse_files(&r, f);
        if (error_check) {
            printf("I can`t parse file [%s]\n", argw[i]);
            return 1;
        }
        fclose(f);
    }

    routes_filter(&r);
    printf("Hello, I can use next operations: \n");
    for (size_t i = 0; i < sizeof(flags) / sizeof(flags[0]); i++) {
        printf("%s ", flags[i]);
    }
    printf("\n");
    printf("\n");
    
    size_t line_len = 0;
    char *line = NULL;
    while (1) {
        int n = getline(&line, &line_len, stdin);
        
        if (n <= 0) {
            break;
        }
        n--;
        line[n] = 0;
        bool there_this_operation = false;
        for (size_t i = 0; i < sizeof(flags) / sizeof(flags[0]); i++) {
            if (strcmp(line, flags[i]) == 0) {
                functions[i](&r);
                there_this_operation = true;
            }
        }
        if (!there_this_operation) {
            printf("I don`t know this operation: [%s]\n", line);
            printf("Use only one from supported operations: ");
            for (size_t i = 0; i < sizeof(flags) / sizeof(flags[0]); i++) {
                printf("%s, ", flags[i]);
            }
            printf("\n");
        }
    }
    if (line)
        free(line);

    free_route(&r);
    return 0;
}