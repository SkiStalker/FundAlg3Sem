#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    int Number_of_bus;
    int Coordinates_of_station;
    char* Time_of_arrival;
    char* Time_of_departure;
    int is_start_prom_end;
    struct Node *next;
};

struct Node* Create(const int Number_of_bus, const int stationCoords, char* Time_of_arrival, char* Time_of_departure, const int is_start_prom_end) {
    struct Node* ptr = NULL;
    ptr = (struct Node*)malloc(sizeof(struct Node));
    ptr->Number_of_bus = Number_of_bus;
    ptr->Coordinates_of_station = stationCoords;
    ptr->Time_of_arrival = Time_of_arrival;
    ptr->Time_of_departure = Time_of_departure;
    ptr->is_start_prom_end = is_start_prom_end;
    ptr->next = NULL;
    return ptr;
}

void Insert(struct Node* pNode, struct Node* newNode) {
    newNode->next = pNode->next;
    pNode->next = newNode;
}

void PushFront(struct Node** pHead, struct Node* newNode) {
    newNode->next = *pHead;
    *pHead = newNode;
}

void delete(struct Node* pNode) {
    struct Node* ptr;
    ptr = pNode->next;
    pNode->next = ptr->next;
    free(ptr);
}

struct NodeMain {
    struct Node* BusHead;
    struct NodeMain *next;
};

struct NodeMain* CreateForMain(struct Node* BusHead) {
    struct NodeMain* ptr = NULL;
    ptr = (struct NodeMain*)malloc(sizeof(struct NodeMain));
    ptr->BusHead = BusHead;
    ptr->next = NULL;
    return ptr;
}

void InsertInMain(struct NodeMain* pNode, struct NodeMain* newNode) {
    newNode->next = pNode->next;
    pNode->next = newNode;
}

void PushFrontInMain(struct NodeMain** pHead, struct NodeMain* newNode) {
    newNode->next = *pHead;
    *pHead = newNode;
}

void deleteFromMain(struct NodeMain* pNode) {
    struct NodeMain* ptr;
    ptr = pNode->next;
    pNode->next = ptr->next;
    free(ptr);
}

int GetDuration (struct Node* bus) {
    int duration = 0, arrTime = 0, depTime = 0, razryad = 1;
    char *ar = bus->Time_of_arrival;
    while (*ar != '\n') {
        ar++;
    }
    ar--;
    while (*ar != ':') {
        arrTime += (*ar - '0') * razryad;
        razryad *= 10;
        ar--;
    }
    ar--;
    razryad = 1;
    while (*ar != ':') {
        arrTime += (*ar - '0') * 60 * razryad;
        razryad *= 10;
        ar--;
    }
    ar--; razryad = 1;
    while (*ar != ' ') {
        arrTime += (*ar - '0') * 3600 * razryad;
        razryad *= 10;
        ar--;
    }
    razryad = 1;

    char *dp = bus->Time_of_departure;
    while (*dp != '\n') {
        dp++;
    }
    dp--;
    while (*dp != ':') {
        depTime += (*dp - '0') * razryad;
        razryad *= 10;
        dp--;
    }
    razryad = 1; dp--;
    while (*dp != ':') {
        depTime += (*dp - '0') * 60 * razryad;
        razryad *= 10;
        dp--;
    }
    razryad = 1; dp--;
    while (*dp != ' ') {
        depTime += (*dp - '0') * 3600 * razryad;
        razryad *= 10;
        dp--;
    }
    duration = depTime - arrTime;
    return duration;
}

char *DurationToString(int duration) {
    char *durationString = (char*)malloc(20 * sizeof(char));
    if (durationString == NULL) {
        return NULL;
    }
    int hrs = duration / 3600, min = duration / 60 % 60, sec = duration % 60;
    sprintf(durationString, "%02d:%02d:%02d", hrs, min, sec);
    return durationString;
}

void StopDuration (struct NodeMain* MainHead, int fl) {
    int Number_of_bus, biggest_bus = 0, biggest_amount = -1, smallest_bus = 0, smallest_amount = 100000000, smallestSt = 0, biggestSt = 0;

    struct NodeMain* ptr = MainHead;
    while (ptr != NULL) {
        Number_of_bus = ptr->BusHead->Number_of_bus;
        struct Node* bus = ptr->BusHead;
        while (bus != NULL) {
            int duration = GetDuration(bus);
            if (duration > biggest_amount) {
                biggest_amount = duration;
                biggest_bus = Number_of_bus;
                biggestSt = bus->Coordinates_of_station;
            }
            if (duration < smallest_amount) {
                smallest_amount = duration;
                smallest_bus = Number_of_bus;
                smallestSt = bus->Coordinates_of_station;
            }
            bus = bus->next;
        }
        ptr = ptr->next;
    }

    if (fl) {
        char *durationString = DurationToString(biggest_amount);
        printf("Bus #%d had longest stop for %s on <%d> station\n\n", biggest_bus, durationString, biggestSt);
        free(durationString);
    } else {
        char *durationString = DurationToString(smallest_amount);
        printf("Bus #%d had shortest stop for %s on <%d> station\n\n", smallest_bus, durationString, smallestSt);
        free(durationString);
    }
}

int ValidateCommand(const char* command) {
    char c = '0';
    for (int i = 0; i <= 9; ++i) {
        char st[2] = "0\0";

        st[0] = c;
        if (strcmp(command, st) == 0) {
            return i;
        }
        c++;
    }
    return -1;
}

void AmountOfRoutes(struct NodeMain* MainHead, const int fl) {
    int Number_of_bus, amount = 0, biggest_bus = 0, biggest_amount = -1, smallest_bus = 0, smallest_amount = 100000000;
    struct NodeMain* ptr = MainHead;
    while (ptr != NULL) {
        Number_of_bus = ptr->BusHead->Number_of_bus;
        struct Node* bus = ptr->BusHead;
        while (bus != NULL) {
            if (bus->is_start_prom_end == 2) {
                amount++;
            }
            bus = bus->next;
        }
        if (amount > biggest_amount) {
            biggest_amount = amount;
            biggest_bus = Number_of_bus;
        }
        if (amount < smallest_amount) {
            smallest_amount = amount;
            smallest_bus = Number_of_bus;
        }
        amount = 0;
        ptr = ptr->next;
    }
    if (fl) {
        printf("Bus #%d traveled the most routes: %d\n\n", biggest_bus, biggest_amount);
    } else {
        printf("Bus #%d traveled the least routes: %d\n\n", smallest_bus, smallest_amount);
    }
}

void Path(struct NodeMain* MainHead, int fl) {
    int Number_of_bus, amount = 0, biggest_bus = 0, biggest_amount = -1, smallest_bus = 0, smallest_amount = 100000000;
    struct NodeMain* ptr = MainHead;
    while (ptr != NULL) {
        Number_of_bus = ptr->BusHead->Number_of_bus;
        struct Node* bus = ptr->BusHead;
        while (bus != NULL) {
            amount++;
            bus = bus->next;
        }
        if (amount > biggest_amount) {
            biggest_amount = amount;
            biggest_bus = Number_of_bus;
        }
        if (amount < smallest_amount) {
            smallest_amount = amount;
            smallest_bus = Number_of_bus;
        }
        amount = 0;
        ptr = ptr->next;
    }
    if (fl) {
        printf("Bus #%d traveled the biggest path: %d\n\n", biggest_bus, biggest_amount);
    } else {
        printf("Bus #%d traveled the smallest path: %d\n\n", smallest_bus, smallest_amount);
    }
}

void RouteLength (struct NodeMain* MainHead, int fl) {
    int Number_of_bus, amount = 0, biggest_bus = 0, biggest_amount = -1, smallest_bus = 0, smallest_amount = 100000000;
    struct NodeMain* ptr = MainHead;
    while (ptr != NULL) {
        Number_of_bus = ptr->BusHead->Number_of_bus;
        struct Node* bus = ptr->BusHead;
        while (bus != NULL) {
            if (bus->is_start_prom_end == 2) {
                amount++;
                if (amount > biggest_amount) {
                    biggest_amount = amount;
                    biggest_bus = Number_of_bus;
                }
                if (amount < smallest_amount) {
                    smallest_amount = amount;
                    smallest_bus = Number_of_bus;
                }
                amount = 0;
            } else {
                amount++;
            }
            bus = bus->next;
        }
        ptr = ptr->next;
    }
    if (fl) {
        printf("Bus #%d traveled the longest route: %d\n\n", biggest_bus, biggest_amount);
    } else {
        printf("Bus #%d traveled the shortest route: %d\n\n", smallest_bus, smallest_amount);
    }
}

void Print(struct NodeMain* MainHead) {
    struct NodeMain* ptr = MainHead;
    while (ptr != NULL) {
        printf("------------ <%d> Bus Route ----------\n", ptr->BusHead->Number_of_bus);
        struct Node* p = ptr->BusHead;
        while (p != NULL) {
            printf("Number_of_bus: %d\nSt.Coords: %d\nArrTime: %sDepTime: %sMarker: %d\n\n", p->Number_of_bus, p->Coordinates_of_station, p->Time_of_arrival, p->Time_of_departure, p->is_start_prom_end);
            p = p->next;
        }
        ptr = ptr->next;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("You must provide files.");
        return -1;
    }
    struct NodeMain* MainHead = NULL;

    for (int i = 1; i < argc; i++) {

        FILE* file = fopen(argv[i], "r");
        if (file == NULL) {
            printf("Error opening file: %s\n", argv[i]);
            return -1;
        }

        char stationCoords[256];
        fgets(stationCoords, sizeof(stationCoords), file);
        int Coordinates_of_station = atoi(stationCoords);

        char line[256];
        while (1) {
            char *Time_of_arrival = malloc(sizeof(char) * 256);
            char *Time_of_departure = malloc(sizeof(char) * 256);

            if (fgets(line, sizeof(line), file) == NULL) {
                break;
            }
            int busNum = atoi(line);

            fgets(line, sizeof(line), file);
            strcpy(Time_of_arrival, line);

            fgets(line, sizeof(line), file);
            strcpy(Time_of_departure, line);

            fgets(line, sizeof(line), file);
            int is_start_prom_end = atoi(line);

            struct Node* busStoppedNode = Create(busNum, Coordinates_of_station, Time_of_arrival, Time_of_departure, is_start_prom_end);
            if (MainHead == NULL) {
                struct Node* busHead = busStoppedNode;
                struct NodeMain* Bus = CreateForMain(busHead);
                MainHead = Bus;
            } else {
                int thisBus = 0;
                struct NodeMain* ptr = MainHead;
                while (ptr != NULL) {
                    if (ptr->BusHead->Number_of_bus == busNum) {
                        thisBus = 1;
                        if (strcmp(Time_of_arrival, ptr->BusHead->Time_of_arrival) < 0) {
                            PushFront(&ptr->BusHead, busStoppedNode);
                        } else {
                            struct Node* busPtr = ptr->BusHead;
                            while (busPtr->next != NULL) {
                                if (strcmp(Time_of_arrival, busPtr->next->Time_of_arrival) < 0) {
                                    break;
                                }
                                busPtr = busPtr->next;
                            }
                            Insert(busPtr, busStoppedNode);
                        }

                    }
                    ptr = ptr->next;
                }
                if (!thisBus) {
                    struct Node* busHead = busStoppedNode;
                    InsertInMain(MainHead, CreateForMain(busHead));
                }
            }
        }
    }
    printf("Choose command:\n"
           "1. Bus with the biggest amount of routes\n"
           "2. Bus with the smallest amount of routes\n"
           "3. Bus with longest path\n"
           "4. Bus with shortest path\n"
           "5. Bus with longest route\n"
           "6. Bus with shortest route\n"
           "7. Bus with longest stop\n"
           "8. Bus with shortest stop\n"
           "9. Exit\n\n");
    int fl = 1;
    while (fl) {
        char command[256];
        printf("Enter command: ");
        scanf("%s", command);
        const int com = ValidateCommand(command);
        switch (com) {
            case -1: printf("Invalid Command\n"); break;
            case 0: Print(MainHead); break;
            case 1: AmountOfRoutes(MainHead, 1); break;
            case 2: AmountOfRoutes(MainHead, 0); break;
            case 3: Path(MainHead, 1); break;
            case 4: Path(MainHead, 0); break;
            case 5: RouteLength(MainHead, 1); break;
            case 6: RouteLength(MainHead, 0); break;
            case 7: StopDuration(MainHead, 1); break;
            case 8: StopDuration(MainHead, 0); break;
            case 9: fl = 0; break;
            default: printf("No way\n");break;
        }
    }
    if (MainHead != NULL) {
        while (MainHead->next != NULL) {
            struct NodeMain* p = MainHead->next;
            struct Node* busHead = p->BusHead;
            while (busHead != NULL) {
                struct Node* next = busHead->next;
                free(busHead);
                busHead = next;
            }
            free(MainHead);
            MainHead = p;
        }
    }
    return 0;
}