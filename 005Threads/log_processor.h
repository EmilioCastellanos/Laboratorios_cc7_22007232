#ifndef LOG_PROCESSOR_H
#define LOG_PROCESSOR_H

#define MAX_URLS 1000
#define MAX_IPS 1000

typedef struct {

    int start;
    int end;

    int error_count;

    char ips[MAX_IPS][50];
    int ip_count;

    char urls[MAX_URLS][100];
    int url_counts[MAX_URLS];
    int url_count;

} ThreadData;

void* process_logs(void* arg);

#endif
