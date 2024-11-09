#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Node {
    int BusNumber;
    int stationCoords;
    char* arrivalTime;
    char* departureTime;
    int marker;
    struct Node *next;
};

struct Node* Create(int BusNum, int stCoords, char* arrivalTime, char* departureTime, int marker) {
    struct Node* ptr = NULL;
    ptr = (struct Node*)malloc(sizeof(struct Node));
    ptr->BusNumber = BusNum;
    ptr->stationCoords = stCoords;
    ptr->arrivalTime = arrivalTime;
    ptr->departureTime = departureTime;
    ptr->marker = marker;
    ptr->next = NULL;
    return ptr;
}

int Atoi(const char* argv) {
    int res = 0;
    int i = 0;
    int neg_flag = 0;
    if (argv[0] == '-') {
        neg_flag = 1;
        i = 1;
    }
    while (argv[i] != '\0' && argv[i] != '\n') {
        res = res * 10 + argv[i] - '0';
        i++;
    }
    if (neg_flag) {
        res *= -1;
    }
    //printf("%d\n", res);
    return res;
}

void pushBehind(struct Node* pNode, struct Node* newNode) {
    newNode->next = pNode->next;
    pNode->next = newNode;
}

void pushFront(struct Node** pHead, struct Node* newNode) {
    newNode->next = *pHead;
    *pHead = newNode;
}

struct List {
    struct Node* BusHead;
    struct List *next;
};

struct List* CreateForMain(struct Node* BusHead) {
    struct List* ptr = NULL;
    ptr = (struct List*)malloc(sizeof(struct List));
    ptr->BusHead = BusHead;
    ptr->next = NULL;
    return ptr;
}

void pushList(struct List* pNode, struct List* newNode) {
    newNode->next = pNode->next;
    pNode->next = newNode;
}

int CheckCommand(char* command) {
    char c = '0';
    for (int i = 0; i <= 9; ++i) {
        char st[2] = "0\0";

        st[0] = c;
        //printf("%s", st);
        if (strcmp(command, st) == 0) {
            return i;
        }
        c++;
    }
    return -1;
}

void PathCnt(struct List* Head, int flag) {
    int bus_numb, cnt = 0, max_bus = 0, max_cnt = -1, min_bus = 0, min_cnt = 100000000;

    struct List* ptr = Head;
    while (ptr != NULL) {
        bus_numb = ptr->BusHead->BusNumber;
        //printf("BusNum = %d\n", BusNum);
        struct Node* bus = ptr->BusHead;
        while (bus != NULL) {
            if (bus->marker == 2) {
                cnt++;
            }
            bus = bus->next;
        }
        if (cnt > max_cnt) {
            max_cnt = cnt;
            max_bus = bus_numb;
        }
        if (cnt < min_cnt) {
            min_cnt = cnt;
            min_bus = bus_numb;
        }
        cnt = 0;
        ptr = ptr->next;
    }
    if (flag) {
        printf("Bus #%d traveled the most routes: %d\n\n", max_bus, max_cnt);
    } else {
        printf("Bus #%d traveled the least routes: %d\n\n", min_bus, min_cnt);
    }
}

void Path(struct List* MainHead, int flag) {
    int bus_numb, len = 0, max_bus = 0, max_len = -1, min_bus = 0, min_len = 100000000;

    struct List* ptr = MainHead;
    while (ptr != NULL) {
        bus_numb = ptr->BusHead->BusNumber;
        struct Node* bus = ptr->BusHead;
        while (bus != NULL) {
            len++;
            bus = bus->next;
        }
        if (len > max_len) {
            max_len = len;
            max_bus = bus_numb;
        }
        if (len < min_len) {
            min_len = len;
            min_bus = bus_numb;
        }
        len = 0;
        ptr = ptr->next;
    }
    if (flag) {
        printf("Bus #%d traveled the biggest path: %d\n\n", max_bus, max_len);
    } else {
        printf("Bus #%d traveled the smallest path: %d\n\n", min_bus, min_len);
    }
}

void PathLen (struct List* MainHead, int flag) {
    int bus_numb, len = 0, max_bus = 0, max_len = -1, min_bus = 0, min_len = 100000000;
    struct List* ptr = MainHead;
    while (ptr != NULL) {
        bus_numb = ptr->BusHead->BusNumber;
        struct Node* bus = ptr->BusHead;
        while (bus != NULL) {
            if (bus->marker == 2) {
                len++;
                if (len > max_len) {
                    max_len = len;
                    max_bus = bus_numb;
                }
                if (len < min_len) {
                    min_len = len;
                    min_bus = bus_numb;
                }
                len = 0;
            } else {
                len++;
            }
            bus = bus->next;
        }

        ptr = ptr->next;
    }
    if (flag) {
        printf("Bus #%d traveled the longest route: %d\n\n", max_bus, max_len);
    } else {
        printf("Bus #%d traveled the shortest route: %d\n\n", min_bus, min_len);
    }
}

int GetParkingTime (struct Node* bus) {
    int parking_time = 0, arrTime = 0, depTime = 0, razr = 1;

    char *ar = bus->arrivalTime;
    while (*ar != '\n') {
        ar++;
    }
    ar--;
    while (*ar != ':') {
        arrTime += (*ar - '0') * razr;
        razr *= 10;
        ar--;
    }
    ar--;
    razr = 1;
    while (*ar != ':') {
        arrTime += (*ar - '0') * 60 * razr;
        razr *= 10;
        ar--;
    }
    ar--; razr = 1;
    while (*ar != ' ') {
        arrTime += (*ar - '0') * 3600 * razr;
        razr *= 10;
        ar--;
    }
    razr = 1;

    char *dp = bus->departureTime;
    while (*dp != '\n') {
        dp++;
    }
    dp--;
    while (*dp != ':') {
        depTime += (*dp - '0') * razr;
        razr *= 10;
        dp--;
    }
    razr = 1; dp--;
    while (*dp != ':') {
        depTime += (*dp - '0') * 60 * razr;
        razr *= 10;
        dp--;
    }
    razr = 1; dp--;
    while (*dp != ' ') {
        depTime += (*dp - '0') * 3600 * razr;
        razr *= 10;
        dp--;
    }
    parking_time = depTime - arrTime;
    return parking_time;
}

char *SecToStr(int dur) {
    char *ans = (char*)malloc(20 * sizeof(char));
    if (ans == NULL) {
        return NULL;
    }
    int h, min, sec;
    h = dur / 3600;
    min = (dur / 60) % 60;
    sec = dur % 60;

    sprintf(ans, "%02d:%02d:%02d", h, min, sec);
    return ans;
}

void maxParkingTime (struct List* MainHead, int flag) {
    int bus_numb, max_bus = 0, max_time = -1, min_bus = 0, min_time = 100000000, min_st = 0, max_st = 0;

    struct List* ptr = MainHead;
    while (ptr != NULL) {
        bus_numb = ptr->BusHead->BusNumber;
        struct Node* bus = ptr->BusHead;
        while (bus != NULL) {
            int dur = GetParkingTime(bus);
            if (dur > max_time) {
                max_time = dur;
                max_bus = bus_numb;
                max_st = bus->stationCoords;
            }
            if (dur < min_time) {
                min_time = dur;
                min_bus = bus_numb;
                min_st = bus->stationCoords;
            }
            bus = bus->next;
        }
        ptr = ptr->next;
    }

    if (flag) {
        char *durString = SecToStr(max_time);
        printf("Bus #%d had longest stop for %s on <%d> station\n\n", max_bus, durString, max_st);
        free(durString);
    } else {
        char *durString = SecToStr(min_time);
        printf("Bus #%d had shortest stop for %s on <%d> station\n\n", min_bus, durString, min_st);
        free(durString);
    }
}

void Print(struct List* Head) {
    struct List* ptr = Head;
    while (ptr != NULL) {
        printf("------------- <%d> Bus Route -----------\n", ptr->BusHead->BusNumber);
        struct Node* p = ptr->BusHead;
        while (p != NULL) {
            printf("BusNum: %d\nSt.Coords: %d\nArrTime: %sDepTime: %sMarker: %d\n\n", p->BusNumber, p->stationCoords, p->arrivalTime, p->departureTime, p->marker);
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
    struct List* Head = NULL;

    for (int i = 1; i < argc; i++) {

        FILE* file = fopen(argv[i], "r");
        if (file == NULL) {
            printf("Error opening file: %s\n", argv[i]);
            return -1;
        }

        char stCoords[256];
        fgets(stCoords, sizeof(stCoords), file);
        int stationCoords = Atoi(stCoords);

        char line[256];
        while (1) {
            char *arrTime = (char*)malloc(sizeof(char) * 32);
            char *depTime = (char*)malloc(sizeof(char) * 32);

            if (fgets(line, sizeof(line), file) == NULL) {
                break;
            }
            int busNum = Atoi(line);

            fgets(line, sizeof(line), file);
            strcpy(arrTime, line);

            fgets(line, sizeof(line), file);
            strcpy(depTime, line);

            fgets(line, sizeof(line), file);
            int marker = Atoi(line);

            struct Node* busStoppedNode = Create(busNum, stationCoords, arrTime, depTime, marker);
            if (Head == NULL) {
                struct Node* busHead = busStoppedNode;
                struct List* Bus = CreateForMain(busHead);
                Head = Bus;
            } else {
                int thisBus = 0;
                struct List* ptr = Head;
                while (ptr != NULL) {
                    if (ptr->BusHead->BusNumber == busNum) {
                        thisBus = 1;
                        if (strcmp(arrTime, ptr->BusHead->arrivalTime) < 0) {
                            pushFront(&ptr->BusHead, busStoppedNode);
                        } else {
                            struct Node* busPtr = ptr->BusHead;
                            while (busPtr->next != NULL) {
                                if (strcmp(arrTime, busPtr->next->arrivalTime) < 0) {
                                    break;
                                }
                                busPtr = busPtr->next;
                            }
                            pushBehind(busPtr, busStoppedNode);
                        }

                    }
                    ptr = ptr->next;
                }
                if (!thisBus) {
                    struct Node* busHead = busStoppedNode;
                    pushList(Head, CreateForMain(busHead));
                }
            }
        }
    }

    printf("Available commands:\n"
           "1. Bus with the biggest amount of routes\n"
           "2. Bus with the smallest amount of routes\n"
           "3. Bus with longest path\n"
           "4. Bus with shortest path\n"
           "5. Bus with longest route\n"
           "6. Bus with shortest route\n"
           "7. Bus with longest stop\n"
           "8. Bus with shortest stop\n"
           "9. Exit\n\n");

    int flag = 1;
    while (flag) {
        char command[256];
        printf("Enter command:\n");
        scanf("%s", command);
        int com = CheckCommand(command);
//        printf("Command: %d\n", com);
        switch (com) {
            case 0: {
                Print(Head);
                break;
            }
            case 1: {
                PathCnt(Head, 1);
                break;
            }
            case 2: {
                PathCnt(Head, 0);
                break;
            }
            case 3: {
                Path(Head, 1);
                break;
            }
            case 4: {
                Path(Head, 0);
                break;
            }
            case 5: {
                PathLen(Head, 1);
                break;
            }
            case 6: {
                PathLen(Head, 0);
                break;
            }
            case 7: {
                maxParkingTime(Head, 1);
                break;
            }
            case 8: {
                maxParkingTime(Head, 0);
                break;
            }
            case 9: {
                flag = 0;
                break;
            }
            default: printf("No such command.\n");break;
        }
    }

    if (Head != NULL){
        while (Head->next != NULL) {
            struct List *p = Head->next;
            struct Node *busHead = p->BusHead;
            while (busHead != NULL) {
                struct Node *next = busHead->next;
                free(busHead);
                busHead = next;
            }
            free(Head);
            Head = p;
        }
    }

    return 0;
}