# Struct `Allocator_Trait`
```C++
struct Allocator_Trait;
```
 - **brief:**      The Allocator trait
## Funciton `alloc`
```C++
template<typename T>
Owner<T>
alloc(usize count = 1);
```
 - **brief:**      Allocates the given count of values
## Funciton `free`
```C++
template<typename T>
void
free(Owner<T>& value);
```
 - **brief:**      Frees the underlying memory of the given owner pointer
## Funciton `free`
```C++
template<typename T>
void
free(Owner<T>&& value);
```
 - **brief:**      Frees the underlying memory of the given owner pointer