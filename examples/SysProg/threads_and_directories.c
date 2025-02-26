#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

void listdir(const char *name, int indent)
{
    DIR *dir;
    struct dirent *entry = NULL;

    if (!(dir = opendir(name)))
        return;

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == 4)
        {

            char path[1024];

            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            printf("%*s[%s]\n", indent, "", entry->d_name);
            listdir(path, indent + 2); // enter to recursion
        }
        else
        {
            printf("%*s- %s\n", indent, "", entry->d_name);
        }
    }
    closedir(dir);
}

void *handle(void *arg)
{
    printf("Thread %lu started\n", pthread_self());
    listdir((char *)arg, 0);
    printf("Thread %lu stopped\n", pthread_self());
    return (void*)pthread_self();
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("No path presented");
        return -1;
    }

    pthread_t *thr_ids = (pthread_t *)malloc((argc - 1) * sizeof(pthread_t));

    size_t thr_cnt = 0;

    for (int i = 1; i < argc; ++i)
    {
        pthread_t thr_id = 0;

        if (pthread_create(&thr_id, NULL, &handle, (void *)argv[i]) == -1)
        {

            printf("Thread creation error");
            continue;
        }

#ifdef DETACH
        if (pthread_detach(thr_id) == -1)
        {
            printf("Thread %lu can't be detached\n", thr_id);
            pthread_cancel(thr_id);
            continue;
        }
#endif

        thr_cnt++;

        thr_ids[thr_cnt - 1] = thr_id;
    }

#ifdef DETACH
    scanf("Pause enter any key for continue\n");
#endif

    for (int i = 0; i < thr_cnt; ++i)
    {
#ifdef DETACH
        /* !!!Практического смысла не имеет!!!! */
        if (pthread_cancel(thr_ids[i]) == -1)
        {
            printf("Error on joining %lu thread", thr_ids[i]);
        }
#else
        pthread_t joined_thr_id = 0;
        if (pthread_join(thr_ids[i], (void*)&joined_thr_id) == -1)
        {
            printf("Error on joining %lu thread", thr_ids[i]);
        }
        else
        {
            printf("Thread %lu joined\n", joined_thr_id);
        }
#endif
    }
    free(thr_ids);
    return 0;
}