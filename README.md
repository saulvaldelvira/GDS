# Generic-Data-Structures
A set of generic Data Structures implemented in C. <br>
It includes:
* Lists: [ArrayList](/src/ArrayList), [LinkedList](/src/LinkedList)
* [Stack](src/Stack)
* [Queue](src/Queue)
* Trees: [BSTree](src/BSTree)

## How it works
To make this structures "generic", they store void pointers to elements, not the elements themselves. This allows them to 
store any kind of data type. <br>
For example, let's say i want to store ints. I would have to add int pointers (wich will cast to void pointers) to the structures.
When i take those elements off the structure, I would have to cast back those void pointers to int pointers again.<br>
<b>NOTE:</b> This is risky, because we have to trust in all the void pointers in the structure to correspond to a memory poisition of an int. The good 
thing is that if you are careful to only add ONE data type to the structure you can "safely" cast those returned pointers to that data type. <br>
So, carefully used this structures are a good way of storing generic information, allowing to use <b>ONE data structure for MULTIPLE data types.</b><br>

## How are elements compared?
This is an important issue. Since we store void pointers, we must have a way to compare those pointers, to know if they are equal, greater or lower than each other. <br>
That's why all this structures require a function pointer to be passed as a parameter when they are constructed. This function must be like this <br>
```c
int func_name (void* param_1, void* param_2);
```
And it must return <br>
* <b> 1</b> if param_1 is > than param_2
* <b>-1</b> if param_1 is < than param_2
* <b> 0</b> if param_1 is == than param_2
  
For example:<br>
```c
int compare_int(void* param_1, void* param_2){
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
    LinkedList list = lnkd_list_init(compare_int); // Pass compare_int as a parameter
    int one = 1;
    lnkd_list_push_back(&list, &one);
    assert(lnkd_list_exists(list, &one)); // This will return true.
}
```
<b>NOTE:</b> The header file [comparator.h](src/Util/comparator.h) defines functions to compare the most common data types. 
Those are defined in the Comparator struct.

```c
#include "comparator.h"

int main(){
    LinkedList list = lnkd_list_init(Comparator.integer); // To compare ints
}
```

## How is memory managed?
In all of this structures, there's a need to allocate memory (scary). They have a dynamic size, wich means we also have to dynamically allocate memory. <br>
But this also means that we have to free this memory. The issue here is that, because of the nature of this structures, they store pointers to memory locations.
Sometimes this void pointers will not need to be freed, and sometimes they will. <br>
Example: <br>
Let's say I have a LinkedList that stores ints. <br>

```c

int main() {
    LinkedList list = lnkd_list_init(Comparator.integer);
    // Declare 3 int variables
    int one = 1;
    int two = 2;
    int three = 3;

    // Add them to the list
    lnkd_list_push_back(&list, &one);
    lnkd_list_push_back(&list, &two);
    lnkd_list_push_back(&list, &three);

    // Now remove them
    lnkd_list_remove(&list, &one);
    lnkd_list_remove(&list, &two);
    lnkd_list_remove(&list, &three);

    return 0;
}
```

Those 3 ints are not allocated dynamically. We declare three variables, and store their adresses into the list. <br>
But what if I want to store more integer into the list?


```c

int main() {
    LinkedList list = lnkd_list_init(Comparator.integer);
    for (int i=0; i < 100; i++){
        lnkd_list_push_back(&list, &i);
    }
    return 0;
}
```

The above code <b> won't work </b> since we only have <b> ONE </b> variable (i) and we are adding it's adress <b>A HUNDRED TIMES</b>. <br>
But it is the same adress so the linked list will contain 100 copies of the adress of i (wich at the end of the loop will store the value 100). <br>
If we want to add the numbers 0 to 99 we can do two things <br>

First: Store those 100 adresses into an array of fixed size

```c
int main() {
    LinkedList list = lnkd_list_init(Comparator.integer);
    int numbers[100]; // Declare an array to store the numbers
    for (int i=0; i < 100; i++){
        numbers[i] = i; // Give the adress a value
        lnkd_list_push_back(&list, &numbers[i]); // Add the adress of the i'th number
    }
    return 0;
}
```

This will behave as expected. The list will contain 100 different adresses, each one with a different value. <br>

Second: Allocate the adresses dynamically

```c
int main() {
    LinkedList list = lnkd_list_init(Comparator.integer);
    for (int i=0; i < 100; i++){
        int *ptr = malloc(sizeof(int));
        *ptr = i;
        lnkd_list_push_back(&list, ptr);
    }
    return 0;
}
```
 
This will also add 100 diferent adresses. <br>

### So what's the problem?

In this second case, we have to make sure those 100 adresses are freed once they are not needed. To make this easier, all 
of the Data Structures in this repository have a variable of type bool called <b>free_on_delete</b>. Setting it to true will make the structure free
the memory adress of the element it is removing. <br>
For example, LinkedList uses a Node for each element, and allocates memory for those nodes. When an element is deleted, 
it's node is freed. If (<b>free_on_delete == true</b>) the adress stored by the node will also be free. So, following the previous example.


```c
int main() {
    LinkedList list = lnkd_list_init(Comparator.integer);
    // FREE_ON_DELETE expands to 1 (true).
    lnkd_list_configure(&list, FREE_ON_DELETE);//this is the same as "list.free_on_delete = true"
    for (int i=0; i < 100; i++){
        int *ptr = malloc(sizeof(int));
        *ptr = i;
        lnkd_list_push_back(&list, ptr);
    }

    for (int i=0; i < 100; i++){
        lnkd_list_remove(&list, &i);
    }

    return 0;
}
```

This will make list free the elements when removing them. So the 100 adresses will be freed automatically and the user won't have to worry about them.<br>
There's a catch. If you mix dynamically allocated memory with static memory you'll have to be careful, because both freeing adresses you shouldn't or dont freeing the ones you should is dangerous. <br>

<b>NOTE:</b> that when removing the elements, we can pass the adress of i. Since we are only using it to compare it with the elements in the list, it does not matter.

The header file [allocate.h](src/Util/allocate.h.h) defines methods to allocate memory for the most common data types. So the previous example could be written like this<br>

```c
#include "allocate.h"

int main() {
    LinkedList list = lnkd_list_init(Comparator.integer);
    lnkd_list_configure(&list, FREE_ON_DELETE); // FREE_ON_DELETE expands to 1 (true).
    for (int i=0; i < 100; i++){
        // alloc_int returns a pointer to an int, storing the value passed as a parameter
        lnkd_list_push_back(&list, alloc_int(i)); 
    }

    for (int i=0; i < 100; i++){
        lnkd_list_remove(&list, &i);
    }

    return 0;
}
```

Another thing to keep in mind is that, since ALL the data structures use dynamic memory, a "freeing routine" should be called NO MATHER WHAT, when they are not
needed anymore. <br>

```c
#include "allocate.h"

int main() {
    LinkedList list = lnkd_list_init(Comparator.integer);
    lnkd_list_configure(&list, FREE_ON_DELETE); // FREE_ON_DELETE expands to 1 (true).
    for (int i=0; i < 100; i++){
        // alloc_int returns a pointer to an int, storing the value passed as a parameter
        lnkd_list_push_back(&list, alloc_int(i)); 
    }

    lnkd_list_free(list); // This will free all the memory allocated for list.

    return 0;
}
```

Those freeing routines also depend on the configuration of the structure. If free_on_delete is true, when freeing the structure, ALL the void pointers it stores will also be freed.