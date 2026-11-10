# C++ Pointers & References

## 1. References (`&`)

A reference is an **alias for an existing variable**. It does not create a copy or a separate object.

```cpp
int x = 10;
int& ref = x;

ref = 20; // Changes x to 20
```

Key characteristics:
- Must be initialized when declared.
- Cannot be reassigned to reference another variable.
- Cannot be null.
- Accessed directly, without dereferencing.
- Generally used when a function requires an existing object.

### Const references

```cpp
const int& ref = x;
```

- Can read `x`.
- Cannot modify `x` through `ref`.
- Does not prevent other non-const references from modifying `x`.
- Particularly useful for passing large objects to functions without copying them.

```cpp
void print_name(const std::string& name);
```

This accepts a string without copying it and prevents the function from modifying it through that reference.

---

## 2. Pointers (`*`)

A pointer is a variable that stores the **memory address of another object**.

```cpp
int x = 10;
int* ptr = &x;

*ptr = 20; // Changes x to 20
```

Key operators:

| Operator | Meaning |
|---|---|
| `&x` | Get the address of x |
| `int* ptr` | Declare a pointer to an int |
| `*ptr` | Dereference: access the pointed-to value |
| `ptr->name` | Access a member through a pointer |
| `ptr = nullptr` | Set pointer to no object |

Key characteristics:
- Can be reassigned to point to different objects.
- Can be `nullptr` (point to nothing).
- Can be dereferenced to access the pointed-to object.
- Can be used for optional relationships, dynamic allocation, arrays, and low-level memory operations.

```cpp
int x = 10;
int y = 20;

int* ptr = &x;
ptr = &y; // Now points to y
```

**Important:** Never dereference a null, dangling, or otherwise invalid pointer.

---

## 3. Pointer vs. Reference

| Feature | Pointer | Reference |
|---|---|---|
| Stores/represents | Address | Alias |
| Must initialize | No | Yes |
| Can be null | Yes | No |
| Can be reassigned | Yes | No |
| Access value | `*ptr` | `ref` |
| Member access | `ptr->member` | `ref.member` |
| Can modify target | Yes, if permitted | Yes, if permitted |

Example:

```cpp
int x = 10;
int y = 20;

int* ptr = &x;
int& ref = x;

ptr = &y; // Pointer now refers to y
ref = y;  // Assigns y's VALUE to x
           // ref still refers to x
```

**General guideline:**
- Use references when an object must exist.
- Use pointers when an object might not exist, or when you need to change which object you're accessing.

---

## 4. `const` and Pointers

This is one of the most important distinctions in C++.

`const` applies to the thing immediately to its right, or to its left if nothing is on its right.

There are three important combinations:

### A. Pointer to constant data

```cpp
const std::string* ptr = &name;
```

Equivalent:

```cpp
std::string const* ptr = &name;
```

- Cannot modify the string through this pointer.
- Can redirect the pointer to another string.
- The pointer itself is NOT constant.

```cpp
std::string a = "John";
std::string b = "Jane";

const std::string* ptr = &a;

ptr = &b;       // VALID: pointer can change
*ptr = "Mike";  // INVALID: cannot modify string through ptr
```

### B. Constant pointer to mutable data

```cpp
std::string* const ptr = &name;
```

- Can modify the string through this pointer.
- Cannot redirect the pointer to another string.
- The pointer itself IS constant.

```cpp
std::string a = "John";
std::string b = "Jane";

std::string* const ptr = &a;

*ptr = "Mike";  // VALID: modifies a
ptr = &b;       // INVALID: pointer cannot change
```

### C. Constant pointer to constant data

```cpp
const std::string* const ptr = &name;
```

- Cannot modify the string through this pointer.
- Cannot redirect the pointer.
- Both the pointer and its access to the string are constant.

This is the fully constant pointer form.

### Quick reference

| Declaration | Modify target? | Redirect pointer? |
|---|---|---|
| `std::string* ptr` | Yes | Yes |
| `const std::string* ptr` | No | Yes |
| `std::string* const ptr` | Yes | No |
| `const std::string* const ptr` | No | No |

**Important:** `const` restricts modification through that particular access path. It does not necessarily make the underlying object immutable. Another non-const pointer or reference may still modify it.

---

## 5. Const References

The same principle applies to references:

```cpp
std::string& ref = name;
const std::string& ref = name;
```

- `std::string&`: can modify the original string.
- `const std::string&`: can read but not modify the original string through this reference.

Unlike pointers, references cannot be redirected or set to null.

---

## 6. Function Parameters and Return Values

Pointers and references are frequently used to control how functions interact with objects.

```cpp
// Pass by value: creates a copy
void example(std::string name);

// Pass by reference: accesses original object
void example(std::string& name);

// Pass by const reference: accesses original, read-only
void example(const std::string& name);

// Pass by pointer: accesses original through an address
void example(std::string* name);

// Pass by const pointer: accesses original, read-only
void example(const std::string* name);
```

### Returning references

```cpp
const std::string& get_name() const {
    return name;
}
```

- Returns a reference, not a copy.
- Avoids copying the string.
- Prevents modification through the returned reference.
- The referenced object must remain alive while the caller uses the reference.

**Warning:** Never return a reference or pointer to a local variable. That variable is destroyed when the function exits, leaving a dangling reference or pointer.

---

## 7. `this` Pointer

C++ provides `this` implicitly inside non-static member functions.

It is a pointer to the object currently executing the method.

```cpp
class Player {
private:
    std::string name;

public:
    void set_name(std::string name) {
        this->name = name;
    }
};
```

- `this`: pointer to the current object.
- `this->name`: access the current object's member.
- `*this`: the current object itself.

You do not need `this` unless it improves clarity or resolves a naming conflict.

Static member functions do not have a `this` pointer because they aren't associated with a particular object instance.

---

## 8. Ownership and Smart Pointers

A pointer does not automatically own the object it points to.

Ownership determines **who is responsible for the object's lifetime and destruction**.

C++ provides smart pointers to manage dynamically allocated objects.

### `std::unique_ptr`

Exclusive ownership: only one `unique_ptr` owns the object.

```cpp
#include <memory>

auto player = std::make_unique<Player>();
```

- Automatically destroys the object when ownership ends.
- Cannot be copied.
- Can transfer ownership using `std::move`.

```cpp
auto player2 = std::move(player);
```

Afterward, `player2` owns the object and `player` is empty.

### `std::shared_ptr`

Shared ownership: multiple pointers can own the same object.

```cpp
auto player = std::make_shared<Player>();
auto player2 = player;
```

The object is destroyed when the last owning `shared_ptr` is destroyed.

Use only when shared ownership is genuinely needed. Reference counting has overhead, and ownership cycles can prevent destruction.

### `std::weak_ptr`

A non-owning reference to an object managed by `shared_ptr`.

It allows access without extending the object's lifetime.

Useful for avoiding ownership cycles.

### Raw pointers

```cpp
Player* ptr = player.get();
```

A raw pointer generally represents non-owning access in modern C++.

- Does not automatically destroy the object.
- Does not extend its lifetime.
- Must not be manually deleted if owned by a smart pointer.

**General guideline:**
- `unique_ptr`: exclusive ownership.
- `shared_ptr`: shared ownership.
- `weak_ptr`: non-owning observation of shared ownership.
- Raw pointer/reference: generally non-owning access.

---

## 9. Important Additional Rules

### Pointer arithmetic

Pointers can move through contiguous arrays.

```cpp
int values[] = {10, 20, 30};

int* ptr = values;

ptr++; // Now points to values[1]
```

Pointer arithmetic is scaled by the size of the pointed-to type. It should only be used within the same array (or one position past its end); dereferencing that one-past position is invalid.

### Arrays and pointers

An array often converts to a pointer to its first element:

```cpp
int values[] = {10, 20, 30};

int* ptr = values;

ptr[0]; // 10
ptr[1]; // 20
```

However, an array is not itself a pointer. It has different type and size behavior.

### `nullptr`

Use `nullptr` to represent a pointer that points to nothing.

```cpp
Player* player = nullptr;

if (player != nullptr) {
    player->do_something();
}
```

Prefer `nullptr` over the older `NULL` or integer `0`.

### Const member methods

A trailing `const` on a member function means the method promises not to modify the object's ordinary non-mutable state.

```cpp
class Player {
public:
    std::string get_name() const {
        return name;
    }
};
```

This is different from `const` on a return type or parameter.

It also allows the method to be called on a const object.

### References and pointers have lifetime requirements

Neither a reference nor a raw pointer keeps an object alive.

If the object is destroyed, references and pointers to it become invalid.

This is particularly important when returning references, storing pointers in containers, or passing objects between components.

---

## 10. Practical Defaults

| Situation | Preferred approach |
|---|---|
| Small primitive argument | Pass by value |
| Large read-only object | `const T&` |
| Object must be modified | `T&` |
| Optional object / no object | `T*` or `const T*` |
| Exclusive dynamic ownership | `std::unique_ptr<T>` |
| Shared dynamic ownership | `std::shared_ptr<T>` |
| Read-only access through pointer | `const T*` |
| Read-only access through reference | `const T&` |
| Current object inside a method | `this` |

**Core distinction to remember:**

- A reference is an alias to an existing object.
- A pointer stores an address and can be redirected.
- `const` restricts what can be modified through a particular access path.
- Smart pointers manage ownership and object lifetime.
- Raw pointers and references generally provide access, not ownership.