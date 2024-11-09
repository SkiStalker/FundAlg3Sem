#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <limits.h>

#define EARTH_RADIUS_KM 6371.0


typedef struct {
    char vehicleId[50];
    char stopTime[20];
    char departureTime[20];
    double latitude;
    double longitude;
    char marker[15];
} Route;

typedef struct RouteNode {
    Route route;
    struct RouteNode* next;
} RouteNode;

typedef struct {
    RouteNode* head;
    RouteNode* tail;
} RouteList;

void sortRoutePointsByTime(RouteList* routeList, const char* vehicleId) {
    RouteNode* sortedList = NULL;
    RouteNode* current = routeList->head;

    while (current) {
        if (strcmp(current->route.vehicleId, vehicleId) == 0) {
            RouteNode* newNode = malloc(sizeof(RouteNode));
            newNode->route = current->route;
            newNode->next = NULL;

            if (!sortedList || strcmp(newNode->route.stopTime, sortedList->route.stopTime) < 0) {
                newNode->next = sortedList;
                sortedList = newNode;
            } else {
                RouteNode* prev = sortedList;
                while (prev->next && strcmp(prev->next->route.stopTime, newNode->route.stopTime) <= 0) {
                    prev = prev->next;
                }
                newNode->next = prev->next;
                prev->next = newNode;
            }
        }
        current = current->next;
    }

    printf("Маршрут транспортного средства %s по времени:\n", vehicleId);
    current = sortedList;
    while (current) {
        printf("Время: %s, Метка: %s\n", current->route.stopTime, current->route.marker);
        RouteNode* temp = current;
        current = current->next;
        free(temp);
    }
}

// Функция для добавления маршрута в список
void addRouteToList(RouteList* routeList, const char* vehicleId, const char* stopTime, const char* departureTime, double latitude, double longitude, const char* marker) {
    RouteNode* newNode = malloc(sizeof(RouteNode));
    strcpy(newNode->route.vehicleId, vehicleId);
    strcpy(newNode->route.stopTime, stopTime);
    strcpy(newNode->route.departureTime, departureTime);
    newNode->route.latitude = latitude;
    newNode->route.longitude = longitude;
    strcpy(newNode->route.marker, marker);
    newNode->next = NULL;

    if (routeList->tail) {
        routeList->tail->next = newNode;
    } else {
        routeList->head = newNode;
    }
    routeList->tail = newNode;
}

int readDataFromFile(RouteList* routeList, const char* filePath) {
    FILE* file = fopen(filePath, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    char line[256];
    double latitude = 0.0, longitude = 0.0;
    
    // Чтение первой строки с координатами
    if (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "Coordinates: %lf, %lf", &latitude, &longitude) != 2) {
            printf("Error parsing coordinates from line: %s\n", line);
            fclose(file);
            return -1;
        }
    }
    
    // Чтение данных для каждого автобуса
    while (fgets(line, sizeof(line), file)) {
        char vehicleId[50];
        char stopTime[20];
        char departureTime[20];
        char marker[15];

        // Читаем данные автобуса
        if (sscanf(line, "%[^;];%[^;];%[^;];%s", vehicleId, stopTime, departureTime, marker) != 4) {
            printf("Error parsing bus data from line: %s\n", line);
            continue;
        }
        
        // Добавляем маршрут в список
        addRouteToList(routeList, vehicleId, stopTime, departureTime, latitude, longitude, marker);
    }

    fclose(file);
    return 0;
}

// Формула Haversine для расчета расстояния
double haversine(double lat1, double lon1, double lat2, double lon2) {
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;
    
    lat1 = lat1 * M_PI / 180.0;
    lat2 = lat2 * M_PI / 180.0;

    double a = sin(dLat/2) * sin(dLat/2) +
               sin(dLon/2) * sin(dLon/2) * cos(lat1) * cos(lat2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    return EARTH_RADIUS_KM * c;
}

// Подсчёт количества маршрутов для транспортного средства
int countRoutes(RouteList* routeList, const char* vehicleId) {
    int count = 0;
    RouteNode* current = routeList->head;
    while (current) {
        if (strcmp(current->route.vehicleId, vehicleId) == 0) {
            count++;
        }
        current = current->next;
    }
    return count;
}

// Подсчет общего расстояния для транспортного средства
double calculateTotalDistance(RouteList* routeList, const char* vehicleId) {
    RouteNode* current = routeList->head;
    double totalDistance = 0.0;
    double prevLat = 0.0, prevLon = 0.0;
    int firstPoint = 1;

    while (current) {
        if (strcmp(current->route.vehicleId, vehicleId) == 0) {
            if (!firstPoint) {
                totalDistance += haversine(prevLat, prevLon, current->route.latitude, current->route.longitude);
            }
            prevLat = current->route.latitude;
            prevLon = current->route.longitude;
            firstPoint = 0;
        }
        current = current->next;
    }
    return totalDistance;
}


int strptime(const char* datetime, const char* format, struct tm* tm) {
    int ret;
    if (strcmp(format, "%d.%m.%Y %H:%M:%S") == 0) {
        // Разбор даты и времени
        ret = sscanf(datetime, "%d.%d.%d %d:%d:%d", 
            &tm->tm_mday, &tm->tm_mon, &tm->tm_year, 
            &tm->tm_hour, &tm->tm_min, &tm->tm_sec);
        if (ret == 6) {
            tm->tm_mon -= 1;  // Месяцы начинаются с 0
            tm->tm_year -= 1900;  // Год с 1900
            return 1;  // Успешно разобрали
        }
    }
    return 0;  // Ошибка
}
// Функция для подсчета длительности маршрута (в минутах)
int calculateRouteDuration(RouteList* routeList, const char* vehicleId) {
    RouteNode* current = routeList->head;
    struct tm startTm, endTm;
    int totalDuration = 0;
    int startTimeSet = 0;

    while (current) {
        if (strcmp(current->route.vehicleId, vehicleId) == 0) {
            if (!startTimeSet) {
                strptime(current->route.stopTime, "%d.%m.%Y %H:%M:%S", &startTm);
                startTimeSet = 1;
            }
            strptime(current->route.departureTime, "%d.%m.%Y %H:%M:%S", &endTm);
            totalDuration += (int)difftime(mktime(&endTm), mktime(&startTm)) / 60;
        }
        current = current->next;
    }
    return totalDuration;
}

// Поиск транспортного средства с наибольшим количеством маршрутов
void findVehicleWithMostRoutes(RouteList* routeList) {
    RouteNode* current = routeList->head;
    char mostRoutesVehicle[50];
    int maxRoutes = 0;

    while (current) {
        int routesCount = countRoutes(routeList, current->route.vehicleId);
        if (routesCount > maxRoutes) {
            maxRoutes = routesCount;
            strcpy(mostRoutesVehicle, current->route.vehicleId);
        }
        current = current->next;
    }

    printf("Транспортное средство с наибольшим количеством маршрутов: %s (Маршрутов: %d)\n", mostRoutesVehicle, maxRoutes);
}

// Поиск транспортного средства с наименьшим количеством маршрутов
void findVehicleWithLeastRoutes(RouteList* routeList) {
    RouteNode* current = routeList->head;
    char leastRoutesVehicle[50];
    int minRoutes = __INT_MAX__;

    while (current) {
        int routesCount = countRoutes(routeList, current->route.vehicleId);
        if (routesCount < minRoutes) {
            minRoutes = routesCount;
            strcpy(leastRoutesVehicle, current->route.vehicleId);
        }
        current = current->next;
    }

    printf("Транспортное средство с наименьшим количеством маршрутов: %s (Маршрутов: %d)\n", leastRoutesVehicle, minRoutes);
}

// Поиск транспортного средства с самым длинным пройденным путем
void findVehicleWithLongestDistance(RouteList* routeList) {
    RouteNode* current = routeList->head;
    char longestDistanceVehicle[50];
    double maxDistance = 0.0;

    while (current) {
        double distance = calculateTotalDistance(routeList, current->route.vehicleId);
        if (distance > maxDistance) {
            maxDistance = distance;
            strcpy(longestDistanceVehicle, current->route.vehicleId);
        }
        current = current->next;
    }

    printf("Транспортное средство с самым длинным пройденным путем: %s (Расстояние: %.2f км)\n", longestDistanceVehicle, maxDistance);
    //printf("Транспортное средство с самым длинным пройденным путем: %s\n", longestDistanceVehicle);
}

// Поиск транспортного средства с самым коротким пройденным путем
void findVehicleWithShortestDistance(RouteList* routeList) {
    RouteNode* current = routeList->head;
    char shortestDistanceVehicle[50];
    double minDistance = __DBL_MAX__;

    while (current) {
        double distance = calculateTotalDistance(routeList, current->route.vehicleId);
        if (distance < minDistance) {
            minDistance = distance;
            strcpy(shortestDistanceVehicle, current->route.vehicleId);
        }
        current = current->next;
    }

    printf("Транспортное средство с самым коротким пройденным путем: %s (Расстояние: %.2f км)\n", shortestDistanceVehicle, minDistance);
    //printf("Транспортное средство с самым коротким пройденным путем: %s\n", shortestDistanceVehicle);
}

// Поиск транспортного средства с самым длинным маршрутом
void findVehicleWithLongestRoute(RouteList* routeList) {
    RouteNode* current = routeList->head;
    char longestRouteVehicle[50];
    int maxDuration = 0;

    while (current) {
        int duration = calculateRouteDuration(routeList, current->route.vehicleId);
        if (duration > maxDuration) {
            maxDuration = duration;
            strcpy(longestRouteVehicle, current->route.vehicleId);
        }
        current = current->next;
    }

    // printf("Транспортное средство с самым длинным маршрутом: %s (Длительность: %d минут)\n", longestRouteVehicle, maxDuration);
    printf("Транспортное средство с самым длинным маршрутом: %s\n", longestRouteVehicle);
}

// Поиск транспортного средства с самым коротким маршрутом
void findVehicleWithShortestRoute(RouteList* routeList) {
    RouteNode* current = routeList->head;
    char shortestRouteVehicle[50];
    int minDuration = __INT_MAX__;

    while (current) {
        int duration = calculateRouteDuration(routeList, current->route.vehicleId);
        if (duration < minDuration) {
            minDuration = duration;
            strcpy(shortestRouteVehicle, current->route.vehicleId);
        }
        current = current->next;
    }

    // printf("Транспортное средство с самым коротким маршрутом: %s (Длительность: %d минут)\n", shortestRouteVehicle, minDuration);
    printf("Транспортное средство с самым коротким маршрутом: %s\n", shortestRouteVehicle);
}

// Поиск транспортного средства с самой долгой остановкой (время одной остановки)
void findVehicleWithLongestStop(RouteList* routeList) {
    RouteNode* current = routeList->head;
    char longestStopVehicle[50];
    int maxStopDuration = 0;

    while (current) {
        struct tm stopTm, depTm;
        strptime(current->route.stopTime, "%d.%m.%Y %H:%M:%S", &stopTm);
        strptime(current->route.departureTime, "%d.%m.%Y %H:%M:%S", &depTm);
        
        int stopDuration = (int)difftime(mktime(&depTm), mktime(&stopTm)); 

        if (stopDuration > maxStopDuration) {
            maxStopDuration = stopDuration;
            strcpy(longestStopVehicle, current->route.vehicleId);
        }
        current = current->next;
    }

    printf("Транспортное средство с самой долгой остановкой: %s (Время остановки: %d секунд)\n", longestStopVehicle, maxStopDuration);
}

void findVehicleWithShortestStop(RouteList* routeList) {
    RouteNode* current = routeList->head;
    char shortestStopVehicle[50];
    int minStopDuration = __INT_MAX__;

    while (current) {
        struct tm stopTm, depTm;
        strptime(current->route.stopTime, "%d.%m.%Y %H:%M:%S", &stopTm);
        strptime(current->route.departureTime, "%d.%m.%Y %H:%M:%S", &depTm);
        
        int stopDuration = (int)difftime(mktime(&depTm), mktime(&stopTm)); // Длительность одной остановки в секундах

        if (stopDuration < minStopDuration) {
            minStopDuration = stopDuration;
            strcpy(shortestStopVehicle, current->route.vehicleId);
        }
        current = current->next;
    }

    printf("Транспортное средство с самой короткой остановкой: %s (Время остановки: %d секунд)\n", shortestStopVehicle, minStopDuration);
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    char vehicleId[50];
    int totalIdleTime;
} VehicleIdleTime;

// Функция для поиска транспорта с самым длинным простоем
void findVehicleWithLongestIdleTime(RouteList* routeList) {
    RouteNode* current = routeList->head;
    VehicleIdleTime idleTimes[100];  // Массив для хранения простоев транспорта, предполагаем макс. 100 транспортов
    int idleTimeCount = 0;

    while (current) {
        int found = 0;
        
        // Ищем, если транспорт уже есть в массиве
        for (int i = 0; i < idleTimeCount; i++) {
            if (strcmp(idleTimes[i].vehicleId, current->route.vehicleId) == 0) {
                found = 1;

                struct tm stopTm, depTm;
                strptime(current->route.stopTime, "%d.%m.%Y %H:%M:%S", &stopTm);
                strptime(current->route.departureTime, "%d.%m.%Y %H:%M:%S", &depTm);
                
                // Добавляем время простоя к общему времени для текущего транспорта
                idleTimes[i].totalIdleTime += (int)difftime(mktime(&depTm), mktime(&stopTm));
                break;
            }
        }

        // Если транспорт не найден в массиве, добавляем его
        if (!found) {
            strcpy(idleTimes[idleTimeCount].vehicleId, current->route.vehicleId);
            struct tm stopTm, depTm;
            strptime(current->route.stopTime, "%d.%m.%Y %H:%M:%S", &stopTm);
            strptime(current->route.departureTime, "%d.%m.%Y %H:%M:%S", &depTm);
            
            idleTimes[idleTimeCount].totalIdleTime = (int)difftime(mktime(&depTm), mktime(&stopTm));
            idleTimeCount++;
        }

        current = current->next;
    }

    // Поиск транспортного средства с самым большим простоем
    int maxIdleTime = 0;
    char longestIdleTimeVehicle[50] = "";
    for (int i = 0; i < idleTimeCount; i++) {
        if (idleTimes[i].totalIdleTime > maxIdleTime) {
            maxIdleTime = idleTimes[i].totalIdleTime;
            strcpy(longestIdleTimeVehicle, idleTimes[i].vehicleId);
        }
    }

    printf("Транспортное средство с самым долгим временем простоя: %s (Общее время простоя: %d секунд)\n", longestIdleTimeVehicle, maxIdleTime);
}

int main(int argc, char *argv[]) {
    RouteList routeList = {0};
    int choice;
    char vehicleId[50];

    if (argc < 2) {
        printf("Использование: %s <файл1> <файл2> ... <файлN>\n", argv[0]);
        return 1;
    }

    // Чтение данных из файлов, указанных в командной строке
    for (int i = 1; i < argc; i++) {
        printf("Чтение данных из файла: %s\n", argv[i]);
        readDataFromFile(&routeList, argv[i]);
    }

    while (1) {
        printf("\nВыберите опцию:\n");
        printf("1 - Транспортное средство с наибольшим количеством маршрутов\n");
        printf("2 - Транспортное средство с наименьшим количеством маршрутов\n");
        printf("3 - Транспортное средство с самым длинным пройденным путем\n");
        printf("4 - Транспортное средство с самым коротким пройденным путем\n");
        printf("5 - Транспортное средство с самым длинным маршрутом\n");
        printf("6 - Транспортное средство с самым коротким маршрутом\n");
        printf("7 - Транспортное средство с самой долгой остановкой\n");
        printf("8 - Транспортное средство с самой короткой остановкой\n");
        printf("9 - Транспортное средство с самым долгим временем простоя\n");
        printf("10 - Вывести список маршрута для транспортного средства\n");
        printf("0 - Выход\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: findVehicleWithMostRoutes(&routeList); break;
            case 2: findVehicleWithLeastRoutes(&routeList); break;
            case 3: findVehicleWithLongestDistance(&routeList); break;
            case 4: findVehicleWithShortestDistance(&routeList); break;
            case 5: findVehicleWithLongestRoute(&routeList); break;
            case 6: findVehicleWithShortestRoute(&routeList); break;
            case 7: findVehicleWithLongestStop(&routeList); break;
            case 8: findVehicleWithShortestStop(&routeList); break;
            case 9: findVehicleWithLongestIdleTime(&routeList); break;
            case 10:
                printf("Введите ID транспортного средства: ");
                scanf("%s", vehicleId);
                sortRoutePointsByTime(&routeList, vehicleId);
                break;
            case 0: exit(0);
            default: printf("Неверный выбор, попробуйте снова.\n");
        }
    }

    return 0;
}
