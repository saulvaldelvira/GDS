# Generic-Data-Structures
A set of generic Data Structures implemented in C. <br>
It includes:
* Lists: [DynamicArray](/src/DynamicArray), [LinkedList](/src/LinkedList)
* [Stack](src/Stack)
* [Queue](src/Queue)
* Trees: [BSTree](src/BSTree), [AVLTree](/src/AVLTree)
* [Graph](src/Graph)
* [Binary Heap](src/BinaryHeap)

## How it works
These structures are "generic" in the way that the only information they need about the data type stored is its size. <br>
When creating 
one of these structures, you'll have to pass a parameter specifying the size in bytes of the data type to be stored. To operate with these structures, for example when adding an element, you just have to pass a pointer to that element. <br>
It's important to note that these structures store the VALUE of the elements, not their references, so even if you pass a pointer to an element, these structures will COPY the value stored in that adress. <b> They do not store the adress into an array of pointers </b>. This makes the structures more memory safe and efficient.

```c
int main(){
    DynamicArray *list = din_arr_empty(sizeof(int), compare_int);
    int tmp = 12;
    din_arr_append(list, &tmp);
    tmp = 3;
    din_arr_append(list, &tmp);

    din_arr_free(list);
    return 0;
}
```

In the example above, both 12 and 3 are added into the list, since every call to the append function just copies 4 bytes (sizeof(int)) from the value stored in tmp into the list. To acess and work with the elements into the structures, pointer arithmetic and standard memory manipulation routines (memcpy, memset) are used.

## How are elements compared?
This is an important issue. Since we store "generic" data, we must have a way to compare that data. <br>
That's why all this structures require a function pointer to be passed, also as a parameter when they are constructed. This function must be like this <br>
```c
int func_name (const void* param_1, const void* param_2);
```
And it must return <br>
* <b> 1</b> if param_1 is > than param_2
* <b>-1</b> if param_1 is < than param_2
* <b> 0</b> if param_1 is == than param_2
  
For example:<br>
```c
int compare_int(const void* param_1, const void* param_2){
    // Take the int values
    int i_1 = * (int*) param_1;
    int i_2 = * (int*) param_2;
    // Compare them
    if (i_1 > i_2){
        return 1;
    }else if(i_1 < i_2){
        return -1;
    }else{
        return 0;
    }
}

int main(){
    LinkedList *list = lnkd_list_init(sizeof(int), compare_int); // Pass compare_int as a parameter
    int one = 1;
    lnkd_list_push_back(list, &one);
    assert(lnkd_list_exists(list, &one)); // This will return true.
}
```
<b>NOTE:</b> The header file [comparator.h](src/Util/comparator.h) defines functions to compare the most common data types (compare_int, compare_float, etc.).

```c
#include "comparator.h"

int main(){
    LinkedList *list = lnkd_list_init(sizeof(char), compare_char); // To compare chars
}
```
