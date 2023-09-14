# Generic Data Structures
A set of Data Structures for the C programming language. <br>
It includes: <br>
- **Vector.h** <br>
- **LinkedList.h** <br>
- **AVLTree.h** <br>
- **Graph.h** <br>
- **Dictionary.h** <br>
- **Heap.h** <br>
- **Stack.h** <br>
- **Queue.h** <br>


These structures are "generic" in the sense that they can store any kind of data type, by only knowing the size of it. <br>

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

In the example above, we create a vector to store integers. To do so, we pass sizeof(int) as a parameter when initializing it. <br>
When calling vector_append, the function copies the size of an integer from the address of tmp into the vector. <br>
Note that these structures store values, not references (i.e. they don't store the pointer we pass, but rather they copy the value that's inside)<br>

Tip: you can use [compound literals](https://gcc.gnu.org/onlinedocs/gcc/Compound-Literals.html) to avoid having to declare a variable. <br>
```c
vector_append(vec, &(int){12});
vector_append(vec, &(int){3});
```

## How are elements compared?
Since we store "generic" data, there must be a way to compare it. <br>
These structures require a comparator function to be passed as a parameter when they are constructed. <br>
That function must be like this:<br>
```c
int func_name(const void* e_1, const void* e_2);
```
And it must return: <br>
- **<= -1&nbsp;** if e_1 is < than e_2 <br>
- **0 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**     if e_1 is == than e_2 <br>
- **>= 1 &nbsp;**  if e_1 is > than e_2 <br>

For example:<br>
```c
int compare_int(const void* e_1, const void* e_2){
    int i_1 = * (int*) e_1;
    int i_2 = * (int*) e_2;
    return i_1 - i_2;
}

int main(){
    int a = 1;
    int b = 2;
    assert(compare_int(&a, &b) < 0);
    return 0;
}
```

The header file **comparator.h** defines functions to compare the most common data types (int, char, long, etc.) <br>

```c
LinkedList *list = list_init(sizeof(char), compare_char); // This list stores chars
```

If you don't need to compare elements inside the structure you can use the `compare_equal`, `compare_lesser` and `compare_greater` functions, which always return 0, -1, and 1 respectively.

## Destructors
You can set a "destructor" function to perform additional cleanup. <br>
Say you have a Vector of char*, and you allocate it's elements using malloc. <br>
You can use the `vector_set_destructor` function, and pass the `destroy_ptr` function as a destructor for that vector. <br>
When freeing the vector or removing an element from it, that "destructor" function will be called on that element(s). <br>
You can also write your own destructors. See `/example/destructors.c` <br>
**Note:** To remove an element without calling the destructor on that element you can use the pop function instead. <br>

```c
// Signature
void destructor_func(void *e); // e is a POINTER to the element to destroy

// Example: destroy a malloc'd pointer
void destroy_ptr(void *e){
	if (e){
		void *ptr = * (void**) e;
		free(ptr);
	}
}

// Example: destroy a struct
struct buffer {
    char *buf; // Dynamically allocated
    size_t capacity;
    size_t size;
};

void destroy_buffer(void *e){
    struct buffer *buffer = (struct buffer*) e;
    free(buffer->buf);
}

```

## Building
You can use the Makefile to build and install the library. <br>
- `make`: builds the library <br>
- `make test`: builds and runs test programs <br>
- `make install`: installs the library on the computer.<br>
          The default installation path is /usr/local, but it
          can be overriden by defining INSTALL_PATH (e.g. `make install INSTALL_PATH=/lib`) <br>
- `make uninstall`: removes the library from the computer. Remember to set INSTALL_PATH to the same value as in installation. <br>
- `make doxygen`: Builds the doxygen documentation. <br>
- `make clean`: Removes the binaries. <br>

To use the library, just include the header(s) and add the **-lGDS** or **-lGDS-static** flags when compiling. <br>
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
    return p1.id - p2.id;
}

int main(){
    Vector *vector = vector_init(sizeof(struct Person), compare_person);
    vector_append(vector, &(struct Person){012345, 23, "My name"});
    vector_free(vector);
}
```
