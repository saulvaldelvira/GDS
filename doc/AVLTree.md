# AVL Tree
The AVL Tree, named after its two Soviet inventors, Georgy Adelson-Velsky and Evgenii Landis,  is a self-balancing binary search tree. <br>
In an AVL tree, the heights of the two child subtrees of any node differ by at most one. This means, the absolute value of `right height - left height` is allways <= 1, for all nodes in the tree.

If an inbalance apprears, a "rotation" is performed to fix it. This guarantees a O(log n) time complexity for all basic operations, and solves one of the main inconveniences of Binary Search Trees, as it avoids the appearence of degenerate Trees.

![AVLTree gif](img/AVLTree/AVLTree1.gif)

## Time complexity

| Operation  | Average Case | Worst Case |
|--- | --- | ---|
| Add        | O(log n)	    | O(log n)   |
| Delete    | O(log n)     | O(log n)   |
| Search     | O(log n)	    | O(log n)       |

## Resources
* [Wikipedia](https://en.wikipedia.org/wiki/AVL_tree)
* [Data Structures (Trees) by Martin Gonzalez Rodriguez](https://www.lulu.com/shop/martin-gonzalez-rodriguez/data-structures-trees/ebook/product-22017004.html?q=&page=1&pageSize=4)