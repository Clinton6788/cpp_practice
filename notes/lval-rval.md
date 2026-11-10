# C++ Reference: Lvalues, Rvalues, References & Move Semantics

## 1. Lvalues vs. Rvalues

Lvalue and rvalue describe **expressions**, not permanent characteristics of variables.

### Lvalue (Locator Value)

An expression that identifies a specific, existing object.

```cpp
int a = 10;

a;              // Lvalue
int& ref = a;   // ref aliases a
ref = 20;       // Modifies a
```

Common examples:
- Named variables: `a`, `my_string`
- Dereferenced pointers: `*ptr`
- Array elements: `values[0]`
- References when used by name: `ref`

### Rvalue (Right-hand Value)

An expression that is either a temporary value or an expiring object.

```cpp
int a = 10;

10;                 // Prvalue
a + 5;              // Prvalue
std::string("Hi");  // Prvalue
std::move(a);       // Xvalue
```

Rvalues are divided into two important categories:

| Category | Meaning | Example |
|---|---|---|
| Prvalue | A pure temporary value | `10`, `a + 5` |
| Xvalue | An existing object treated as expiring | `std::move(a)` |

Both are rvalues.

### Important distinctions

```cpp
int a = 10;

a = 20;        // a is an lvalue; 20 is an rvalue
int& b = a;    // b is an lvalue reference
b = 30;        // b is an lvalue expression; modifies a
```

**Being an lvalue does NOT mean something cannot receive a new value.**

The distinction is about the expression's category, not whether it can be modified.

---

## 2. References

References provide another name for an existing object.

### Lvalue Reference: `T&`

Binds to an lvalue.

```cpp
int a = 10;

int& ref = a;

ref = 20; // a is now 20
```

Characteristics:
- Must be initialized when declared.
- Cannot be reseated to refer to another object.
- Cannot normally bind to an rvalue.
- Does not create a new object.

### Const Lvalue Reference: `const T&`

Can bind to both lvalues and rvalues.

```cpp
int a = 10;

const int& ref1 = a;
const int& ref2 = 20;
```

Characteristics:
- Cannot modify the object through that reference.
- Can bind to temporary values.
- Binding to a temporary extends its lifetime to the lifetime of the local reference in this situation.

Common use: accepting objects that a function only needs to read.

### Rvalue Reference: `T&&`

Binds to rvalues.

```cpp
int&& ref = 10;
```

An important rule:

**A named rvalue reference is itself an lvalue expression.**

```cpp
int&& ref = 10;

ref = 20; // Valid: ref is an lvalue expression
```

This rule is especially important when implementing move constructors.

---

## 3. Function Overloading With References

C++ can select different function overloads depending on whether an argument is an lvalue or rvalue.

```cpp
#include <iostream>
#include <string>

void inspect(const std::string& value) {
    std::cout << value << " Const Ref\n";
}

void inspect(std::string&& value) {
    std::cout << value << " Rvalue Ref\n";
}

int main() {
    std::string a = "Named String";

    inspect(a);
    inspect(std::string("Temporary String"));
    inspect(std::move(a));
}
```

| Function call | Expression category | Selected overload |
|---|---|---|
| `inspect(a)` | Lvalue | `const std::string&` |
| `inspect(std::string(...))` | Prvalue | `std::string&&` |
| `inspect(std::move(a))` | Xvalue | `std::string&&` |

**Important:** Receiving an rvalue reference does not automatically move anything. The function must actually perform a move operation if that is its intention.

---

## 4. std::move()

Include:

```cpp
#include <utility>
```

`std::move()` does NOT actually move anything.

It converts an expression into an xvalue, allowing operations that accept rvalues (such as move constructors) to be selected.

```cpp
std::string a = "Hello";
std::string b = std::move(a);
```

Conceptually:

1. `a` is an lvalue.
2. `std::move(a)` produces an xvalue referring to the same object.
3. `b` is initialized using the string's move constructor.
4. The move constructor transfers resources from `a` to `b`.

### Critical distinction

```cpp
std::move(a); // Does not move anything by itself
```

A move constructor or move assignment operator must actually perform the transfer.

### After a move

```cpp
std::string a = "Hello";
std::string b = std::move(a);
```

Afterward:
- `b` owns the string's former contents.
- `a` still exists.
- `a` is in a valid but unspecified state.
- `a` is not guaranteed to be empty.
- `a` can be assigned a new value and reused.

Do not rely on the previous contents of a moved-from object.

---

## 5. What Actually Moves?

For types that manage dynamically allocated resources, moving often transfers ownership of the underlying allocation rather than copying all its contents.

For example, a `std::string` may internally contain:
- Metadata (size, capacity, etc.).
- A pointer to dynamically allocated character storage.

A move can transfer that allocation to another string object.

The two string objects still occupy separate object locations. The resource ownership changes.

### Copy vs. Move

```cpp
std::vector<int> a(1000000);

std::vector<int> b = a;            // Copy
std::vector<int> c = std::move(a); // Move
```

| Copy | Move |
|---|---|
| Creates independent contents | Transfers resource ownership when supported |
| Usually allocates new storage | Often reuses existing allocation |
| Copies elements | Usually transfers small amounts of metadata |
| Original remains unchanged | Original becomes valid but unspecified |

For large dynamically allocated containers, moving can be significantly cheaper than copying.

**Caveat:** Small strings may use Small String Optimization (SSO), storing characters inside the string object itself. Moving those strings may involve copying a small amount of data.

---

## 6. Why Not Just Use Pointers?

Pointers, references, copies, and moves serve different purposes.

| Approach | Purpose |
|---|---|
| `const T&` | Read existing object without copying |
| `T&` | Modify existing object |
| `T*` | Indirect access; can also represent absence |
| Copy (`T`) | Create independent state |
| Move (`std::move`) | Enable transfer of resources/ownership |

### Example: Reading vs. Taking Ownership

```cpp
// Only needs to read the data
void inspect(const std::vector<int>& data);

// Needs its own independent data
void retain(std::vector<int> data);
```

Calling the second function:

```cpp
std::vector<int> values(1000000);

retain(std::move(values));
```

The function receives its own vector object and can take ownership of the existing allocation.

This avoids copying a million elements.

### When to use each

- Read existing data: `const T&`
- Modify existing data: `T&`
- Optionally reference an object: `T*`
- Create independent ownership: Copy
- Transfer existing resources: Move

Move semantics are primarily about ownership and resource management, not simply about passing data between functions.

The receiving function does NOT have to outlive the calling function for moving to be useful.

---

## 7. Pointers, Memory, and Object Lifetimes

C++ automatically destroys local objects when they leave scope.

However, **destroying a raw pointer does not destroy the memory it points to.**

### Automatic Storage

```cpp
void example() {
    int value = 10;
    int* ptr = &value;
}
```

When the function ends:
- `ptr` is destroyed.
- `value` is destroyed.
- No manual cleanup is needed.

### Dynamic Allocation

```cpp
void example() {
    int* ptr = new int(10);
}
```

When the function ends:
- `ptr` is destroyed.
- The dynamically allocated integer remains allocated.
- The memory is leaked because the pointer is lost.

Manual cleanup:

```cpp
delete ptr;
```

### Smart Pointers

C++ provides smart pointers to manage dynamically allocated resources automatically.

Include:

```cpp
#include <memory>
```

#### `std::unique_ptr<T>`

Exclusive ownership. Cannot be copied, but can be moved.

```cpp
std::unique_ptr<int> a = std::make_unique<int>(10);

std::unique_ptr<int> b = std::move(a);
```

Now `b` owns the integer. `a` is empty (`nullptr`).

When `b` leaves scope, its owned integer is automatically destroyed.

#### `std::shared_ptr<T>`

Shared ownership through reference counting.

```cpp
std::shared_ptr<int> a = std::make_shared<int>(10);
std::shared_ptr<int> b = a;
```

Both pointers share ownership. The resource is destroyed when the last owning `shared_ptr` is destroyed or reset.

#### `std::weak_ptr<T>`

Observes an object managed by `shared_ptr` without extending its lifetime.

### Comparison

| Type | Ownership | Automatic cleanup? |
|---|---|---|
| Raw pointer | Not inherently defined | No |
| `unique_ptr` | Exclusive | Yes |
| `shared_ptr` | Shared | Yes, when last owner releases |
| `weak_ptr` | Non-owning observer | Does not own resource |

---

## 8. RAII — Resource Acquisition Is Initialization

RAII is one of C++'s central resource-management principles.

An object acquires resources during construction and releases them during destruction.

Examples:
- `std::string`
- `std::vector`
- `std::unique_ptr`
- `std::shared_ptr`
- File streams
- Mutex locks

This means you generally do not need to manually manage the memory owned by standard-library containers.

```cpp
void example() {
    std::vector<int> values(1000000);
    
    // Use values.
    
} // Vector destructor releases its allocation.
```

RAII makes cleanup predictable, including when functions exit early or exceptions occur.

---

## 9. Quick Reference / Rules to Remember

| Rule | Explanation |
|---|---|
| Named variables are lvalue expressions | Even if declared as `T&&` |
| `std::move()` does not move | It enables moving |
| `T&` | Lvalue reference |
| `T&&` | Rvalue reference |
| `const T&` | Can bind to lvalues and rvalues |
| Moving does not destroy the source object | It remains valid but may have unspecified state |
| Raw pointers do not own memory automatically | Destroying a pointer does not free its allocation |
| Smart pointers manage ownership | RAII handles cleanup |
| Move is not always faster | Depends on the type and its resources |

### The Mental Model

- **Lvalue:** An expression identifying an existing object.
- **Rvalue:** An expression representing a temporary value or expiring object.
- **Reference:** Another way to access an existing object.
- **Pointer:** An indirect way to access an object.
- **Copy:** Create independent state.
- **Move:** Transfer resources/ownership.
- **RAII:** Automatically release resources when their owning object is destroyed.

**Most important takeaway:** Use references when you want access to existing objects, copies when you need independent state, and moves when you want to transfer ownership without unnecessarily duplicating resources.