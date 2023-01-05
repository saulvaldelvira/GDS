# Generic-Data-Structures
A set of generic Data Structures implemented in C. <br>
It includes
* Lists: [ArrayList.h](/src/ArrayList), [LinkedList.h](/src/LinkedList)
* [Stack](src/Stack)
* [Queue](src/Queue)
* Trees: [BSTree.h](src/BSTree)

## How it works
To make this structures "generic", they store void pointers to elements, not the elements themselves. This allows them to 
store any kind of data type. <br>
For example, let's say i want to store ints. I would have to add int pointers (wich will cast to void pointers) to the structures.
When i take those elements off the structure, I would have to cast back those void pointers to int pointers again<br>
<b>NOTE:</b> This is risky, because we have to trust that all the void pointers in the structure correspond to a memory poisition of an int. The good 
thing is that if you are carefull to only add ONE data type to the structure you can "safely" cast those returned pointers to that data type. <br>
So, carefully used this structures are a good way of storing generic information, allowing the user to use <b>ONE data structure for MULTIPLE data types</b>
