//
// Created by litao21 on 2017/7/24.
//

#include "queue.h"
#include "lev.h"

static struct queue_node_t *new_queue_node() {
    struct queue_node_t *node = (struct queue_node_t*)malloc(sizeof(struct queue_node_t));
    node->data = NULL;
    node->next = NULL;
    node->prev = NULL;
    return node;
};

queue *new_queue() {
    queue *q = (queue*)malloc(sizeof(queue));
    q->node_count = 0;
    q->head = new_queue_node();
    q->tail = q->head;
    return q;
}

void *queue_front(queue *q) {
    if (q->node_count < 0 || !q->head->next) {
        return (void*)0;
    }
    struct queue_node_t *node = q->head->next;
    return node->data;
}

void queue_remove(queue *q, struct queue_node_t *node) {
    struct queue_node_t *p = q->head->next;
    while (p != node && p) {
        p = p->next;
    }

    if (!p)
        return;

    p->prev->next = p->next;
    if (p->next) {
        p->next->prev = p->prev;
    } else {
        q->tail = p->prev;
    }
    free(node);
}

void queue_push(queue *q, void *data) {
    struct queue_node_t *node = new_queue_node();
    node->data = data;
    node->prev = q->tail;
    q->tail->next = node;
    q->tail = node;
}

int queue_empty(queue *q) {
    return q->node_count == 0;
}