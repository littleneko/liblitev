//
// Created by litao21 on 2017/7/24.
//

#ifndef LIBED_LEV_H
#define LIBED_LEV_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "config.h"

#ifdef HAVE_KQUEUE_H
#include <sys/event.h>
#endif
#ifdef HAVE_EPOLL_H
#include <sys/epoll.h>
#endif

#include "queue.h"
#include "io.h"

#define EPOLL_FD_SIZE   1024
#define EPOLL_EVENTS    20
#define EPOLL_WAIT_MAX  5000

#define LEV_READ     0x01
#define LEV_WRITE    0x02
#define LEV_TIMEOUT  0x04
#define LEV_PERSIST  0x08

typedef void (*lev_cb_func)(int fd, int what, void *args);

typedef struct event_t {
    union {
        int fd;
        int timeout;
    };
    int flags;
    lev_cb_func cb;
    void *args;
} lev_event;


typedef struct loop_t {
    int io_handler_id;
    int event_count;
    int stop_flag;
    long long last_update_timer_time;
    queue *active_queue;
    queue *ready_queue;
    queue *timer_queue;
    queue *timeout_queue;
    struct io_handler *io_handler;
} lev_loop;

typedef lev_event timer;

lev_event *lev_event_new(int, int, lev_cb_func, void *);

void lev_event_del(lev_event *);

#define lev_timer_new(time, cb, arg) lev_event_new(0, EV_TIMEOUT, cb, args)

lev_loop *lev_loop_new();

void lev_loop_del(lev_loop *);

void add_event(lev_loop *, lev_event *);

#define lev_add_timer(loop, ev) add_event(loop, ev)

void lev_run(lev_loop *);

#endif //LIBED_LEV_H
