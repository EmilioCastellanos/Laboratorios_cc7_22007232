
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
#include "log_processor.h"

#define MAX_LINES 10000
#define THREADS 4
#define MAX_IPS 1000
#define MAX_URLS 1000

char global_ips[MAX_IPS][50];
int global_ip_count = 0;

char global_urls[MAX_URLS][100];
int global_url_counts[MAX_URLS];
int global_url_count = 0;

char lines[MAX_LINES][256];

int ip_exists_global(char ips[][50], int count, char *ip)
{

    for (int i = 0; i < count; i++)
    {
        if (strcmp(ips[i], ip) == 0)
        {
            return 1;
        }
    }

    return 0;
}

int find_global_url(char urls[][100], int count, char *url)
{

    for (int i = 0; i < count; i++)
    {
        if (strcmp(urls[i], url) == 0)
        {
            return i;
        }
    }

    return -1;
}


int main(int argc, char *argv[])
{
    int use_threads = 1;
    if (argc > 1) {
        if (strcmp(argv[1], "single") == 0) {
            use_threads = 0;
        } else if (strcmp(argv[1], "multi") == 0) {
            use_threads = 1;
        } else {
            printf("Uso: %s [single|multi]\n", argv[0]);
            return 1;
        }
    }

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    FILE *file = fopen("access.log", "r");
    if (!file)
    {
        printf("Error opening log\n");
        return 1;
    }
    int total_lines = 0;

    while (fgets(lines[total_lines], 256, file))
    {
        total_lines++;
    }

    fclose(file);


    int total_errors = 0;
    if (use_threads) {
        pthread_t threads[THREADS];
        ThreadData data[THREADS];
        int chunk = total_lines / THREADS;
        for (int i = 0; i < THREADS; i++) {
            data[i].start = i * chunk;
            data[i].end = (i == THREADS - 1) ? total_lines : (i + 1) * chunk;
            data[i].error_count = 0;
            data[i].ip_count = 0;
            data[i].url_count = 0;
            printf("Thread %d -> start %d end %d\n", i, data[i].start, data[i].end);
            pthread_create(&threads[i], NULL, process_logs, &data[i]);
        }
        // Recuentos
        for (int t = 0; t < THREADS; t++) {
            pthread_join(threads[t], NULL);
            total_errors += data[t].error_count;
            for (int i = 0; i < data[t].ip_count; i++) {
                if (!ip_exists_global(global_ips, global_ip_count, data[t].ips[i])) {
                    strcpy(global_ips[global_ip_count], data[t].ips[i]);
                    global_ip_count++;
                }
            }
            for (int i = 0; i < data[t].url_count; i++) {
                int idx = find_global_url(global_urls, global_url_count, data[t].urls[i]);
                if (idx == -1) {
                    strcpy(global_urls[global_url_count], data[t].urls[i]);
                    global_url_counts[global_url_count] = data[t].url_counts[i];
                    global_url_count++;
                } else {
                    global_url_counts[idx] += data[t].url_counts[i];
                }
            }
        }
    } else {
        ThreadData data;
        data.start = 0;
        data.end = total_lines;
        data.error_count = 0;
        data.ip_count = 0;
        data.url_count = 0;
        process_logs(&data);
        total_errors = data.error_count;
        for (int i = 0; i < data.ip_count; i++) {
            if (!ip_exists_global(global_ips, global_ip_count, data.ips[i])) {
                strcpy(global_ips[global_ip_count], data.ips[i]);
                global_ip_count++;
            }
        }
        for (int i = 0; i < data.url_count; i++) {
            int idx = find_global_url(global_urls, global_url_count, data.urls[i]);
            if (idx == -1) {
                strcpy(global_urls[global_url_count], data.urls[i]);
                global_url_counts[global_url_count] = data.url_counts[i];
                global_url_count++;
            } else {
                global_url_counts[idx] += data.url_counts[i];
            }
        }
    }

    // encontrar la url más visitada
    int max = 0;
    int max_index = 0;
    for (int i = 0; i < global_url_count; i++) {
        if (global_url_counts[i] > max) {
            max = global_url_counts[i];
            max_index = i;
        }
    }

    gettimeofday(&end_time, NULL);
    double elapsed = (end_time.tv_sec - start_time.tv_sec) * 1000.0;
    elapsed += (end_time.tv_usec - start_time.tv_usec) / 1000.0;

    // resultados
    printf("Total Unique IPs: %d\n", global_ip_count);
    printf("HTTP Errors: %d\n", total_errors);
    printf("Most Visited URL: %s (%d times)\n",
       global_urls[max_index],
       global_url_counts[max_index]);
    printf("Execution time: %.2f ms\n", elapsed);

    return 0;
}