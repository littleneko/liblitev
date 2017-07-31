//
// Created by litao21 on 2017/7/25.
//

#ifndef LIBED_IO_H
#define LIBED_IO_H

enum io_type {
    SELECT,
    EPOLL,
    KQUEUE
};

typedef void (*io_handler_init)(struct loop_t *);

typedef void (*io_add)(struct loop_t *, struct evevt_t *);

typedef void (*io_del)(struct loop_t *, struct evevt_t *);

typedef void (*io_poll)(struct loop_t *, int timeout);

struct io_handler {
    enum io_type type;
    io_handler_init io_init;
    io_add io_add;
    io_del io_del;
    io_poll io_poll;
};

void using_epoll(struct loop_t *loop);
void using_kqueue(struct loop_t *loop);

#endif //LIBED_IO_H
