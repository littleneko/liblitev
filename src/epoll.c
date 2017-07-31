//
// Created by litao21 on 2017/7/24.
//

#include "lev.h"

void epoll_init(lev_loop *loop) {
    loop->io_handler_id = epoll_create(EPOLL_FD_SIZE);
    if (loop->io_handler_id < 0)
        error("epoll create error!");
}

void epoll_add(lev_loop *loop, lev_event *ev) {
    struct epoll_event event;
    event.data.fd = ev->fd;
    event.events = ev->flags | EPOLLET;
    int n = epoll_ctl(loop->io_handler_id, EPOLL_CTL_ADD, ev->fd, &event);
    if (n < 0)
        error("epoll add error!");
}

void epoll_del(lev_loop *loop, event *ev) {
    int n = epoll_ctl(loop->io_handler_id, EPOLL_CTL_DEL, ev->fd, NULL);
    if (n < 0)
        error("epoll del error!");
}

void epoll_poll(lev_loop *loop, int timeout) {
    struct epoll_event events[EPOLL_EVENTS];
    int ret = epoll_wait(loop->io_handler_id, events, EPOLL_EVENTS, timeout);
    for (int i = 0; i < ret; i++) {
        int sock = events[i].data.fd;

        while (queue_empty(loop->active_queue)) {
            lev_event *event = queue_front(loop->active_queue);
            queue_remove(loop->active_queue, event);
            if (event->fd == sock) {
                queue_push(loop->ready_queue, event);
            }
        }
    }
}

void using_epoll(lev_loop *loop) {
    loop->io_handler->type = EPOLL;
    loop->io_handler->io_init = epoll_init;
    loop->io_handler->io_add = epoll_add;
    loop->io_handler->io_del = epoll_del;
    loop->io_handler->io_poll = epoll_poll;
}