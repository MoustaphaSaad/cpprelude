#memory_context
A memory context is an object that abstracts a memory resource and it does so by abstracting the memory API which consists of three functions: `_alloc`, `_realloc`, `_free`.

##memory_context_t
```C++
struct memory_context_t;
```
###Members Interface
```C++
void* _self;
alloc_func_t _alloc;
realloc_func_t _realloc;
free_func_t _free;
```

- **_alloc:** a function used to allocate a chunk of memory.
- **_realloc:** a function used to resize a chunk of an already allocated memory.
- **_free:** a function used to release an allocated chunk of memory.
- **_self:** a pointer to user data. And this is for some allocation algorithms that may need to access state during execution.

###Function Types:

- **alloc_func_t:** `slice<byte> _alloc(void* userdata, usize wanted_size);`
- **realloc_func_t:** `void _realloc(void* userdata, slice<byte>& data, usize new_size);`
- **free_func_t:** `void _free(void* userdata, slice<byte>&);`

###Interface
####alloc
```C++
template<typename T>
slice<T> alloc(usize count = 1);
```
Allocates a slice of type `T` that can accommodate the provided `count`.
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
