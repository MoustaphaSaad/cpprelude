# Struct `Dynamic_Array`
```C++
template<typename T>
struct Dynamic_Array;
```
 - **brief:**      A Dynamic array container
## Typedef `Data_Type`
```C++
using Data_Type = T;
```
 Range Type of dynamic array
## VarDecl `GROW_FACTOR`
```C++
constexpr static usize GROW_FACTOR = 2;
```
 Iterator type of this container
## Variable `mem_context`
```C++
Memory_Context mem_context;
```
 Memory context used by the container
## Default Constructor `Dynamic_Array<T>`
```C++
Dynamic_Array(const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a dynamic array that uses the provided memory context for allocation
## Cast Constructor `Dynamic_Array<T>`
```C++
Dynamic_Array(std::initializer_list<Data_Type> list,
	const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a dynamic array from an initializer list and memory context
## Cast Constructor `Dynamic_Array<T>`
```C++
Dynamic_Array(usize count, const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a dynamic array with the provided count and memory context
## Constructor `Dynamic_Array<T>`
```C++
Dynamic_Array(usize count, const Data_Type& fill_value,
	const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a dynamic array with the provided count and fills it with the fill_value
## Copy Constructor `Dynamic_Array<T>`
```C++
Dynamic_Array(const Dynamic_Array<Data_Type>& other);
```
 - **brief:**      Copy Constructor
## Constructor `Dynamic_Array<T>`
```C++
Dynamic_Array(const Dynamic_Array<Data_Type>& other, const Memory_Context& context);
```
 - **brief:**      Copy constructor with another context
## Move Constructor `Dynamic_Array<T>`
```C++
Dynamic_Array(Dynamic_Array<Data_Type>&& other);
```
 - **brief:**      Move constructor
## CXXDestructor `~Dynamic_Array<T>`
```C++
~Dynamic_Array();
```
 - **brief:**      Destroys the dynamic array.
## Funciton `operator=`
```C++
Dynamic_Array<Data_Type>&
operator=(const Dynamic_Array<Data_Type>& other);
```
 - **brief:**      Copy assignment operator
## Funciton `operator=`
```C++
Dynamic_Array<Data_Type>&
operator=(Dynamic_Array<Data_Type>&& other);
```
 - **brief:**      Move assignment operator
## Funciton `capacity`
```C++
usize
capacity() const;
```
 - **return:**     The capacity of this dynamic array
## Funciton `empty`
```C++
bool
empty() const;
```
 - **return:**     Whether the dynamic array is empty
## Funciton `count`
```C++
usize
count() const;
```
 - **return:**     The count of values in this dynamic array
## Funciton `data`
```C++
const T*
data() const;
```
 - **return:**     A Const Pointer to the content of the dynamic array
## Funciton `data`
```C++
T*
data();
```
 - **return:**     A Pointer to the content of the dynamic array
## Funciton `operator[]`
```C++
Data_Type&
operator[](usize index);
```
 - **param[in]:**  index  The index of the value
## Funciton `operator[]`
```C++
const Data_Type&
operator[](usize index) const;
```
 - **param[in]:**  index  The index of the value
## Funciton `reserve`
```C++
void
reserve(usize expected_count);
```
 - **brief:**      Ensures that the dynamic array has the capacity for the expected count
## Funciton `expand_back`
```C++
void
expand_back(usize additional_count);
```
 - **brief:**      Adds the specified count of element to the back of the dynamic array
## Funciton `expand_back`
```C++
void
expand_back(usize additional_count, const Data_Type& value);
```
 - **brief:**      Adds the specified count of element and initializes them to the provided value
## Funciton `shrink_back`
```C++
void
shrink_back(usize shrinkage_count);
```
 - **brief:**      Removes element of the back of the dynamic array
## Funciton `expand_back_raw`
```C++
void
expand_back_raw(usize additional_count);
```
 - **brief:**      Adds the specified count of elements **Uninitialized** to the back of the array
## Funciton `shrink_back_raw`
```C++
void
shrink_back_raw(usize shrinkage_count);
```
 - **brief:**      Removes the specified count of elements **Undestructed** of the back of the array
## Funciton `resize`
```C++
void
resize(usize new_count);
```
 - **brief:**      Changes the count of the array to be the new provided count and ensures the existance of the needed memory
## Funciton `resize_raw`
```C++
void
resize_raw(usize new_count);
```
 - **brief:**      Changes the count of the array to be the new provided count and ensures the existance of the needed memory, and it does so in an uninitialized/undestructed way.
## Funciton `emplace_back`
```C++
template<typename ... TArgs>
void
emplace_back(TArgs&& ... args);
```
 - **brief:**      Adds a value to the back of the array in place
## Funciton `insert_back`
```C++
void
insert_back(const Data_Type& value);
```
 - **brief:**      Adds a value to the back of the array
## Funciton `insert_back`
```C++
void
insert_back(Data_Type&& value);
```
 - **brief:**      Adds a value to the back of the array
## Funciton `remove_back`
```C++
void
remove_back(usize removal_count = 1);
```
 - **brief:**      Removes the specified count of values from the back of the array
## Funciton `clear`
```C++
void
clear();
```
 - **brief:**      Removes all the values of the array
## Funciton `reset`
```C++
void
reset();
```
 - **brief:**      Removes all the values of the array and frees the memory
## Funciton `shrink_to_fit`
```C++
void
shrink_to_fit();
```
 - **brief:**      Shrinks the memory of the dynamic array to fit the exact count of values inside
## Funciton `all`
```C++
Range_Type
all();
```
 - **return:**     Range viewing all the values in the array
## Funciton `all`
```C++
Const_Range_Type
all() const;
```
 - **return:**     Const range viewing all the values in the array
## Funciton `range`
```C++
Range_Type
range(usize start, usize end);
```
 - **param[in]:**  start  The start index of the range
## Funciton `range`
```C++
Const_Range_Type
range(usize start, usize end) const;
```
 - **param[in]:**  start  The start index of the range
## Funciton `range`
```C++
Range_Type
range(iterator start, iterator end);
```
 - **param[in]:**  start   The start iterator of the range
## Funciton `range`
```C++
Const_Range_Type
range(const_iterator start, const_iterator end) const;
```
 - **param[in]:**  start   The start iterator of the range
## Funciton `front`
```C++
Data_Type&
front();
```
 - **return:**     A Reference to the front value in the array
## Funciton `front`
```C++
const Data_Type&
front() const;
```
 - **return:**     A Const Reference to the front value in the array
## Funciton `back`
```C++
Data_Type&
back();
```
 - **return:**     A Reference to the back value in the array
## Funciton `back`
```C++
const Data_Type&
back() const;
```
 - **return:**     A Const reference to the back value in the array
## Funciton `begin`
```C++
iterator
begin();
```
 - **return:**     An Iterator to the beginning of this container
## Funciton `begin`
```C++
const_iterator
begin() const;
```
 - **return:**     A Const iterator to the beginning of this container
## Funciton `cbegin`
```C++
const_iterator
cbegin() const;
```
 - **return:**     A Const iterator to the beginning of this container
## Funciton `end`
```C++
iterator
end();
```
 - **return:**     An Iterator to the end of the container
## Funciton `end`
```C++
const_iterator
end() const;
```
 - **return:**     A Const Iterator to the end of the container
## Funciton `cend`
```C++
const_iterator
cend() const;
```
 - **return:**     A Const Iterator to the end of the container