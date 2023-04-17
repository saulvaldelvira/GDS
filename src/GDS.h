/**
 *  Copyright (C) 2023 - Saúl Valdelvira
 *
 *  This library is free software. You can modify
 *  and/or redistribute it under the terms of the
 *  GNU Lesser General Public License version 2.1
 *  See <https://www.gnu.org/licenses/>
 *
 *  Email: saulvaldelvira@gmail.com
 */
#pragma once

#ifndef GDS_H
#define GDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "AVLTree.h"
#include "Heap.h"
#include "BSTree.h"
#include "BTree.h"
#include "Dictionary.h"
#include "Graph.h"
#include "LinkedList.h"
#include "Queue.h"
#include "Stack.h"
#include "Vector.h"

#include "Util/comparator.h"
#include "Util/definitions.h"
#include "Util/error.h"
#include "Util/hash.h"
#include "Util/index_t.h"
#include "Util/void_cast.h"

#ifdef __cplusplus
}
#endif
#endif // GDS_H