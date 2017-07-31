//
// Created by litao21 on 2017/7/24.
//

#include "lev.h"
#include "io.h"
#include "utils.h"

static int __lev_update_timer(struct loop_t *);
static void __lev_run_timer(struct loop_t *);
static void __lev_run_pending(struct loop_t *);
static void __lev_run_idle(struct loop_t *);

lev_loop *lev_loop_new() {
    lev_loop *loop = (lev_loop *) malloc(sizeof(lev_loop));
    memset(loop, 0, sizeof(lev_loop));
    loop->active_queue = new_queue();
    loop->ready_queue = new_queue();
    loop->timer_queue = new_queue();
    loop->timeout_queue = new_queue();
    loop->last_update_timer_time = 0;

    loop->io_handler = (struct io_handler*)malloc(sizeof(struct io_handler));
    using_epoll(loop);
    loop->io_handler->io_init(loop);
    return loop;
}

void lev_loop_del(lev_loop *loop) {
    if (loop) {
        if (loop->ready_queue) {
            free(loop->ready_queue);
        }
        if (loop->active_queue) {
            free(loop->active_queue);
        }
        free(loop);
    }
}

void add_event(lev_loop *loop, lev_event *event) {
    loop->event_count++;
    if (event->flags & LEV_TIMEOUT) {
        queue_push(loop->timer_queue, event);
        __lev_update_timer(loop);
    } else {
        queue_push(loop->active_queue, event);
        loop->io_handler->io_add(loop, event);
    }
}

void lev_run(lev_loop *loop) {
    while (1 && !loop->stop_flag) {
        __lev_run_timer(loop);
        __lev_run_pending(loop);
        __lev_run_idle(loop);
        int timeout = __lev_update_timer(loop);
        loop->io_handler->io_poll(loop, timeout);
    }
}

int __lev_update_timer(struct loop_t *loop) {
    struct timespec ts;
    get_cur_time(&ts);
    long long cur_time_ms = ts.tv_sec * 1000 + ts.tv_nsec / 1000;
    if (loop->last_update_timer_time == 0) {
        loop->last_update_timer_time = cur_time_ms;
    }

    int min_timeout = EPOLL_WAIT_MAX;

    struct queue_node_t *cur_node = loop->timer_queue->head;
    while (cur_node != NULL) {
        lev_event *ev = (lev_event *) (cur_node->data);
        ev->timeout -= (cur_time_ms - loop->last_update_timer_time);
        if (ev->timeout <= 0) {
            queue_push(loop->timeout_queue, ev);
        } else {
            min_timeout = ev->timeout < min_timeout ? ev->timeout : min_timeout;
        }
        struct queue_node_t *tmp = cur_node;
        cur_node = cur_node->next;
        queue_remove(loop->timer_queue, tmp);
    }
    loop->last_update_timer_time = cur_time_ms;

    return min_timeout;
}

void __lev_run_timer(struct loop_t *loop) {
    while (queue_empty(loop->timeout_queue)) {
        lev_event *ev = queue_front(loop->timeout_queue);
        ev->cb(0, LEV_TIMEOUT, ev->args);
        queue_remove(loop->timeout_queue, ev);
    }
}

void __lev_run_pending(struct loop_t *loop) {
    while (queue_empty(loop->ready_queue)) {
        lev_event *ev = queue_front(loop->ready_queue);
        ev->cb(0, LEV_TIMEOUT, ev->args);
        queue_remove(loop->ready_queue, ev);
    }
}

void __lev_run_idle(struct loop_t *loop) {
    // Todo:
}