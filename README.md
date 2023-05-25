# Generic Data Structures
A set of "generic" Data Structures. <br>
It includes: <br>
- **Vector** <br>
- **Linked List** <br>
- **AVL Tree** <br>
- **Graph** <br>
- **Dictionary** <br>
- **Heap** <br>
- **Stack** <br>
- **Queue** <br>


These structures are "generic" in the sense that they only need to know the size of the data type being stored. <br>
This is achieved using pointer arithmetic and functions like memcpy. <br>

```c
int main(){
    Vector *vec = vector_init(sizeof(int), compare_int);
    int tmp = 12;
    vector_append(vec, &tmp);
    tmp = 3;
    vector_append(vec, &tmp);
    vector_free(vec);
    return 0;
}
```

In the example above, we create a vector to store integers. We pass sizeof(int) as a parameter when initializing it. <br>
When calling vector_append, the function copies the size of an integer from the address of tmp into the vector. <br>
Note that these structures store values, not references (i.e. they don't store the pointer we pass, but rather they copy the value that's inside)<br>

Tip: you can use [compound literals](https://gcc.gnu.org/onlinedocs/gcc/Compound-Literals.html) to avoid having to declare a variable. <br>
```c
vector_append(vec, &(int){12});
vector_append(vec, &(int){3});
```

## How are elements compared?
Since we store "generic" data, we must have a way to compare it. <br>
These structures require a comparator function to be passed as a parameter when they are constructed. <br>
That function must be like this:<br>
```c
int func_name(const void* param_1, const void* param_2);
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
    if (i_1 > i_2)
        return 1;
    else if(i_1 < i_2)
        return -1;
    else
        return 0;
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

```c
LinkedList *list = list_init(sizeof(char), compare_char); // This list stores chars
```

If you don't need to compare elements inside the structure (e.g. when using a stack to just push and pop) you can use the `compare_equal`, `compare_lesser` or `compare_greater` functions, which always return 0, -1, and 1 respectively.

## Destructors
You can set a "destructor" function, to perform additional cleanup. <br>
For example, let's say you have a Vector of int pointers (int*), and you allocate it's elements using malloc. <br>
You can use the `vector_set_destructor` function, and pass the `destroy_ptr` function as a destructor for that vector. <br>
When freeing the vector or removing elements from it, destroy_ptr will be called. <br>
You can also write your own destructors. See `/example/destructors.c` <br>
Note: if you set a destructor, and want to remove elements without "destroying" them, you should use the pop function instead. <br>

## Building
You can use the Makefile to build and install the library. <br>
- `make`: builds the library <br>
- `make test`: builds and runs test programs <br>
- `make install`: installs the library on the computer.<br>
          The default installation path is /usr/local, but it
          can be overriden by defining INSTALL_PATH (e.g. `make install INSTALL_PATH=/lib`) <br>
- `make uninstall`: removes the library from the computer.<br>
          Remember to set INSTALL_PATH to the same value as in installation.
          If you don't remember it, run `find / -name 'libGDS.so'`. <br>
- `make clean`: Removes the binaries. <br>

To use the library, just include the header(s) and remember to compile with the **-lGDS** or **-lGDS-static** flags. <br>
NOTE: The headers are installed in $(INSTALL_PATH)/include/GDS. <br>
Example:
```c
#include <GDS/GDS.h> // or #include <GDS/Vector.h>

int main(){
        Vector *v = vector_init(sizeof(int), compare_int);
        // ....
        vector_free(v);
        return 0;
}
```

## Another example:
```c
struct Person{
    int id;
    int age;
    char *name;
};

int compare_person(const void* e_1, const void* e_2){
    struct Person p1 = * (struct Person*) e_1;
    struct Person p2 = * (struct Person*) e_2;
    // You can tweak things as much as you want. What if the id is different?
    // Maybe then we want to compare by name, or by age...
    if (p1.id > p2.id)
        return 1;
    else if (p1.id < p2.id)
        return -1;
    else
        return 0;
}

int main(){
    Vector *vector = vector_init(sizeof(struct Person), compare_person);
    vector_append(vector, &(struct Person){012345, 23, "My name"});
    vector_free(vector);
}
```
More in the `/example` folder
