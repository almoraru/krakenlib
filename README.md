# 🐙 KRAKENLIB - The Legendary C Library

```
                        ______
                     .-"      "-.
                    /            \
        _          |              |          _
       ( \         |,  .-.  .-.  ,|         / )
        > "=._     | )(__/  \__)( |     _.=" <
       (_/"=._"=._ |/     /\     \| _.="_.="\_)
              "=._ (_     ^^     _)"_.="
                  "=\__|IIIIII|__/="
                 _.="| \IIIIII/ |"=._
       _     _.="_.="\          /"=._"=._     _
      ( \_.="_.="     `--------`     "=._"=._/ )
       > _.="                            "=._ <
      (_/                                    \_)


```

**A powerful, production-ready C library featuring custom implementations of libc functions, malloc, printf, and more!**

[![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C_(programming_language))

---

## 📚 Table of Contents

- [Features](#-features)
- [Installation](#-installation)
- [Usage](#-usage)
- [Modules](#-modules)
- [Testing](#-testing)
- [Performance](#-performance)
- [Documentation](#-documentation)
- [Contributing](#-contributing)
- [License](#-license)

---

## ✨ Features

### 🔧 Core Functions (150+ functions)
- **String manipulation**: `sea_strlen`, `sea_strcpy`, `sea_strdup`, `sea_strjoin`, etc.
- **Memory operations**: `sea_memcpy`, `sea_memset`, `sea_bzero`, `sea_memmove`
- **Character checks**: `sea_isalpha`, `sea_isdigit`, `sea_isalnum`, etc.
- **Conversions**: `sea_atoi`, `sea_itoa`, `sea_atol`, number base conversions

### 💾 Custom Memory Allocator
- **Dynamic memory management** with custom `sea_malloc`, `sea_free`, `sea_realloc`, `sea_calloc`
- **Three-tier allocation strategy**:
  - TINY (≤128 bytes) - 16KB zone size
  - SMALL (129-8192 bytes) - 1MB zone size
  - LARGE (>8192 bytes) - Direct mmap allocation
- **Memory introspection**: `show_alloc_mem()`, `show_alloc_mem_ex()`
- **Thread-safe** with mutex protection
- **Memory efficient** with block reuse and defragmentation

### 🖨️ Custom Printf Implementation
- **Full format specifier support**: `%c`, `%s`, `%d`, `%i`, `%u`, `%x`, `%X`, `%p`, `%%`
- **Flag support**: `-`, `0`, `#`, `+`, space
- **Width and precision** control
- **Return value compatibility** with standard printf

### 📝 Get Next Line (GNL)
- **Efficient line reading** from file descriptors
- **Multiple FD support** (up to 1024 simultaneous files)
- **Buffer management** with configurable buffer size
- **Memory leak free**

### 📋 Linked Lists
- **Complete list operations**: create, add, iterate, map, filter
- **Node management**: `sea_lstnew`, `sea_lstadd_front`, `sea_lstadd_back`
- **Functional operations**: `sea_lstmap`, `sea_lstiter`
- **Memory safe**: proper cleanup with `sea_lstclear`, `sea_lstdelone`

---

## 🚀 Installation

### Prerequisites
- GCC or Clang compiler
- GNU Make
- BSD library (`libbsd-dev` on Ubuntu/Debian)

### Build

```bash
# Clone the repository
git clone https://github.com/almoraru/krakenlib.git
cd krakenlib

# Build the library
make

# Build with bonus functions
make bonus

# Clean build artifacts
make clean

# Full clean (including library)
make fclean

# Rebuild everything
make re
```

### Output
The compiled library will be: `libkraken.a`

---

## 💻 Usage

### Basic Example

```c
#include "krakenlib.h"

int main(void)
{
    // String operations
    char *str = sea_strdup("Hello, Kraken!");
    sea_printf("String: %s\n", str);
    
    // Memory allocation
    int *arr = sea_malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++)
        arr[i] = i;
    
    // Custom printf
    sea_printf("Array[5] = %d\n", arr[5]);
    
    // Get next line
    int fd = open("file.txt", O_RDONLY);
    char *line;
    while (sea_get_line(fd, &line) > 0)
    {
        sea_printf("%s\n", line);
        sea_free(line);
    }
    close(fd);
    
    // Cleanup
    sea_free(str);
    sea_free(arr);
    
    return 0;
}
```

### Compilation

```bash
# Link with krakenlib
gcc -I includes main.c libkraken.a -lbsd -o program

# Run
./program
```

---

## 📦 Modules

### 1. Core Library (`sea_*`)

**String Functions:**
```c
size_t  sea_strlen(const char *s);
char    *sea_strdup(const char *s);
char    *sea_strcpy(char *dst, const char *src);
char    *sea_strjoin(char const *s1, char const *s2);
int     sea_strcmp(const char *s1, const char *s2);
char    *sea_substr(char const *s, unsigned int start, size_t len);
```

**Memory Functions:**
```c
void    *sea_memcpy(void *dst, const void *src, size_t n);
void    *sea_memset(void *b, int c, size_t len);
void    sea_bzero(void *s, size_t n);
void    *sea_memmove(void *dst, const void *src, size_t len);
int     sea_memcmp(const void *s1, const void *s2, size_t n);
```

**Character Functions:**
```c
int     sea_isalpha(int c);
int     sea_isdigit(int c);
int     sea_isalnum(int c);
int     sea_toupper(int c);
int     sea_tolower(int c);
```

### 2. Memory Allocator (`sea_malloc`, `sea_free`, `sea_realloc`, `sea_calloc`)

```c
// Standard allocation
void *ptr = sea_malloc(1024);
sea_free(ptr);

// Reallocation
ptr = sea_malloc(100);
ptr = sea_realloc(ptr, 200);

// Zero-initialized allocation
int *arr = sea_calloc(10, sizeof(int));

// Memory inspection
show_alloc_mem();           // Show all allocations
show_alloc_mem_ex(ptr);     // Show hex dump of allocation
```

**Memory Zones:**
- **TINY**: ≤128 bytes, 16KB zone size (optimized for small allocations)
- **SMALL**: 129-8192 bytes, 1MB zone size (designed to hold 100 blocks of 1024 bytes, page-aligned at 106496 bytes)
- **LARGE**: >8192 bytes, direct mmap allocation

### 3. Printf (`sea_printf`)

```c
// Standard output
sea_printf("Hello %s, you are %d years old\n", name, age);
```

**Supported Formats:**
- `%c` - Character
- `%s` - String
- `%d`, `%i` - Signed decimal
- `%u` - Unsigned decimal
- `%x`, `%X` - Hexadecimal (lowercase/uppercase)
- `%p` - Pointer address
- `%%` - Literal percent

**Flags:**
- `-` - Left-justify
- `0` - Zero-padding
- `#` - Alternative form
- `+` - Always show sign
- ` ` - Space before positive numbers

### 4. Get Next Line

```c
#include "sea_get_line.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h> // Essential for error checking

int main(void)
{
    int   fd;
    char  *line;

    fd = open("my_file.txt", O_RDONLY);
    if (fd < 0)
        return (1);

    // 1. Reset errno before the loop
    errno = 0;

    // 2. Loop while a line is returned
    while ((line = sea_get_line(fd)) != NULL)
    {
        printf("%s", line); // line includes the \n
        
        // CRITICAL: You must free the line!
        sea_free(line);
        
        // Reset errno for the next iteration safety
        errno = 0;
    }

    // 3. Check why the loop stopped
    if (errno != 0)
    {
        perror("Error reading file");
    }
    else
    {
        printf("End of file reached successfully.\n");
    }

    close(fd);
    return (0);
}
```

### 5. Linked Lists

```c
// Create a list
t_list *head = sea_lstnew("First");
sea_lstadd_back(&head, sea_lstnew("Second"));
sea_lstadd_front(&head, sea_lstnew("Zero"));

// Iterate
sea_lstiter(head, &print_content);

// Map (transform)
t_list *new_list = sea_lstmap(head, &transform, &sea_free);

// Clear
sea_lstclear(&head, &sea_free);
```

---

## 🧪 Testing

### Run All Tests

```bash
# Test core library
make test-sealib

# Test malloc
make test-malloc

# Test printf
make test-printf

# Test get next line
make test-gnl

# Run all tests
make test
```

### Test Output Example

```
🐙 ============================================== 🐙
       KRAKENLIB MALLOC COMPREHENSIVE TESTS
🐙 ============================================== 🐙

🔹 TEST 1: Basic Allocations
  ✅ Basic allocations work!

🔹 TEST 2: Zone Boundaries
  ✅ Zone boundaries work!

🔹 TEST 3: Zero Size and NULL
  ✅ Edge cases handled!

...

🐙 ============================================== 🐙
       🎉 ALL MALLOC TESTS PASSED! 🎉
🐙 ============================================== 🐙
```

---

## ⚡ Performance

### Benchmark Results

Tested on **AMD Ryzen 9 7950X3D**  
Compiler: GCC with -O2 optimization  
Iterations: 1,000,000 per test (scaled down for larger allocations)

#### String Operations

| Operation | Kraken (ns) | libc (ns) | Cycles (Kraken) | Cycles (libc) | Difference |
|-----------|-------------|-----------|-----------------|---------------|------------|
| `sea_strlen` | 1.79 | 2.14 | 8 | 9 | **-16.6%** |
| `sea_strcmp` | 8.08 | 2.25 | 34 | 9 | +259.7% |
| `sea_memcpy_fast(1KB)` | 12.46 | 14.11 | 52 | 58 | **-11.7%** |

#### Memory Allocation

| Operation | Kraken (ns) | libc (ns) | Cycles (Kraken) | Cycles (libc) | Difference |
|-----------|-------------|-----------|-----------------|---------------|------------|
| `sea_malloc(16)` | 19.57 | 6.72 | 82 | 28 | +191.1% |
| `sea_malloc(512)` | 33.00 | 108.87 | 139 | 457 | **-69.7%** |
| `sea_malloc(4KB)` | 168.24 | 717.45 | 707 | 3013 | **-76.6%** |
| `sea_malloc(8KB)` | 272.71 | 1005.12 | 1145 | 4221 | **-72.9%** |
| `sea_malloc(32KB)` | 4925.30 | 1194.72 | 20685 | 5018 | +312.3% |
| `sea_malloc(64KB)` | 5159.29 | 1632.99 | 21667 | 6858 | +215.9% |
| `sea_malloc(512KB)` | 5914.44 | 2311.60 | 24840 | 9708 | +155.9% |
| `sea_malloc(1MB)` | 4906.26 | 2572.84 | 20606 | 10805 | +90.7% |
| `sea_malloc(16MB)` | 6107.10 | 5974.39 | 25647 | 25090 | +2.2% |

#### Formatted Output

| Operation | Kraken (ns) | libc (ns) | Cycles (Kraken) | Cycles (libc) | Difference |
|-----------|-------------|-----------|-----------------|---------------|------------|
| `sea_printf` | 501.12 | 486.79 | 2105 | 2044 | +2.9% |

### Performance Analysis

**String Operations:**
- ✅ **`strlen`**: Faster than glibc by **16.6%** - excellent optimization!
- ✅ **`memcpy_fast`**: Faster than glibc by **11.7%** - optimized for bulk data transfer!

**Memory Allocation:**
- ✅ **Sweet spot: 512B - 8KB** - Krakenlib is **70-77% faster** than glibc!
- Small allocations (16 bytes) show overhead due to glibc's fastbin optimization
- Medium allocations excel with Krakenlib's three-tier design
- Very large allocations (32KB+) favor glibc's arena-based approach
- At 16MB, performance is nearly identical (only 2.2% difference)

**Why Krakenlib Excels at Medium Sizes:**
- Optimized block reuse in TINY/SMALL zones
- Reduced fragmentation with dedicated size classes
- Efficient page management for 512B-8KB range
- No arena/thread-cache overhead for these sizes

**Printf:**
- ✅ Only **2.9% slower** than glibc - virtually identical performance!
- Excellent result for formatted output
- Both implementations use similar buffering strategies

### Key Takeaways

🏆 **Krakenlib outperforms glibc for medium-sized allocations (512B-8KB)** - the most common allocation sizes in many applications!

🏆 **Competitive string performance** with `strlen` beating glibc

🏆 **Printf performance on par with glibc** - only 2.9% difference

---

## 📖 Documentation

### Memory Layout

```
TINY ZONE (≤128 bytes per block)
┌─────────────────────────────────────┐
│ Zone metadata                       │
├─────────────────────────────────────┤
│ Block + data (up to 128 bytes)     │
│ Block + data (up to 128 bytes)     │
│ ...                                │
│ Total zone size: 16KB              │
└─────────────────────────────────────┘

SMALL ZONE (129-8192 bytes per block)
┌─────────────────────────────────────┐
│ Zone metadata                       │
├─────────────────────────────────────┤
│ Block + data (up to 8192 bytes)    │
│ Block + data (up to 8192 bytes)    │
│ ...                                │
│ Total zone size: 1MB               │
│ (Designed for 100 blocks of 1KB)   │
└─────────────────────────────────────┘

LARGE (>8192 bytes, direct mmap)
┌─────────────────────────────────────┐
│ Block metadata + N bytes data      │
│ Allocated directly with mmap       │
└─────────────────────────────────────┘
```

## 📜 License

This project is licensed under the Creative Commons 1.0 Universal License - see the [LICENSE](LICENSE) file for details.

---

## 👥 Authors

- [@espadara](https://github.com/almoraru)

---

<div align="center">

**⚓ Release the Kraken! ⚓**

Made with 🐙 by pirates of code

</div>
