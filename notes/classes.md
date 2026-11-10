# C++ Classes — Syntax & Reference

## 1. Class Declaration

```cpp
class Player {
private:
    std::string name;
    int score;

public:
    void print_score() {
        std::cout << score << '\n';
    }
};
```

- `class` declares a class.
- Members are `private` by default.
- `public` members are accessible externally.
- `private` members are accessible only within the class and its friends.
- `protected` members are accessible within the class, its friends, and derived classes.
- The class declaration ends with a semicolon (`;`).

### `class` vs. `struct`

Both support constructors, methods, inheritance, and access control.

The defaults differ:

| Feature | `class` | `struct` |
|---|---|---|
| Default member access | private | public |
| Default inheritance access | private | public |

Use `class` when encapsulation is important; `struct` is commonly used for simple data structures.

---

## 2. Constructors

A constructor initializes an object when it is created.

```cpp
class Player {
private:
    std::string name;
    int score;

public:
    Player(std::string player_name, int player_score)
        : name(player_name), score(player_score)
    {
    }
};
```

- Constructor name matches the class name.
- Has no return type, not even `void`.
- Can accept parameters.
- Can be overloaded.
- Executes automatically when an object is constructed.

### Member initializer list

The `:` introduces the initializer list.

```cpp
Player(std::string name, int score)
    : name(name), score(score)
{
}
```

This initializes the members directly before the constructor body executes.

This is preferred over assigning values inside the body.

Initializer lists are required for:
- `const` data members.
- Reference data members.
- Members whose types have no default constructor.
- Explicitly initializing a parent class.

**Important:** Members are initialized in the order they are declared in the class, not the order written in the initializer list.

### Default constructor

A constructor that can be called without arguments.

```cpp
Player() : name("Unknown"), score(0) {}
```

If you declare your own parameterized constructor, C++ does not automatically generate a default constructor.

You can explicitly request one:

```cpp
Player() = default;
```

### Constructor delegation

One constructor can call another constructor in the same class.

```cpp
Player() : Player("Unknown", 0) {}

Player(std::string name, int score)
    : name(name), score(score) {}
```

This avoids duplicating initialization logic.

---

## 3. Destructors

A destructor executes when an object's lifetime ends.

```cpp
class Player {
public:
    ~Player() {
        // Cleanup
    }
};
```

- Same name as class, prefixed with `~`.
- No return type or parameters.
- Only one destructor per class.
- Called automatically when an object is destroyed.

Common uses:
- Releasing resources.
- Closing files.
- Releasing manually acquired resources.
- Cleanup of owned objects.

**C++ concept: RAII**

Resource Acquisition Is Initialization.

The general idea is that an object's constructor acquires resources and its destructor releases them. This is why C++ often avoids manual cleanup calls.

Smart pointers, strings, vectors, and other standard library types already manage their resources.

---

## 4. Creating Objects

```cpp
Player player("John", 100);
```

Creates an object directly.

```cpp
Player player2 = Player("Jane", 200);
```

Also constructs an object, although the first form is generally simpler.

### Dynamic allocation

```cpp
Player* player = new Player("John", 100);

delete player;
```

- `new` allocates and constructs an object.
- `delete` destroys it and releases its allocation.
- Forgetting `delete` can cause memory leaks.

Prefer smart pointers in modern C++:

```cpp
auto player = std::make_unique<Player>("John", 100);
```

This automatically manages the object's lifetime.

---

## 5. Accessing Members

The accessor depends on whether you have an object or a pointer.

```cpp
Player player("John", 100);

player.print_score();
```

Use `.` with an object.

```cpp
Player* player = &some_player;

player->print_score();
```

Use `->` with a pointer.

Equivalent pointer syntax:

```cpp
(*player).print_score();
```

`->` is shorthand for dereferencing and accessing a member.

---

## 6. Getters and Setters

C++ does not have Python-style properties built into the language.

You generally implement getters and setters explicitly.

```cpp
class Player {
private:
    std::string name;
    int score;

public:
    const std::string& get_name() const {
        return name;
    }

    void set_name(const std::string& new_name) {
        if (!new_name.empty()) {
            name = new_name;
        }
    }
};
```

### Important syntax

```cpp
const std::string& get_name() const
```

- First `const`: returned string cannot be modified through the reference.
- `&`: returns a reference instead of copying the string.
- Trailing `const`: method promises not to modify the object's ordinary state.

A `const` object can call only methods that are themselves `const` (or otherwise appropriately qualified).

---

## 7. Methods Defined Outside the Class

You can declare methods inside the class and define them separately.

**player.hpp**

```cpp
class Player {
private:
    std::string name;

public:
    Player(std::string name);
    void print_name() const;
};
```

**player.cpp**

```cpp
#include "player.hpp"
#include <iostream>

Player::Player(std::string name)
    : name(name)
{
}

void Player::print_name() const {
    std::cout << name << '\n';
}
```

The `::` is the **scope resolution operator**.

`Player::print_name` means the method `print_name` belonging to `Player`.

This is also used for:
- Namespaces: `std::string`
- Static members: `Player::count`
- Nested types: `Player::Status`

---

## 8. Static Members

A static member belongs to the class itself rather than an individual object.

```cpp
class Player {
public:
    inline static int player_count = 0;

    Player() {
        player_count++;
    }
};
```

Access it through the class:

```cpp
Player::player_count;
```

Or through an object, although class access is clearer.

### Static methods

```cpp
class Player {
public:
    static int get_count() {
        return player_count;
    }

private:
    inline static int player_count = 0;
};
```

- Can be called without creating an object.
- Do not have access to `this`.
- Cannot directly access non-static members because those belong to individual objects.

`inline static` allows a static data member to be defined inside the class (C++17 onward).

---

## 9. Inheritance

C++ uses `:` to declare inheritance.

```cpp
class Game {
public:
    void start() {}
};

class Pinochle : public Game {
};
```

`Pinochle` inherits publicly from `Game`.

### Inheritance access

```cpp
class Derived : public Base {};
class Derived : protected Base {};
class Derived : private Base {};
```

| Inheritance | Base public members become | Base protected members become |
|---|---|---|
| `public` | public | protected |
| `protected` | protected | protected |
| `private` | private | private |

Base-class private members are never directly accessible in derived classes.

For normal OOP inheritance, `public` is the usual choice.

### Parent constructor initialization

```cpp
class Game {
public:
    Game(int id) {}
};

class Pinochle : public Game {
public:
    Pinochle(int id) : Game(id) {}
};
```

The derived constructor initializes its base class through the initializer list.

---

## 10. Virtual Methods and Polymorphism

By default, C++ selects methods based on the declared type at compile time.

`virtual` enables runtime method dispatch through a base-class pointer or reference.

```cpp
class Game {
public:
    virtual void calculate_score() {
        // Default behavior
    }

    virtual ~Game() = default;
};

class Pinochle : public Game {
public:
    void calculate_score() override {
        // Pinochle-specific behavior
    }
};
```

### Important keywords

- `virtual`: enables runtime dispatch.
- `override`: explicitly declares that a method overrides a virtual base method.
- `final`: prevents further overriding or inheritance, depending on where used.

### Pure virtual methods

```cpp
class Game {
public:
    virtual void calculate_score() = 0;
    virtual ~Game() = default;
};
```

`= 0` makes the method pure virtual.

This makes `Game` an abstract class. It cannot be instantiated directly.

Derived classes must implement the pure virtual methods to become concrete.

### Virtual destructors

If a class is intended to be used polymorphically, its destructor should generally be virtual.

This ensures the derived destructor runs when deleting an object through a base-class pointer.

---

## 11. Composition

A class can contain instances of other classes as members.

```cpp
class Player {
    std::string name;
};

class Team {
private:
    Player player1;
    Player player2;
};
```

`Team` contains two `Player` objects.

The contained objects are constructed before the enclosing class's constructor body executes and destroyed after its destructor body completes.

Members are destroyed in reverse declaration order.

Composition is often simpler than inheritance when one object is made up of other objects.

---

## 12. Operator Overloading

C++ allows classes to define behavior for operators.

```cpp
class Score {
public:
    int value;

    Score operator+(const Score& other) const {
        return Score{value + other.value};
    }
};
```

This allows:

```cpp
Score total = score1 + score2;
```

Operators can be overloaded for custom types, but cannot be invented or have their precedence changed.

Use this when operator syntax genuinely makes the type easier to work with.

---

## 13. Friend

`friend` grants another function or class access to private and protected members.

```cpp
class Player {
private:
    int score = 100;

    friend void print_score(const Player& player);
};

void print_score(const Player& player) {
    std::cout << player.score;
}
```

Friendship is explicit and is not automatically inherited or reciprocal.

Use sparingly; it bypasses normal encapsulation.

---

## 14. Copy and Move Behavior

C++ objects can be copied or moved.

```cpp
Player player1("John", 100);

Player player2 = player1;            // Copy
Player player3 = std::move(player1); // Move
```

- Copying creates a separate object with copied state.
- Moving transfers resources where the type supports it.
- `std::move` itself does not move anything; it enables move construction or assignment.

C++ automatically provides certain special member functions when appropriate:
- Default constructor (subject to constructor declarations).
- Copy constructor.
- Copy assignment operator.
- Move constructor (subject to other special member declarations).
- Move assignment operator.
- Destructor.

You can explicitly default or disable operations:

```cpp
Player(const Player&) = delete;
Player& operator=(const Player&) = delete;
```

This prohibits copying.

```cpp
Player(Player&&) = default;
```

This requests a defaulted move constructor.

**Important:** If a class directly manages resources such as raw allocated memory, you must carefully consider copy and move behavior. Prefer standard resource-managing types such as `std::string`, `std::vector`, and smart pointers.

---

## 15. Nested Types and Enums

Classes can contain their own types.

```cpp
class Player {
public:
    enum class Status {
        ACTIVE,
        INACTIVE,
        RETIRED
    };

    Status status = Status::ACTIVE;
};
```

Access the enum through the class:

```cpp
Player::Status::ACTIVE;
```

Nested types are useful for organizing concepts that belong specifically to a class.

---

## 16. Useful C++ Class Rules

| Concept | Rule |
|---|---|
| Class declaration | Ends with `;` |
| Constructor | Same name as class, no return type |
| Destructor | `~ClassName()`, no parameters |
| Initializer list | `:` after constructor parameters |
| Member access | `.` for objects, `->` for pointers |
| Scope resolution | `ClassName::member` |
| `this` | Pointer to current object |
| `const` method | Promises not to modify ordinary object state |
| `static` member | Belongs to class, not individual object |
| `virtual` method | Enables runtime dispatch |
| `override` | Explicitly marks an overridden method |
| `= 0` | Pure virtual method |
| `= default` | Requests compiler-generated behavior |
| `= delete` | Prohibits an operation |
| `friend` | Grants private/protected access |

## Practical Guidelines

1. Use `private` for class state unless public access is intentionally part of the design.
2. Prefer initializer lists for member initialization.
3. Use `const` on methods that do not modify object state.
4. Use `const T&` for large read-only parameters.
5. Prefer composition over inheritance unless a genuine "is-a" relationship exists.
6. Use `override` on derived virtual methods.
7. Use a virtual destructor on polymorphic base classes.
8. Prefer standard library resource-management types over manually managed memory.
9. Avoid making every member function virtual; use runtime polymorphism when it serves a purpose.
10. Keep simple data structures simple. Not every struct needs getters, setters, and a complex class hierarchy.