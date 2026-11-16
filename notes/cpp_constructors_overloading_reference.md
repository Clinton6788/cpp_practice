# C++ Constructors, Overloading, and Special Member Functions

A practical reference for constructor overloads, copy/move behavior, and
the compiler's automatic generation rules.

## 1. What is a constructor?

A constructor is a special member function that initializes an object
when it is created.

-   Its name matches the class name.
-   It has no return type.
-   It runs automatically during object construction.
-   A class can have multiple constructors, as long as their parameter
    lists differ.

``` cpp
#include <string>

class Player {
private:
    std::string name;
    int number;

public:
    Player() : name("Unknown"), number(0) {}

    Player(const std::string& player_name)
        : name(player_name), number(0) {}

    Player(const std::string& player_name, int player_number)
        : name(player_name), number(player_number) {}
};
```

These are constructor overloads. The compiler chooses the matching
constructor based on the arguments used:

``` cpp
Player a;                  // Player()
Player b("Jeff");          // Player(const std::string&)
Player c("George", 7);     // Player(const std::string&, int)
```

The overload must be distinguishable by its parameter list. You cannot
overload constructors solely by changing the return type (constructors
have no return type anyway).

## 2. Overload resolution: how C++ chooses

The compiler considers the number, types, and value categories of the
arguments, along with conversions and other overload-resolution rules.

``` cpp
class Example {
public:
    Example(int value) {}
    Example(double value) {}
    Example(const std::string& value) {}
};
```

``` cpp
Example a(10);       // int overload
Example b(2.5);      // double overload
Example c("Hello"); // string overload is viable through conversion to std::string
```

If multiple overloads are equally good, the call is ambiguous and
compilation fails. Avoid overload sets that invite surprising implicit
conversions.

`explicit` can prevent a single-argument constructor from being used for
unintended implicit conversions:

``` cpp
class Player {
public:
    explicit Player(int player_id) {}
};

Player a(42);       // OK
// Player b = 42;   // Error: explicit constructor
```

Use `explicit` for constructors that should only be used when the caller
clearly asks for that conversion. It is especially useful for
single-argument constructors.

## 3. Member initializer lists

Prefer initializing members in the constructor's member initializer
list:

``` cpp
class Player {
private:
    std::string name;
    int score;

public:
    Player(const std::string& player_name, int initial_score)
        : name(player_name), score(initial_score) {}
};
```

This initializes members directly. Writing assignments in the
constructor body would first default-initialize members and then assign
to them.

Important points:

-   Members are initialized in the order they are declared in the class,
    not the order written in the initializer list.
-   A `const` data member and a reference data member must be
    initialized in the initializer list (or have an in-class initializer
    where permitted).
-   The initializer list is also where base classes and member objects
    are initialized.

## 4. The special member functions

C++ classes have a set of special member functions with language-defined
roles:

  ------------------------------------------------------------------------------------
  Special member          Typical form                         Purpose
  ----------------------- ------------------------------------ -----------------------
  Default constructor     `Player()`                           Creates an object with
                                                               no arguments

  Copy constructor        `Player(const Player&)`              Creates a new object
                                                               from an existing object

  Move constructor        `Player(Player&&)`                   Creates a new object by
                                                               moving from an expiring
                                                               object

  Copy assignment         `Player& operator=(const Player&)`   Replaces an existing
                                                               object's state by
                                                               copying

  Move assignment         `Player& operator=(Player&&)`        Replaces an existing
                                                               object's state by
                                                               moving

  Destructor              `~Player()`                          Runs when the object's
                                                               lifetime ends
  ------------------------------------------------------------------------------------

Copy/move constructors are constructors. Copy/move assignment operators
are not constructors: they operate on an object that already exists.

### Construction versus assignment

``` cpp
Player first("Jeff");

Player second = first; // Copy construction: second is being created
Player third("Sam");
third = first;         // Copy assignment: third already exists
```

The `=` in `Player second = first` is initialization syntax, so it
invokes a constructor. The `=` in `third = first` invokes an assignment
operator.

## 5. Copy constructor and copy assignment

A copy constructor typically takes a `const` reference to another
object:

``` cpp
Player(const Player& other);
```

A copy assignment operator typically looks like:

``` cpp
Player& operator=(const Player& other);
```

The compiler can generate memberwise copy behavior. For members such as
`std::string` and `std::vector`, this copies their values using their
own copy operations.

You can explicitly ask the compiler to generate the normal
implementation:

``` cpp
Player(const Player&) = default;
Player& operator=(const Player&) = default;
```

`= default` does **not** mean "empty function" or "do nothing." It means
"generate the compiler's default implementation for this special
member."

If the implicit implementation is already suitable and you have not
otherwise changed the generation rules, you usually do not need to write
these declarations.

## 6. Move constructor and move assignment

A move constructor typically takes an rvalue reference:

``` cpp
Player(Player&& other);
```

A move assignment operator typically looks like:

``` cpp
Player& operator=(Player&& other);
```

The move constructor creates a new object from another object that can
be treated as expiring. Move assignment replaces the state of an
already-existing object.

For a class containing `std::string` and `std::vector`, their own move
operations can transfer the managed resources efficiently:

``` cpp
#include <string>
#include <vector>
#include <utility>

class Player {
private:
    std::string name;
    std::vector<int> scores;

public:
    Player(Player&& other) noexcept
        : name(std::move(other.name)),
          scores(std::move(other.scores)) {}

    Player& operator=(Player&& other) noexcept {
        name = std::move(other.name);
        scores = std::move(other.scores);
        return *this;
    }
};
```

`std::move` does not itself move anything. It casts its argument to an
rvalue expression so that a suitable move operation can be selected.

A named variable is an lvalue expression, even if its declared type is
an rvalue reference. Therefore, inside the move constructor,
`other.name` is an lvalue expression; `std::move(other.name)` enables
moving from it.

After a move, the source object remains alive and valid, but its value
is generally unspecified. Do not assume that it is empty unless the
relevant type explicitly guarantees that.

`noexcept` is commonly used on move operations when they genuinely
cannot throw. Standard containers may prefer copying over a potentially
throwing move in some reallocation situations.

## 7. What does the compiler generate automatically?

For an ordinary class, the compiler may implicitly declare and define
special members. The rules are conditional; they are not simply "write
one and all the others disappear."

A useful starting point:

``` cpp
class Player {
    std::string name;
    std::vector<int> scores;
};
```

For this class, the compiler can provide default construction (subject
to member requirements), copying, moving, assignment, and destruction as
appropriate. The generated operations act on the members.

### Critical generation rules

The following are the key rules to remember:

1.  **A default constructor is implicitly declared only if the class has
    no user-declared constructors.** If you declare
    `Player(const std::string&)`, the compiler does not also
    automatically provide `Player()`. Add `Player() = default;` if you
    want the default constructor too.
2.  **A user-declared copy constructor suppresses implicit move
    constructor and move assignment generation.**
3.  **A user-declared copy assignment operator suppresses implicit move
    constructor and move assignment generation.**
4.  **A user-declared move constructor suppresses implicit copy
    constructor and copy assignment generation.** The copy operations
    are defined as deleted in this situation unless explicitly provided.
5.  **A user-declared move assignment operator suppresses implicit copy
    constructor and copy assignment generation.** The copy operations
    are defined as deleted in this situation unless explicitly provided.
6.  **A user-declared destructor suppresses implicit move constructor
    and move assignment generation.**
7.  The compiler may still declare a special member but define it as
    deleted if a member or base class cannot support the required
    operation.

These are special-member generation rules, not a general rule for every
overloaded function. Adding an ordinary constructor overload does not
automatically suppress copy operations; it does suppress the implicit
*default constructor*.

### The important example from the tutorial

``` cpp
class Player {
public:
    Player(const Player&) = default;
    Player& operator=(const Player&) = default;

    Player(Player&& other) noexcept
        : name(std::move(other.name)),
          scores(std::move(other.scores)) {}

private:
    std::string name;
    std::vector<int> scores;
};
```

The explicitly declared copy constructor and copy assignment operator
suppress automatic move generation. Therefore, if this class should
support moving, a move constructor must be explicitly declared (or
defaulted).

The copy operations are written with `= default` to request the normal
copy implementations. They are not custom copy logic.

If you remove all four copy/move declarations, the compiler can generate
the appropriate copy and move operations for this class automatically.

## 8. `= default` versus `= delete`

These declarations have opposite purposes:

``` cpp
Player(const Player&) = default; // Request compiler's normal implementation
Player(const Player&) = delete;  // Prohibit copying
```

Use `= default` when you want the normal special-member behavior but
need to declare it explicitly.

Use `= delete` when an operation must not be available:

``` cpp
class UniqueResource {
public:
    UniqueResource(const UniqueResource&) = delete;
    UniqueResource& operator=(const UniqueResource&) = delete;
};
```

This class cannot be copied through those operations. It may still be
movable if suitable move operations are provided and the other
generation rules allow them.

A defaulted operation can itself be defined as deleted if its members or
bases make the operation impossible.

## 9. Equality is different

The equality operator is not one of the five copy/move/lifetime special
members.

In C++20, you can ask the compiler to generate memberwise equality:

``` cpp
class Player {
public:
    bool operator==(const Player&) const = default;

private:
    std::string name;
    std::vector<int> scores;
};
```

This compares the corresponding members. Without a declared or otherwise
available equality operator, the compiler does not generally invent one
for an arbitrary class.

## 10. Rule of Zero, Three, and Five

These are design guidelines for resource-managing classes.

### Rule of Zero --- preferred

If your class only contains types that manage their own resources
correctly (`std::string`, `std::vector`, smart pointers, etc.), usually
write no destructor, copy operations, or move operations. Let the
compiler-generated operations compose the member types' behavior.

``` cpp
class Player {
private:
    std::string name;
    std::vector<int> scores;

public:
    Player(const std::string& player_name)
        : name(player_name) {}
};
```

This class can use compiler-generated copy/move behavior where the
language rules permit it.

### Rule of Three

If a class manually manages a resource and needs a custom destructor, it
will often also need custom copy construction and copy assignment to
manage that resource safely.

### Rule of Five

When manually managing a resource in modern C++, consider all five
operations:

-   Destructor
-   Copy constructor
-   Copy assignment
-   Move constructor
-   Move assignment

The Rule of Zero is generally preferable. Use the Rule of Five when you
truly own a resource that your member types do not already manage.

## 11. Practical checklist

When writing a class:

-   Do I need a no-argument constructor? If I declare another
    constructor, add `ClassName() = default;` if needed.
-   Can `std::string`, `std::vector`, or another standard type manage
    the resources for me?
-   Am I writing custom copy/move/destructor code unnecessarily?
-   If I declare a copy or move special member, which automatic
    operations does that suppress?
-   Do I want copying prohibited? Use `= delete`.
-   Do I want the compiler's normal implementation but need an explicit
    declaration? Use `= default`.
-   Am I confusing construction (`Player b = a`) with assignment
    (`b = a`)?
-   If I write a move operation, is it genuinely safe to promise
    `noexcept`?

## 12. Quick syntax reference

``` cpp
class Example {
public:
    Example();                              // Default constructor
    Example(int value);                     // Overloaded constructor

    Example(const Example& other);          // Copy constructor
    Example(Example&& other) noexcept;      // Move constructor

    Example& operator=(const Example& other); // Copy assignment
    Example& operator=(Example&& other) noexcept; // Move assignment

    ~Example();                             // Destructor

    bool operator==(const Example& other) const = default;
};
```

Remember: constructor overloads are selected when a new object is
created. Assignment overloads are selected when an existing object is
assigned to. Copy and move special members have additional
automatic-generation and suppression rules, so declaring one can affect
which others the compiler provides.
