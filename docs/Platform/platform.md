#platform
Represents the underlying host platforms.

##platform_t
```C++
struct platform_t;
extern platform_t& platform;
```
All the functionality of the host platform is exposed through the globally declared instance `platform`.

###Member interface
```C++
memory_contex_t* global_memory;
usize allocation_count;
usize allocation_size;
usize RAM_SIZE;
```

- **global_memory:** a memory context to the global memory of the platform.
- **allocation_count:** a counter to keep track of the currently alive allocated memory.
- **allocation_size:** a counter to keep track of the size of the currently alive allocated memory.
- **RAM_SIZE:** has the size of the physical RAM installed in the system.

###Interface
####virtual_alloc
```C++
slice<byte> virtual_alloc(void* address_hint, usize size);
```
Given an `address_hint` and a size, it attempts to allocate a chunk of memory from the platform virtual memory tables.

####virtual_free
```C++
bool virtual_free(slice<byte>& data);
bool virtual_free(slice<byte>&& data);
```
Given a slice of memory that's virtually allocated, it will free the memory.

####alloc
```C++
template<typename T>
slice<T> alloc(usize count = 1);
```
Allocates a slice of type `T`  that can accommodate the provided `count`.

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

####print_memory_report
```C++
void print_memory_report() const;
```
Prints the currently alive allocations and its size.

###Functions
####print
```C++
template<typename TStream, typename ... TArgs>
inline static void print(TStream& out, TArgs&& ... args);
```
Given an output stream and what to output, this function will print the provided `args` in a thread-safe manner.
####println
```C++
template<typename TStream, typename ... TArgs>
inline static void println(TStream& out, TArgs&& ... args);
```
Same as above but with a new line at the end.
