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

#include "AVLTree.h"
#include "Heap.h"
#include "Dictionary.h"
#include "Graph.h"
#include "LinkedList.h"
#include "Queue.h"
#include "Stack.h"
#include "Vector.h"

#include "util/compare.h"
#include "util/definitions.h"
#include "util/error.h"
#include "util/hash.h"

#ifdef __cplusplus
}
#endif
#endif // GDS_H
