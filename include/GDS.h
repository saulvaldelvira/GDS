/*
 * GDS.h - Main header file, includes all the modules.
 * Author: Saúl Valdelvira (2023)
 */
#pragma once

#ifndef GDS_H
#define GDS_H

#ifdef __cplusplus
extern "C" {
#endif

#define GDS_VERSION 1.0

#include "avl_tree.h"
#include "heap.h"
#include "dictionary.h"
#include "graph.h"
#include "linked_list.h"
#include "queue.h"
#include "stack.h"
#include "vector.h"

#include "util/compare.h"
#include "util/error.h"
#include "util/hash.h"

#ifdef __cplusplus
}
#endif
#endif // GDS_H
