#memory_watcher
A handy tool to watch a memory in a specific scope.

##memory_watcher_t
```C++
struct memory_watcher_t;
```
If declared at a scope, it will monitor the memory till the end of the scope. And at the end if there's a memory that's allocated during the watcher lifetime that's not yet freed, it will print the name of the watcher and the leaked memory's count and size. If there're no leak detected, it will be destructed in silence.

###Interface
####Constructor
```C++
memory_watcher_t(const char* scope_name = nullptr);
```
Given a name, it constructs a memory watcher.
####begin_watching
```C++
void begin_watching();
```
Samples the platform's alive allocations and its size.
####end_watching
```C++
void end_watching();
```
Samples the platform's alive allocations and its size.
####delta_alive_allocations
```C++
usize delta_alive_allocations() const;
```
Returns the count of alive allocations from the start watching till now.
####delta_size
```C++
usize delta_size() const;
```
Returns the delta size of allocated memory from the start watching till now.
####print_report
```c++
void print_report() const;
```
Prints the memory report, if there're leaked memory and prints nothing otherwise.