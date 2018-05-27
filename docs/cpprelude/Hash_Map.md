# Struct `Hash_Set`
```C++
template<typename T, typename THash = Hash<T>>
template<typename T, typename THash = Hash<T>>
struct Hash_Set;
```
 - **brief:**      A Hash set container
## Typedef `Data_Type`
```C++
using Data_Type = T;
```
 Data type of the hash set
## Typedef `Range_Type`
```C++
using Range_Type = Hash_Range<const T, internal::HASH_FLAGS>;
```
 Range Type of hash set
## Typedef `Const_Range_Type`
```C++
using Const_Range_Type = Hash_Range<const T, internal::HASH_FLAGS>;
```
 Const Range Type of the hash set
## Typedef `iterator`
```C++
using iterator = Hash_Iterator<const T, internal::HASH_FLAGS>;
```
 Iterator type of this container
## Typedef `const_iterator`
```C++
using const_iterator = iterator;
```
 Const Iterator type of this container
## Variable `mem_context`
```C++
Memory_Context mem_context;
```
 Memory contex used by the container
## Default Constructor `Hash_Set<T, THash>`
```C++
Hash_Set(const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a hash set that uses the provided memory context for allocation
## Cast Constructor `Hash_Set<T, THash>`
```C++
Hash_Set(const THash& hasher, const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a hash set that uses the provided memory context for allocation
## Copy Constructor `Hash_Set<T, THash>`
```C++
Hash_Set(const Hash_Set& other);
```
 - **brief:**      Copy constructor
## Constructor `Hash_Set<T, THash>`
```C++
Hash_Set(const Hash_Set& other, const Memory_Context& context);
```
 - **brief:**      Copy constructor with another memory context
## Move Constructor `Hash_Set<T, THash>`
```C++
Hash_Set(Hash_Set&& other);
```
 - **brief:**      Move constructor
## Funciton `operator=`
```C++
Hash_Set&
operator=(const Hash_Set& other);
```
 - **brief:**      Copy assignment operator
## Funciton `operator=`
```C++
Hash_Set&
operator=(Hash_Set&& other);
```
 - **brief:**      Move assignment operator
## CXXDestructor `~Hash_Set<T, THash>`
```C++
~Hash_Set();
```
 - **brief:**      Destroys the hash set
## Funciton `capacity`
```C++
usize
capacity() const;
```
 - **return:**     The capacity of this hash set
## Funciton `count`
```C++
usize
count() const;
```
 - **return:**     The count of values in this hash set
## Funciton `empty`
```C++
bool
empty() const;
```
 - **return:**     Whether the hash set is empty
## Funciton `reserve`
```C++
void
reserve(usize expected_count);
```
 - **brief:**      Ensures that the hash set has the capacity for the expected count
## Funciton `insert`
```C++
iterator
insert(const Data_Type& value);
```
 - **param[in]:**  value  The value to be inserted
## Funciton `insert`
```C++
iterator
insert(Data_Type&& value);
```
 - **param[in]:**  value  The value to be inserted
## Funciton `lookup`
```C++
iterator
lookup(const Data_Type& value);
```
 - **param[in]:**  value  The value to search for
## Funciton `lookup`
```C++
const_iterator
lookup(const Data_Type& value) const;
```
 - **param[in]:**  value  The value to search for
## Funciton `remove`
```C++
bool
remove(const Data_Type& value);
```
 - **param[in]:**  value  The value to be removed
## Funciton `remove`
```C++
bool
remove(const iterator& it);
```
 - **param[in]:**  it    The iterator to the value to be removed
## Funciton `clear`
```C++
void
clear();
```
 - **brief:**      Removes all the values of the hash set
## Funciton `reset`
```C++
void
reset();
```
 - **brief:**      Removes all the values of the hash set and frees the memory
## Funciton `all`
```C++
Range_Type
all();
```
 - **return:**     Range viewing all the values in the hash set
## Funciton `all`
```C++
Const_Range_Type
all() const;
```
 - **return:**     Const range viewing all the values in the hash set
## Funciton `range`
```C++
Range_Type
range(usize start, usize end_count);
```
 - **param[in]:**  start  The start index of the range
## Funciton `range`
```C++
Const_Range_Type
range(usize start, usize end_count) const;
```
 - **param[in]:**  start  The start index of the range
## Funciton `range`
```C++
Range_Type
range(iterator start, iterator end_it);
```
 - **param[in]:**  start   The start iterator of the range
## Funciton `range`
```C++
Const_Range_Type
range(const_iterator start, const_iterator end_it) const;
```
 - **param[in]:**  start   The start iterator of the range
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
# Struct `Hash_Map`
```C++
template<typename TKey, typename TValue,
template<typename TKey, typename TValue,
typename THash = internal::Pair_Hash_Functor<TKey, TValue>>;
```
 - **brief:**      A Hash map container
## Typedef `Data_Type`
```C++
using Data_Type = internal::Hash_Pair<const TKey, TValue>;
```
 Data type of the hash map
## Typedef `Range_Type`
```C++
using Range_Type = Hash_Range<Data_Type, internal::HASH_FLAGS>;
```
 Range Type of hash map
## Typedef `Const_Range_Type`
```C++
using Const_Range_Type = Hash_Range<const Data_Type, internal::HASH_FLAGS>;
```
 Const Range Type of the hash map
## Typedef `iterator`
```C++
using iterator = Hash_Iterator<Data_Type, internal::HASH_FLAGS>;
```
 Iterator type of this container
## Typedef `const_iterator`
```C++
using const_iterator = Hash_Iterator<const Data_Type, internal::HASH_FLAGS>;
```
 Const Iterator type of this container
## Variable `mem_context`
```C++
Memory_Context mem_context;
```
 Memory contex used by the container
## Default Constructor `Hash_Map<TKey, TValue, THash>`
```C++
Hash_Map(const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a hash map that uses the provided memory context for allocation
## Cast Constructor `Hash_Map<TKey, TValue, THash>`
```C++
Hash_Map(const THash& hasher, const Memory_Context& context = os->global_memory);
```
 - **brief:**      Constructs a hash map that uses the provided memory context for allocation
## Copy Constructor `Hash_Map<TKey, TValue, THash>`
```C++
Hash_Map(const Hash_Map& other);
```
 - **brief:**      Copy constructor
## Constructor `Hash_Map<TKey, TValue, THash>`
```C++
Hash_Map(const Hash_Map& other, const Memory_Context& context);
```
 - **brief:**      Copy constructor with another memory context
## Move Constructor `Hash_Map<TKey, TValue, THash>`
```C++
Hash_Map(Hash_Map&& other);
```
 - **brief:**      Move constructor
## Funciton `operator=`
```C++
Hash_Map&
operator=(const Hash_Map& other);
```
 - **brief:**      Copy assignment operator
## Funciton `operator=`
```C++
Hash_Map&
operator=(Hash_Map&& other);
```
 - **brief:**      Move assignment operator
## CXXDestructor `~Hash_Map<TKey, TValue, THash>`
```C++
~Hash_Map();
```
 - **brief:**      Destroys the hash map
## Funciton `capacity`
```C++
usize
capacity() const;
```
 - **return:**     The capacity of this hash map
## Funciton `count`
```C++
usize
count() const;
```
 - **return:**     The count of values in this hash map
## Funciton `empty`
```C++
bool
empty() const;
```
 - **return:**     Whether the hash map is empty
## Funciton `reserve`
```C++
void
reserve(usize expected_count);
```
 - **brief:**      Ensures that the hash map has the capacity for the expected count
## Funciton `insert`
```C++
iterator
insert(const Data_Type& value);
```
 - **param[in]:**  value  The value to be inserted
## Funciton `insert`
```C++
iterator
insert(Data_Type&& value);
```
 - **param[in]:**  value  The value to be inserted
## Funciton `insert`
```C++
iterator
insert(const TKey& key);
```
 - **param[in]:**  key  The key to be inserted, and value of this key will be default initialized
## Funciton `insert`
```C++
iterator
insert(TKey&& key);
```
 - **param[in]:**  key  The key to be inserted, and value of this key will be default initialized
## Funciton `insert`
```C++
iterator
insert(const TKey& key, const TValue& value);
```
 - **param[in]:**  key  The key to be inserted
## Funciton `insert`
```C++
iterator
insert(const TKey& key, TValue&& value);
```
 - **param[in]:**  key  The key to be inserted
## Funciton `insert`
```C++
iterator
insert(TKey&& key, const TValue& value);
```
 - **param[in]:**  key  The key to be inserted
## Funciton `insert`
```C++
iterator
insert(TKey&& key, TValue&& value);
```
 - **param[in]:**  key  The key to be inserted
## Funciton `operator[]`
```C++
TValue&
operator[](const TKey& key);
```
 - **brief:**      Access a value of the hash map with the given key. if the value doesn't exist it will be added with a default initialized value
## Funciton `operator[]`
```C++
TValue&
operator[](TKey&& key);
```
 - **brief:**      Access a value of the hash map with the given key. if the value doesn't exist it will be added with a default initialized value
## Funciton `lookup`
```C++
iterator
lookup(const Data_Type& value);
```
 - **param[in]:**  value  The value to search for
## Funciton `lookup`
```C++
const_iterator
lookup(const Data_Type& value) const;
```
 - **param[in]:**  value  The value to search for
## Funciton `lookup`
```C++
iterator
lookup(const TKey& key);
```
 - **param[in]:**  key  The key to search for
## Funciton `lookup`
```C++
const_iterator
lookup(const TKey& key) const;
```
 - **param[in]:**  key  The key to search for
## Funciton `remove`
```C++
bool
remove(const Data_Type& value);
```
 - **param[in]:**  value  The value to be removed
## Funciton `remove`
```C++
bool
remove(const TKey& key);
```
 - **param[in]:**  key  The key to be removed
## Funciton `remove`
```C++
bool
remove(const iterator& it);
```
 - **param[in]:**  it    The iterator to the value to be removed
## Funciton `remove`
```C++
bool
remove(const const_iterator& it);
```
 - **param[in]:**  it    The iterator to the value to be removed
## Funciton `clear`
```C++
void
clear();
```
 - **brief:**      Removes all the values of the hash map
## Funciton `reset`
```C++
void
reset();
```
 - **brief:**      Removes all the values of the hash map and frees the memory
## Funciton `all`
```C++
Range_Type
all();
```
 - **return:**     Range viewing all the values in the hash map
## Funciton `all`
```C++
Const_Range_Type
all() const;
```
 - **return:**     Const range viewing all the values in the hash map
## Funciton `range`
```C++
Range_Type
range(usize start, usize end_count);
```
 - **param[in]:**  start  The start index of the range
## Funciton `range`
```C++
Const_Range_Type
range(usize start, usize end_count) const;
```
 - **param[in]:**  start  The start index of the range
## Funciton `range`
```C++
Range_Type
range(iterator start, iterator end_it);
```
 - **param[in]:**  start   The start iterator of the range
## Funciton `range`
```C++
Const_Range_Type
range(const_iterator start, const_iterator end_it) const;
```
 - **param[in]:**  start   The start iterator of the range
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