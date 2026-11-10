# C++ Quick Reference --- `std::string`, `std::vector`, `std::array`

Include the relevant header and use the `std::` namespace prefix.

``` cpp
#include <string>
#include <vector>
#include <array>
```

## `std::string`

``` cpp
std::string name = "Clint";       // Initialize
std::string empty;                // Empty string
std::string copy = name;          // Copy
std::string joined = name + " H"; // Concatenate

name += " Smith";                 // Append
name.size();                      // Character count
name.length();                    // Same as size()
name.empty();                     // true if empty
name.clear();                     // Remove all characters
name[0];                          // Character access (unchecked)
name.at(0);                       // Character access (bounds-checked)
name.front();                     // First character
name.back();                      // Last character
name.substr(0, 3);                 // Substring: start, length
name.find("Smith");               // Position, or std::string::npos
```

Input:

``` cpp
std::cin >> name;                 // Reads until whitespace
std::getline(std::cin, name);     // Reads the whole line
std::getline(std::cin >> std::ws, name); // Whole line after formatted input
```

Common operators: `=`, `+`, `+=`, `==`, `!=`, `<`, `>`, `[]`.

## `std::vector<T>` --- dynamic size

``` cpp
std::vector<int> scores;               // Empty vector
std::vector<int> values(5);            // 5 elements, value-initialized
std::vector<int> fixed(5, 10);         // 5 elements, each 10
std::vector<int> nums = {10, 20, 30};  // Initializer list
```

Operations:

``` cpp
scores.push_back(90);       // Add to end
scores.emplace_back(95);    // Construct element at end
scores.pop_back();          // Remove last element
scores.size();              // Number of elements
scores.empty();             // Is empty?
scores.clear();             // Remove all elements
scores[0];                  // Access (unchecked)
scores.at(0);               // Access (bounds-checked)
scores.front();             // First element
scores.back();              // Last element
```

Iteration:

``` cpp
for (int score : scores) {       // Copies each element
    std::cout << score << '\\n';
}

for (int& score : scores) {      // Mutable reference; can modify
    score += 1;
}

for (const int& score : scores) {// Read-only reference; avoids copying
    std::cout << score << '\\n';
}
```

Common operators: `=`, `==`, `!=`, `[]`. Use `.at()` when bounds
checking is desired.

## `std::array<T, N>` --- fixed size

``` cpp
std::array<int, 4> scores;                 // 4 elements
std::array<int, 4> values = {10, 20, 30, 40};
std::array<int, 4> zeros{};                // Value-initialized to zero
```

Operations:

``` cpp
scores.size();       // Number of elements (always N)
scores.empty();      // Whether N is zero
scores[0];           // Access (unchecked)
scores.at(0);        // Access (bounds-checked)
scores.front();      // First element
scores.back();       // Last element
scores.fill(0);      // Set every element to 0
```

Iteration works the same as `std::vector` using range-based `for` loops.

Common operators: `=`, `==`, `!=`, `[]`.

## Choosing a container

  -----------------------------------------------------------------------
  Type                    Size                    Typical use
  ----------------------- ----------------------- -----------------------
  `std::string`           Dynamic                 Text

  `std::vector<T>`        Dynamic                 Collection that may
                                                  grow or shrink

  `std::array<T, N>`      Fixed                   Collection with a
                                                  known, fixed element
                                                  count
  -----------------------------------------------------------------------

## Quick reminders

-   Indexes start at `0`; the last valid index is `size() - 1` (when
    non-empty).
-   `[]` does not perform bounds checking; `.at()` does.
-   Check `empty()` before accessing `front()`, `back()`, or an element.
-   `std::vector` may reallocate when it grows. Pointers, references,
    and iterators to its elements can then become invalid.
-   `std::string` is not a C-style null-terminated `char` array, though
    `.c_str()` provides a null-terminated view when needed.
