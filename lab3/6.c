#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>

#define EPS 0.00000001

enum Errors {
    ERROR = -1,
    SUCCESS,
};


typedef struct Stop {
    float x;
    float y;
    char stopTime[20];
    char leaveTime[20];
    int isInitial;
    struct Stop* next;
} Stop;

typedef struct Route {
    char vehicleNumber[20];
    Stop* stops;             
    struct Route* next;
} Route;


typedef struct RouteList {
    Route* head;
} RouteList;

long convertToUnixTime(const char* datetime) {
    struct tm tm_time = {0};
    int day, month, year, hour, minute, second;

    if (sscanf(datetime, "%d.%d.%d %d:%d:%d", &day, &month, &year, &hour, &minute, &second) != 6) {
        return ERROR;
    }

    tm_time.tm_mday = day;
    tm_time.tm_mon = month - 1;
    tm_time.tm_year = year - 1900;
    tm_time.tm_hour = hour;
    tm_time.tm_min = minute;
    tm_time.tm_sec = second;

    return mktime(&tm_time);
}

Stop* createStop(float x, float y, char* stopTime, char* leaveTime, int isInitial) {
    Stop* newStop = (Stop*)malloc(sizeof(Stop));
    strcpy(newStop->stopTime, stopTime);
    strcpy(newStop->leaveTime, leaveTime);
    newStop->x = x;
    newStop->y = y;
    newStop->isInitial = isInitial;
    newStop->next = NULL;
    return newStop;
}

int strAddStr(char* str1, const char* str2) {
    while (*str1 != '\0') {
        str1++;
    }
    *str1++ = ' ';
    
    while (*str2 != '\0') {
        *str1++ = *str2++;
    }
    
    *str1 = '\0';

    return SUCCESS;
}

int addStopToRoute(Route* route, Stop* stop) {
    if (route->stops == NULL) {
        route->stops = stop;
    } else {
        Stop* temp = route->stops;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = stop;
    }
    return SUCCESS;
}

Route* createRoute(char* vehicleNumber) {
    Route* newRoute = (Route*)malloc(sizeof(Route));
    strcpy(newRoute->vehicleNumber, vehicleNumber);
    newRoute->stops = NULL;
    newRoute->next = NULL;
    return newRoute;
}


int addRouteToList(RouteList* list, Route* route) {
    if (list->head == NULL) {
        list->head = route;
    } else {
        Route* temp = list->head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = route;
    }
    return SUCCESS;
}


void readFile(RouteList* list, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Не удалось открыть файл %s\n", filename);
        return;
    }

    char vehicleNumber[20];
    char stopTime[20], leaveTime[20], time1[10], time2[10];
    int isInitial;
    float x, y;
    fscanf(file, "%f %f", &x, &y);


    while (fscanf(file, "%s %s %s %s %s %d", vehicleNumber, stopTime, time1, leaveTime, time2, &isInitial) != EOF) {
        strAddStr(stopTime, time1);
        strAddStr(leaveTime, time2);
        Route* route = list->head;
        while (route != NULL && strcmp(route->vehicleNumber, vehicleNumber) != 0) {
            route = route->next;
        }


        if (route == NULL) {
            route = createRoute(vehicleNumber);
            addRouteToList(list, route);
        }

        Stop* stop = createStop(x, y, stopTime, leaveTime, isInitial);
        addStopToRoute(route, stop);
    }

    fclose(file);
}


int compareTime(const char* time1, const char* time2) {
    return strcmp(time1, time2);
}

int sortStops(Route* route) {
    if (route->stops == NULL) return ERROR;

    Stop* sorted = NULL;

    Stop* current = route->stops;
    while (current != NULL) {
        Stop* next = current->next;

        if (sorted == NULL || compareTime(current->stopTime, sorted->stopTime) < 0) {
            current->next = sorted;
            sorted = current;
        } else {
            Stop* temp = sorted;
            while (temp->next != NULL && compareTime(temp->next->stopTime, current->stopTime) < 0) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }

        current = next;
        
    }

    route->stops = sorted;
    return SUCCESS;
}

float CalcWay(Stop* s1, Stop* s2) {
    float x1 = s1->x;
    float y1 = s1->y;
    float x2 = s2->x;
    float y2 = s2->y;
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));

}

Route* findMaxRoute(RouteList* list) {
    Route* max = NULL;
    int maxStops = 0;

    Route* route = list->head;
    while (route != NULL) {
        int startCount = 0;
        Stop* stop = route->stops;
        while (stop != NULL) {
            if (stop->isInitial == 1) {
                startCount++;
            }
            stop = stop->next;
        }

        if (startCount > maxStops) {
            maxStops = startCount;
            max = route;
        }

        route = route->next;
    }

    return max;
}

Route* findMinRoute(RouteList* list) {
    Route* max = NULL;
    int minStops = INT_MAX;

    Route* route = list->head;
    while (route != NULL) {
        int startCount = 0;
        Stop* stop = route->stops;
        while (stop != NULL) {
            if (stop->isInitial == 1) {
                startCount++;
            }
            stop = stop->next;
        }

        if (startCount < minStops) {
            minStops = startCount;
            max = route;
        }

        route = route->next;
    }

    return max;
}

Route* findLongestRoute(RouteList* list) {
    Route* longest = NULL;
    int maxStops = 0;

    Route* route = list->head;
    while (route != NULL) {
        int stopCount = 0;
        Stop* stop = route->stops;
        while (stop != NULL) {
            stopCount++;
            stop = stop->next;
        }

        if (stopCount > maxStops) {
            maxStops = stopCount;
            longest = route;
        }

        route = route->next;
    }

    return longest;
}

Route* findShortestRoute(RouteList* list) {
    Route* shortest = NULL;
    int minStops = INT_MAX;

    Route* route = list->head;
    while (route != NULL) {
        int stopCount = 0;
        Stop* stop = route->stops;
        while (stop != NULL) {
            stopCount++;
            stop = stop->next;
        }

        if (stopCount < minStops) {
            minStops = stopCount;
            shortest = route;
        }

        route = route->next;
    }

    return shortest;
}

Route* findShortestWay(RouteList* list) {
    Route* shortest = NULL;
    float minWay = INT_MAX;

    Route* route = list->head;
    while (route != NULL) {
        float way = 0;
        Stop* stop1 = route->stops;
        if (stop1 != NULL && stop1->next != NULL) {
            Stop* stop2 = stop1->next;

            while (stop1 != NULL && stop2 != NULL) {
                way += CalcWay(stop1, stop2);
                stop1 = stop2;
                stop2 = stop2->next;
            }
        }
        if ((way - minWay) < EPS) {
            minWay = way;
            shortest = route;
        }
        route = route->next;
    }

    return shortest;
}


Route* findLongestWay(RouteList* list) {
    Route* longest = NULL;
    float maxWay = -1;

    Route* route = list->head;
    while (route != NULL) {
        float way = 0;
        Stop* stop1 = route->stops;
        if (stop1 != NULL && stop1->next != NULL) {
            Stop* stop2 = stop1->next;

            while (stop2 != NULL) { 
                way += CalcWay(stop1, stop2);
                stop1 = stop2;
                stop2 = stop2->next;
            }
        }
        if ((way - maxWay) >EPS) { 
            maxWay = way;
            longest = route;
        }
        route = route->next;
    }

    return longest;
}

Route* findRouteWithLongestStop(RouteList* list) {
    Route* longestStopRoute = NULL;
    float maxStopDuration = -1;

    Route* route = list->head;
    while (route != NULL) {
        Stop* stop = route->stops;
        while (stop != NULL) {
            long stopTime = convertToUnixTime(stop->stopTime);
            long leaveTime = convertToUnixTime(stop->leaveTime);
            float duration = leaveTime - stopTime;
            if ((duration - maxStopDuration) > EPS) {
                maxStopDuration = duration;
                longestStopRoute = route;
            }

            stop = stop->next;
        }
        route = route->next;
    }

    return longestStopRoute;
}

Route* findRouteWithShortestStop(RouteList* list) {
    Route* shortestStopRoute = NULL;
    float minStopDuration = INT_MAX;

    Route* route = list->head;
    while (route != NULL) {
        Stop* stop = route->stops;
        while (stop != NULL) {
            long stopTime = convertToUnixTime(stop->stopTime);
            long leaveTime = convertToUnixTime(stop->leaveTime);
            if (stopTime == -1 || leaveTime == -1) {
                stop = stop->next;
                continue;
            }

            float duration = (float)(leaveTime - stopTime);
            if ((duration - minStopDuration) < EPS) {
                minStopDuration = duration;
                shortestStopRoute = route;
            }

            stop = stop->next;
        }
        route = route->next;
    }

    return shortestStopRoute;
}

Route* findRouteWithLongestStopTime(RouteList* list) {
    Route* longestStopRoute = NULL;
    float maxStopDuration = -1;

    Route* route = list->head;
    while (route != NULL) {
        Stop* stop = route->stops;
        float sumStop = 0.0;
        
        while (stop != NULL) {
            long stopTime = convertToUnixTime(stop->stopTime);
            long leaveTime = convertToUnixTime(stop->leaveTime);
            
            if (stopTime == -1 || leaveTime == -1) {
                fprintf(stderr, "Ошибка преобразования времени для остановки\n");
                stop = stop->next;
                continue;
            }
            
            float duration = (float)(leaveTime - stopTime);
            sumStop += duration;

            stop = stop->next;
        }

        if (sumStop > maxStopDuration) {
            maxStopDuration = sumStop; 
            longestStopRoute = route;
        }
        route = route->next;
    }

    return longestStopRoute;
}


int printRouteList(RouteList* r) {
    Route* currentRoute = r->head;
    while (currentRoute != NULL) {
        char* vehicle = currentRoute->vehicleNumber;
        printf("Номер машины: %s\n", vehicle);
        
        Stop* stop = currentRoute->stops;
        while (stop != NULL) {
            printf("Х:%.2f Y:%.2f Время остановки: %s Время отправления: %s Маркер: %d\n", stop->x, stop->y, stop->stopTime, stop->leaveTime, stop->isInitial);
            stop = stop->next;
        }
        
        currentRoute = currentRoute->next;
    }
    
    return SUCCESS;
}

int freeRouteList(RouteList* list) {
    Route* currentRoute = list->head;
    while (currentRoute != NULL) {
        Route* tempRoute = currentRoute;
        currentRoute = currentRoute->next;

        Stop* currentStop = tempRoute->stops;
        while (currentStop != NULL) {
            Stop* tempStop = currentStop;
            currentStop = currentStop->next;
            free(tempStop); 
        }

        free(tempRoute);
    }
    
    list->head = NULL;
    return SUCCESS;
}

int proccesFile(RouteList* list, int count, ...) {
    va_list args;
    va_start(args, count);
    for(int i = 0; i < count; i++) {
        char* file = va_arg(args, char*);
        readFile(list, file);
    }
    va_end(args);
    return SUCCESS;

}


int main() {
    RouteList list = {NULL};

    proccesFile(&list, 3, "task6_test/stop1.txt", "task6_test/stop2.txt", "task6_test/stop3.txt");

    
    Route* route = list.head;
    while (route != NULL) {
        sortStops(route);
        route = route->next;
    }
    printRouteList(&list);


    char stop[15] = "stop";
    char way[15] = "way";
    char r[15] = "route";
    char stopTime[15] = "stopTime";
    char stay[15] = "stay";
    char exitProg[15] = "exit";


    while (1) {
        printf("----------------------------------\n");
        printf("stop - вывести машину, которая проехала больше/меньше всего остановок\n");
        printf("way - вывести машину, которая проехала самый длинный/короткий путь\n");
        printf("route - вывести машину, у которой больше/меньше всего маршрутов\n");
        printf("stopTime - вывести машину с самой долгой/короткой остановкой\n");
        printf("stay - вывести машину c самым большим временем простоя\n");
        printf("exit - завершить программу\n");
        printf("----------------------------------\n");
        char command[20];
        scanf("%s", command);

        if (!strcmp(command, stop)) {
            Route* longestRoute = findLongestRoute(&list);
            if (longestRoute != NULL) {
                printf("Машина с наибольшим количеством остановок: %s\n", longestRoute->vehicleNumber);
            } else {
                return ERROR;
            }

            Route* shortestRoute = findShortestRoute(&list);
            if (shortestRoute != NULL) {
                printf("Машина с наименьшим количеством остановок: %s\n", shortestRoute->vehicleNumber);
            } else {
                return ERROR;
            }
        } else if (!strcmp(command, way)) {
            Route* minWay = findShortestWay(&list);
            if (minWay != NULL) {
                printf("Машина с кротчайшим путем: %s\n", minWay->vehicleNumber);
            } else {
                return ERROR;
            }

            Route* maxWay = findLongestWay(&list);
            if (maxWay != NULL) {
                printf("Машина с длинейшим путем: %s\n", maxWay->vehicleNumber);
            } else {
                return ERROR;
            }
        } else if (!strcmp(command, r)) {
            Route* maxRoute = findMaxRoute(&list);
            if (maxRoute != NULL) {
                printf("Машина с наибольшим количеством маршрутов: %s\n", maxRoute->vehicleNumber);
            } else {
                return ERROR;
            }

            Route* minRoute = findMinRoute(&list);
            if (minRoute != NULL) {
                printf("Машина с наименьшим количеством маршрутов: %s\n", minRoute->vehicleNumber);
            } else {
                return ERROR;
            }
        } else if (!strcmp(command, stopTime)) {
            Route* longestStopRoute = findRouteWithLongestStop(&list);
            if (longestStopRoute != NULL) {
                printf("Машина с самой длинной остановкой: %s\n", longestStopRoute->vehicleNumber);
            } else {
                return ERROR;
            }

            Route* shortestStopRoute = findRouteWithShortestStop(&list);
            if (shortestStopRoute != NULL) {
                printf("Машина с самой короткой остановкой: %s\n", shortestStopRoute->vehicleNumber);
            } else {
                return ERROR;
            }
        } else if (!strcmp(command, stay)) {
            Route* longestStopTime = findRouteWithLongestStopTime(&list);
            if (longestStopTime != NULL) {
                printf("Машина с самым большим временем простоя: %s\n", longestStopTime->vehicleNumber);
            } else {
                return ERROR;
            }
        } else if (!strcmp(command, exitProg)) {
            freeRouteList(&list);
            break;
        } else {
            printf("Неизвестная команда. Пожалуйста, попробуйте снова.\n");
            return ERROR;
        }
    }
    return SUCCESS;
}
