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
- **Dynamic memory management** with custom `malloc`, `free`, `realloc`, `calloc`
- **Three-tier allocation strategy**:
  - TINY (≤64 bytes)
  - SMALL (65-1024 bytes)  
  - LARGE (>1024 bytes)
- **Memory introspection**: `show_alloc_mem()`, `show_alloc_mem_ex()`
- **Thread-safe** with mutex protection
- **Memory efficient** with block reuse and defragmentation

### 🖨️ Custom Printf Implementation
- **Full format specifier support**: `%c`, `%s`, `%d`, `%i`, `%u`, `%x`, `%X`, `%p`, `%%`
- **Flag support**: `-`, `0`, `#`, `+`, space
- **Width and precision** control
- **Custom output**: file descriptors, strings, buffers
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
    int *arr = malloc(10 * sizeof(int));
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
        free(line);
    }
    close(fd);
    
    // Cleanup
    free(str);
    free(arr);
    
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

### 2. Memory Allocator (`malloc`, `free`, `realloc`, `calloc`)

```c
// Standard allocation
void *ptr = malloc(1024);
free(ptr);

// Reallocation
ptr = malloc(100);
ptr = realloc(ptr, 200);

// Zero-initialized allocation
int *arr = calloc(10, sizeof(int));

// Memory inspection
show_alloc_mem();           // Show all allocations
show_alloc_mem_ex(ptr);     // Show hex dump of allocation
```

**Memory Zones:**
- **TINY**: 64 bytes per block, 128 blocks per page
- **SMALL**: 1024 bytes per block, 128 blocks per page
- **LARGE**: Direct mmap allocation

### 3. Printf (`sea_printf`, `sea_dprintf`, `sea_sprintf`)

```c
// Standard output
sea_printf("Hello %s, you are %d years old\n", name, age);

// File descriptor output
sea_dprintf(fd, "Error: %s\n", error_msg);

// String output
char buffer[100];
sea_sprintf(buffer, "Value: %d", 42);
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
        free(line);
        
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
t_list *new_list = sea_lstmap(head, &transform, &free);

// Clear
sea_lstclear(&head, &free);
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

Tested on **AMD Ryzen 9 7950X3D @ 4.2GHz** (3D V-Cache)  
Compiler: GCC with -O2 optimization  
Iterations: 1,000,000 per test

#### String Operations

| Operation | Krakenlib (ns) | glibc (ns) | Cycles (Kraken) | Cycles (glibc) | Difference |
|-----------|----------------|------------|-----------------|----------------|------------|
| `sea_strlen` | 3.08 | 2.22 | 13 | 9 | +38.8% |
| `sea_strcmp` | 13.13 | 2.17 | 55 | 9 | +506.1% |
| `sea_memcpy_fast(1KB)` | 14.40 | 6.92 | 60 | 29 | +108.0% |

#### Memory Allocation

| Operation | Krakenlib (ns) | glibc (ns) | Cycles (Kraken) | Cycles (glibc) | Difference |
|-----------|----------------|------------|-----------------|----------------|------------|
| `malloc(16)` | 674.59 | 7.69 | 2833 | 32 | +8670% |
| `malloc(512)` | 1012.83 | 128.73 | 4254 | 541 | +687% |
| `malloc(4096)` | 4677.41 | 783.99 | 19644 | 3293 | +497% |

#### Formatted Output

| Operation | Krakenlib (ns) | glibc (ns) | Cycles (Kraken) | Cycles (glibc) | Difference |
|-----------|----------------|------------|-----------------|----------------|------------|
| `sea_printf` | 483.80 | 423.69 | 2032 | 1779 | +14.2% |

### Performance Notes

**String Operations:**
- `strlen`: Competitive performance, only 38% slower than highly-optimized glibc
- `strcmp`: Uses simple byte-by-byte comparison; glibc uses SIMD for long strings
- `memcpy`: Good performance at ~2x slower; glibc uses SSE/AVX instructions

**Memory Allocation:**
- glibc's malloc (ptmalloc2) has **decades of optimization** including:
  - Thread-local caches (tcache) - no mutex overhead
  - CPU-optimized arena design
  - Assembly-optimized fast paths
  - Predictive prefetching
- Krakenlib's allocator prioritizes **simplicity and educational value** while maintaining reasonable performance
- Small allocations (16 bytes) show the biggest gap due to glibc's fastbin optimization
- Larger allocations (4KB+) are more competitive at ~6x slower

**Printf:**
- Krakenlib's printf is only **14% slower** than glibc - excellent result!
- Both implementations use similar buffering strategies
- Minimal overhead for formatted output

---

## 📖 Documentation

### Memory Layout

```
TINY ZONE (64 bytes per block)
┌─────────────────────────────────────┐
│ t_page header                       │
├─────────────────────────────────────┤
│ t_block + 64 bytes data            │
│ t_block + 64 bytes data            │
│ ...                                │
└─────────────────────────────────────┘

SMALL ZONE (1024 bytes per block)
┌─────────────────────────────────────┐
│ t_page header                       │
├─────────────────────────────────────┤
│ t_block + 1024 bytes data          │
│ t_block + 1024 bytes data          │
│ ...                                │
└─────────────────────────────────────┘

LARGE (direct mmap)
┌─────────────────────────────────────┐
│ t_block + N bytes data             │
└─────────────────────────────────────┘
```

---

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

---

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
