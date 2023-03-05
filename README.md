# Generic-Data-Structures
**Author**: Saúl Valdelvira <<saulvaldelvira@gmail.com>> <br>
 ============================================ <br>
A set of ""generic"" Data Structures, implemented in C. <br>
It includes: <br>
&nbsp;&nbsp;&nbsp;&nbsp; - **Lists**: Vector, LinkedList <br>
&nbsp;&nbsp;&nbsp;&nbsp; - **Stack** <br>
&nbsp;&nbsp;&nbsp;&nbsp; - **Queue** <br>
&nbsp;&nbsp;&nbsp;&nbsp; - **Trees**: BTree, AVLTree, BSTree <br>
&nbsp;&nbsp;&nbsp;&nbsp; - **Graph** <br>
&nbsp;&nbsp;&nbsp;&nbsp; - **Binary Heap** <br>




## How it works
These structures are "generic" in the way that the only information they need about the data type stored is its size. <br>
When creating one of these structures, you'll have to pass a parameter specifying the size in bytes of the data type to be stored. <br> 
To operate with these structures, for example when adding an element, you just have to pass a pointer to that element. <br>
It's important to note that these structures store the **VALUE** of the elements, not their references, so even if you pass a pointer to an element, these structures will COPY the value stored in that adress. <b> They do not store the adress into an array of pointers </b>. This makes the structures more memory safe and efficient.

```c
int main(){
    Vector *vec = vector_empty(sizeof(int), compare_int);
    int tmp = 12;
    vector_append(vec, &tmp);
    tmp = 3;
    vector_append(vec, &tmp);

    vector_free(vec);
    return 0;
}
```

In the example above, both 12 and 3 are added into the vector, since every call to the append function just copies 4 bytes (sizeof(int)) from the value stored in tmp into vec. To acess and work with the elements into the structures, pointer arithmetic and standard memory manipulation routines (memcpy, memset) are used.

## How are elements compared?
This is an important issue. Since we store "generic" data, we must have a way to compare that data.
That's why this structures require a function pointer to be passed as a parameter when they are constructed.<br>
That function must be like this:<br>
```c
int func_name (const void* param_1, const void* param_2);
```
And it must return: <br>
&nbsp;&nbsp;&nbsp;&nbsp; - **1**  if param_1 is > than param_2 <br>
&nbsp;&nbsp;&nbsp;&nbsp; - **-1** if param_1 is < than param_2 <br>
&nbsp;&nbsp;&nbsp;&nbsp; - **0**  if param_1 is == than param_2 <br>
  
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
    int a = 1;
    int b = 2;
    void *aptr = &a;
    void *bptr = &b;
    assert(compare_int(aptr, bptr) < 0);
    return 0;
}
```
The header file **comparator.h** defines functions to compare the most common data types (int, char, long, etc.)

```c
LinkedList *list = lnkd_list_init(sizeof(char), compare_char); // This list stores chars
```
For more info look at the **doc** folder
