#ifndef _MESSG_H
#define _MESSG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <mqueue.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

#define MONITOR_QUEUE "/MONITOR"
#define NODE_NAME_PREFIX "NODE_"
#define MEMSIZE 8
#define MAX_MSG_SIZE 1024
#define TYPE 1
typedef struct temperature {
    mqd_t msqid;
    float temperature;
} TEMPERATURE;

typedef struct message {
    bool stable;
    int nodeId;
    TEMPERATURE temperature;
} MESSAGE;

#define oops(ermsg,erno) {perror(ermsg); exit(erno); }

#endif
