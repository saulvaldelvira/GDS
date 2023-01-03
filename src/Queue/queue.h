/**
 * Autor:   Saúl Valdelvira Iglesias
 * Email:   saulvaldelvira@gmail.com
 * Version: 03/01/2023
*/
#pragma once
#ifndef QUEUE_H
#define QUEUE_H

typedef struct QueueNode {
    void *info;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue {
    QueueNode *head;
    QueueNode *tail;
    // Comparator function
    bool (*compare) (void*, void*);
} Queue;


#endif