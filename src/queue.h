//
//
// Created by litao21 on 2017/7/24.
//

#ifndef LIBED_LIST_H
#define LIBED_LIST_H

struct queue_node_t {
    struct queue_node_t *next;
    struct queue_node_t *prev;
    void *data;
};

typedef struct queue_t {
    struct queue_node_t *head;
    struct queue_node_t *tail;
    int node_count;
} queue;

queue *new_queue();

void *queue_front(queue *);

void queue_remove(queue *, struct queue_node_t *);

void queue_push(queue *, void *);

int queue_empty(queue *);

#endif //LIBED_LIST_H
