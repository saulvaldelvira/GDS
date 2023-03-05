# Binary Heaps
A binary heap is a [heap](https://en.wikipedia.org/wiki/Heap_(data_structure)) data structure that takes the form of a [Binary Tree](https://en.wikipedia.org/wiki/Binary_tree). <br>
However, there are two extra constraints to this structure <br>
- **Shape property**: a Binary Heap must be a complete binary tree. This means, all levels of the tree (except the last one) must be filled.
    This makes posible to implement this structure as an array of elements, and compute the nodes with simple operations that will be later discussed. <br>
- **Heap property**: The key stored in a node must be either >= or <= than the keys in the childen nodes. This depends on wether the Heap is a **max-heap** (<=) or a **min-heap** (>=)

This data structure is commonly used to implement [Prioirity Queues](https://en.wikipedia.org/wiki/Priority_queue)<br>

## How are nodes computed
Given an index i, we can calculate the father of i as <br>
`father = (i - 1) / 2` (taking only the integer part).<br>
We can calculate the two child nodes as `child_1 = i * 2 + 1` and `child_2 = i * 2 + 2` <br>


## Time complexity

| Operation  | Average Case | Worst Case |
|--- | --- | ---|
| Add        | O(1)	    | O(log n)   |
| Peek-min   | O(1)	    | O(1)       |
| Pop-min    | O(log n)     | O(log n)   |
| Search     | O(n)	    | O(n)       |

## Resources: 
* [Binary Heaps (Wikipedia)](https://en.wikipedia.org/wiki/Binary_heap)
* [Data Structures (Trees) by Martin Gonzalez Rodriguez](https://www.lulu.com/shop/martin-gonzalez-rodriguez/data-structures-trees/ebook/product-22017004.html?q=&page=1&pageSize=4)