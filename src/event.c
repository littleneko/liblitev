//
// Created by litao21 on 2017/7/24.
//

#include "lev.h"

lev_event *lev_event_new(int fd, int flags, lev_cb_func cb, void *args) {
    struct event_t *ev = (struct event_t*)malloc(sizeof(struct event_t));
    memset(ev, 0, sizeof(struct event_t));
    ev->fd = fd;
    ev->flags = flags;
    ev->cb = cb;
    ev->args = args;
}
void lev_event_del(lev_event *ev) {
    free(ev);
}