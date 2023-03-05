# Vector
The Vector (Arraylist, Dynamic Array, etc.) is one of the simplest data structures.
It is a **random access, variable-size** list.

## Pros and Cons
The biggest strength of this structure is how fast it is in **random access operations** (like getting the element in a certain index). <br>
On the other hand, removing or inserting elements, specially at the beginning of the array (push front) becomes a really expensive task. <br>
Since we store the elements in an array, when we remove an element at index i, we must iterate the whole array until the end since i, and move back every element one position. <br>
In the worst case this means a whole iteration through the list [ **O(n)** ]. <br>


## When to use it
The Vector is a great option when you need the memory locallity and efficiency of an array, with the advantage of not having to worry about the size.
## Growth Factor
In this implementation, the growth factor is 2. This means that when the list is full, it's size is **multiplied by two**. <br>
This makes the list expand **geometrically**, which is desirable, since resizing an array is a very expensive operation. <br>
To resize an array you have to allocate the new memory and copy all the element of the old array into the new one. <br>

## Time complexity

| Operation  | Average Case | Worst Case |
|--- | --- | ---|
| Add/Remove (End)        | O(1)	    | O(1)   |
| Add/Remove (Beginning)        | O(n)	    | O(n)   |
| Get/Set (by Index)   | O(1)	    | O(1)       |
| Get/Set (by Value)   | O(n)	    | O(n)       |
