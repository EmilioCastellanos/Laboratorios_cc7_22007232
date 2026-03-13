#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "log_processor.h"

extern char lines[10000][256];

int ip_exists(ThreadData *data, char *ip)
{

    for (int i = 0; i < data->ip_count; i++)
    {
        if (strcmp(data->ips[i], ip) == 0)
        {
            return 1;
        }
    }

    return 0;
}

int find_url(ThreadData *data, char *url)
{

    for (int i = 0; i < data->url_count; i++)
    {
        if (strcmp(data->urls[i], url) == 0)
        {
            return i;
        }
    }

    return -1;
}

void *process_logs(void *arg)
{

    ThreadData *data = (ThreadData *)arg;

    char ip[50];
    char method[10];
    char url[100];
    int status;

    for (int i = data->start; i < data->end; i++)
    {

        sscanf(lines[i], "%s - - [%*[^]]] \"%s %[^\"]\"", ip, method, url);


        char *ptr = strrchr(lines[i], ' ');
        status = atoi(ptr);

        if (status >= 400 && status <= 599)
        {
            data->error_count++;
        }

        if (!ip_exists(data, ip))
        {
            strcpy(data->ips[data->ip_count], ip);
            data->ip_count++;
        }

        int idx = find_url(data, url);

        if (idx == -1)
        {

            strcpy(data->urls[data->url_count], url);
            data->url_counts[data->url_count] = 1;
            data->url_count++;
        }
        else
        {

            data->url_counts[idx]++;
        }
        // printf("Thread %d -> %s %s %d\n", data->start, ip, url, status);

    }

    return NULL;
}