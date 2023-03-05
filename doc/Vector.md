# Vector
The Vector (Arraylist, Dynamic Array, etc.) is one of the simplest data structures.
It is a <b>random access, variable-size</b> list.

## Pros and Cons
The main problem with the Vector is it's time complexity when adding, removing or inserting elements, specially at the beginning of the array (push front).
Also when searching elements with no known index (by value). In all those cases, the complexity tends to be O(n). <br>
Since we store the elements in an array, when we remove an element at index i, we must iterate the whole
array until the end since i, and move back every element one position. In the worst case this means a whole iteration
through the list [ O(n) ]. <br>
The biggest strength of this structure is how fast it is in <b>random access operations</b> (like getting the element in a certain index).

## When to use it
The Vector is a great option when you need the memory locallity and efficiency of an array, with the advantage of not having to worry about size or memory management.
## Growth Factor
In this list, the growth factor is 2. This means that when the list is full, it's size is <b>multiplied by two</b>. <br>
This makes the list expand <b>geometrically</b>, wich is efficient since resizing an array is a very expensive operation. <br>
To resize an array you have to allocate the new memory and copy all the element of the old array into the new one. <br>

## Time complexity

| Operation  | Average Case | Worst Case |
|--- | --- | ---|
| Add/Remove (End)        | O(1)	    | O(1)   |
| Add/Remove (Beginning)        | O(n)	    | O(n)   |
| Get/Set (by Index)   | O(1)	    | O(1)       |
| Get/Set (by Value)   | O(n)	    | O(n)       |
