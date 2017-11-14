# Allocators

Allocators can be anything that wrap a `memory_context_t` and provide an access to `memory_context_t*` to be used later to interact with memory.

##arena_t
```C++
struct arena_t;
```
An arena is a chunk of memory that allocates memory in a stack fashion, and it can only free memory if the provided chunk is at the top of the stack. The main philosophy of an arena is that you want to build data structures quickly into the provided container and you will throw it away and reuse the same chunk of memory to build another temp data structures.

###Interface
####Constructor
```C++
arena_t(usize size, bool use_virtual_memory = true);
```
Constructs an arena with the provided size and it by default uses `virtual_alloc`.

####context
```C++
memory_context_t* context();
```
Returns a pointer to the context.

####free_all
```C++
void free_all();
```
Resets the memory and starts to allocate from the beginning.

####alloc
```C++
template<typename T>
slice<T> alloc(usize count = 1);
```
Allocates a slice of type `T  ` that can accommodate the provided `count`.

####free
```C++
template<typename T>
void free(slice<T>& data);
template<typename T>
void free(slice<T>&& data);
```
Given a slice memory, it frees the provided `data`.

####realloc
```C++
template<typename T>
void realloc(slice<T>& data, usize count);
template<typename T>
void realloc(slice<T>&& data, usize count);
```
Given a slice of memory, it resizes the slice to accommodate to the provided `count`.

