# Generic Data Structures
A set of "generic" Data Structures, implemented in C. <br>
It includes: <br>
- **Lists:** Vector, LinkedList <br>
- **Stack** <br>
- **Queue** <br>
- **Trees:** BTree, AVLTree, BSTree <br>
- **Graph** <br>
- **Dictionary** <br>
- **Binary Heap** <br>

## How it works
These structures are "generic" in the sense that the only information they need about the
data type stored is its size. <br>
They use pointer arithmetic and memory manipulation routines (e.g. memcpy) to work with the
data in a "type independent" way. <br>
It's important to note that these structures store **VALUES**, not references.

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

In the example above, both 12 and 3 are added into the vector, since every call to the append function just copies 4 bytes (sizeof(int)) from the value stored in tmp into vec.

## How are elements compared?
Since we store "generic" data, we must have a way to compare it. <br>
That's why these structures require a comparator function to be passed as a parameter when they are constructed. <br>
That function must be like this:<br>
```c
int func_name (const void* param_1, const void* param_2);
```
And it must return: <br>
- **1**  if param_1 is > than param_2 <br>
- **-1** if param_1 is < than param_2 <br>
- **0**  if param_1 is == than param_2 <br>

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
The header file **comparator.h** defines functions to compare the most common data types (int, char, long, etc.) <br>
If you don't need to compare elements inside the structure (e.g. when using a stack to just push and pop) you can use the `compare_always_equal` function so the program doesn't complain.

```c
LinkedList *list = list_init(sizeof(char), compare_char); // This list stores chars
```
## Building
You can use the Makefile to build and install the library. <br>
- `make`: builds the library <br>
- `make test`: builds and runs test programs <br>
- `make install`: installs the library on the computer. Must be run as root.<br>
          The default installation path is /usr/local/lib, but it
          can be overriden by defining INSTALL_PATH (e.g. `make install INSTALL_PATH=/lib`) <br>
- `make uninstall`: removes the library from the computer. Must be run as root.<br>
          Remember to set INSTALL_PATH to the same value as in installation.
          If you don't remember it, run `find / -name 'libGDS.so'`.

To use the library, just include the header(s) and remember to compile with the **-lGDS** or **-lGDS-static** flags. <br>
NOTE: The headers are installed in $(INSTALL_PATH)/include/GDS. <br>
Example:
```c
#include <GDS/GDS.h> // or #include <GDS/Vector.h>

int main(){
        Vector *v = vector_empty(sizeof(int), compare_int);
        // ....
        vector_free(v);
        return 0;
}
```
Compile: `gcc main.c -lGDS` or `gcc main.c -lGDS-static`
