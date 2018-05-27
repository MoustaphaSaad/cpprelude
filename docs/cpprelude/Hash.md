# Struct `Hash`
```C++
template<typename T>
struct Hash;
```
 - **brief:**      Hash functor 
## Funciton `operator()`
```C++
inline usize
operator()(T value) const;
```
 - **brief:**    Call operator to hash the given value
# Funciton `murmur_hash`
```C++
inline static usize
murmur_hash(const void* ptr, usize len, usize seed = 0xc70f6907UL);
```
 - **brief:**      murmur hash function implmentation