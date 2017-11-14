# String

`cpprelude::string` is a `utf-8` encoded string. It's simply a slice of binary data. This string is immutable once it's created, However you can reassign and copy strings around.

## Interface

```C++
struct string;
```

`cpprelude::string` is a `utf-8` encoded string. It's simply a slice of binary data. This string is immutable once it's created, However you can reassign and copy strings around.

### Meta Interface

```C++
using iterator = rune_iterator;
using const_iterator = rune_iterator;
```

### Function Interface

#### Constructor

```C++
API string();
API string(memory_context_t* context);
API string(const char* data, memory_context_t* context = platform.global_memory);
API string(const slice<byte>& data, memory_context_t* context = platform.global_memory);
API string(slice<byte>&& data, memory_context_t* context = platform.global_memory);
```

1. Creates an empty string.
2. Creates an empty string with a custom allocator.
3. Creates a string from the provided `data` pointer and the provided memory `context`.
4. Creates a string from the provided `data` slice and the provided memory `context`.
5. Creates a string from the provided `data` slice and the provided memory `context`.

#### count

```C++
usize count() const;
```

Returns the count of runes in the string.

#### size

```C++
usize size() const;
```

Returns the size of the string in bytes.

#### capacity

```C++
usize capacity() const;
```

Returns the size of the underlying slice of data.

#### empty

```C++
bool empty() const;
```

Returns whether the string is empty or not.

#### data

```C++
const byte* data() const;
```

Returns a raw pointer to the underlying data.

#### operator[]

```C++
byte operator[](usize index) const;
```

Indexes the string as a byte stream.

#### Comparison operators

```C++
bool operator==(const string& str) const;
bool operator!=(const string& str) const;
bool operator<(const string& str) const;
bool operator>(const string& str) const;
bool operator<=(const string& str) const;
bool operator>=(const string& str) const;
```

Compares two strings lexicographically.

#### begin

```C++
const_iterator begin() const;
const_iterator cbegin() const;
```

Iterator interface of a string which iterates through the runes not the bytes. It returns an iterator to the beginning of the string.

#### end

```C++
const_iterator end() const;
const_iterator cend() const;
```

Iterator interface of a string which iterates through the runes not the bytes. Returns an iterator to the end of the string.

#### _cs

```C++
string operator"" _cs(const char* str, usize str_count);
```

Creates a string from a string literal without allocating any memory.

#### concat

```C++
template<typename ... TArgs>
inline static string concat(TArgs&& ... args);
```

Concats multiple parameters of different types into a string.

## rune

A rune is a UTF-8 character which is 32-bit wide.

## Interface

```c++
struct rune;
```

A rune is a UTF-8 character which is 32-bit wide.

### Function Interface

#### Comparison operators

```C++
bool operator==(const rune& other) const;
bool operator!=(const rune& other) const;
bool operator<(const rune& other) const;
bool operator>(const rune& other) const;
bool operator<=(const rune& other) const;
bool operator>=(const rune& other) const;
```

Compare two different characters lexicographically.