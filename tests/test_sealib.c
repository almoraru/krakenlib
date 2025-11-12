/* ************************************************************************** */
/*                                                                            */
/*                        ______                                              */
/*                     .-"      "-.                                           */
/*                    /            \                                          */
/*        _          |              |          _                              */
/*       ( \         |,  .-.  .-.  ,|         / )                             */
/*        > "=._     | )(__/  \__)( |     _.=" <                              */
/*       (_/"=._"=._ |/     /\     \| _.="_.="\_)                             */
/*              "=._ (_     ^^     _)"_.="                                    */
/*                  "=\__|IIIIII|__/="                                        */
/*                 _.="| \IIIIII/ |"=._                                       */
/*       _     _.="_.="\          /"=._"=._     _                             */
/*      ( \_.="_.="     `--------`     "=._"=._/ )                            */
/*       > _.="                            "=._ <                             */
/*      (_/                                    \_)                            */
/*                                                                            */
/*      Filename: test.c                                                      */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/08/27 22:40:24 by espadara                              */
/*      Updated: 2025/11/13 00:20:19 by espadara                              */
/*                                                                            */
/* ************************************************************************** */


#include "krakenlib.h"

#define PRINT_TEST(description, condition)                              \
    printf("  Test: %-50s -> %s\n", description, (condition) ? "OK" : "FAIL")

/* HELPER FUNCTIONS */
    // A simple function that converts a character to uppercase
    char map_toupper_func(unsigned int i, char c) {
        (void)i; // Unused parameter
        return (sea_toupper(c));
    }

    // A function that adds the index to the character's ASCII value
    char map_add_index_func(unsigned int i, char c) {
        return (c + i);
    }

    // A function that returns the character unchanged
    char map_identity_func(unsigned int i, char c) {
        (void)i; // Unused parameter
        return (c);
    }
    // --- Helper functions to use for testing ---
    void map_to_upper(unsigned int i, char *c) {
        (void)i;
        if (*c >= 'a' && *c <= 'z') *c -= 32;
    }
    void map_add_index(unsigned int i, char *c) {
        *c = *c + i;
    }
    void map_to_x(unsigned int i, char *c) {
        (void)i;
        *c = 'x';
    }
    // Helper function to add a new node to the front of a list
    void lst_add_front(t_list **lst, t_list *new_node) {
        if (lst && new_node) {
            new_node->next = *lst;
            *lst = new_node;
        }
    }

static void *map_strdup_toupper(void *content)
{
    char *str = (char *)content;
    char *new_str;
    int i = 0;

    if (content == NULL)
        return (NULL);
    new_str = sea_strdup(str);
    if (!new_str)
        return (NULL);
    while (new_str[i])
    {
        new_str[i] = sea_toupper(new_str[i]);
        i++;
    }
    return (new_str);
}

/**
 * @brief Helper to compare two lists.
 * Returns 1 if identical, 0 otherwise. Assumes content is a string.
 */
static int compare_lists(t_list *l1, t_list *l2)
{
    while (l1 && l2)
    {
        if (l1->content == NULL && l2->content == NULL)
        {
            // Both are NULL, continue
        }
        else if (l1->content == NULL || l2->content == NULL)
        {
            return 0; // One is NULL, the other isn't
        }
        else if (strcmp(l1->content, l2->content) != 0)
        {
            return 0; // Content differs
        }
        l1 = l1->next;
        l2 = l2->next;
    }
    return (l1 == NULL && l2 == NULL); // Both lists must end at the same time
}
    // Helper function to free the test list
    void free_test_list(t_list *head) {
        t_list *tmp;
        while (head) {
            tmp = head->next;
            free(head);
            head = tmp;
        }
    }
    // A global flag to check if our test deletion function was called.
static int g_del_called_flag = 0;

    // A custom deletion function for testing purposes.
void test_del_func(void *data)
{
    if (data)
        free(data);
    g_del_called_flag = 1; // Set the flag to prove this function ran.
}
void free_list(t_list *lst)
{
    t_list *head = lst;
    t_list *temp;
    while (head)
    {
        temp = head;
        if (temp->content)
            free(temp->content);
        head = head->next;
        free(temp);
    }
}
static void noop_del(void *data)
{
    (void)data; // This just tells the compiler we know the parameter is unused.
}

static int g_del_counter = 0;
void test_del_counter(void *data) {
    free(data);
    g_del_counter++;
}

// A simple function to check if the function was called
int g_iter_called_count = 0;

void test_count_func(void *content)
{
    (void)content; // Unused parameter
    g_iter_called_count++;
}

void simple_toupper_func(void *content)
{
    char *str = (char *)content;
    if (str) {
        while (*str)
        {
            *str = toupper((unsigned char)*str);
            str++;
        }
    }
}
/* TEST STRUCTS */
struct TestCase {
    const char *string;
    int character;
};

int main(void)
{
  printf("CONDUCTING TESTS!!!\n");

  puts("\n---ISALPHA---");
  {
    char tests[] = {'a', 'G', 'z', 'Z', '5', '@', 0};
    for (int i = 0; tests[i]; i++) {
        int real = isalpha(tests[i]);
        int seal = sea_isalpha(tests[i]);
        printf("Test '%c': REAL=%d, SEAL=%d -> %s\n", tests[i], real, seal,
               (!!real == !!seal) ? "OK" : "FAIL");
    }
  }

  puts("\n---ISDIGIT---");
  {
    char tests[] = {'5', '0', '9', 'a', 'Z', '#', 0};
    for (int i = 0; tests[i]; i++) {
        int real = isdigit(tests[i]);
        int seal = sea_isdigit(tests[i]);
        printf("Test '%c': REAL=%d, SEAL=%d -> %s\n", tests[i], real, seal,
               (!!real == !!seal) ? "OK" : "FAIL");
    }
  }

  puts("\n---ISALNUM---");
  {
    char tests[] = {'a', 'Z', '5', '0', 'g', '9', '@', 0};
    for (int i = 0; tests[i]; i++) {
        int real = isalnum(tests[i]);
        int seal = sea_isalnum(tests[i]);
        printf("Test '%c': REAL=%d, SEAL=%d -> %s\n", tests[i], real, seal,
               (!!real == !!seal) ? "OK" : "FAIL");
    }
  }

  puts("\n---ISASCII---");
  {
    int tests[] = {'a', 127, 0, 128, -1, 255, 0};
    for (int i = 0; tests[i]; i++) {
        int real = isascii(tests[i]);
        int seal = sea_isascii(tests[i]);
        printf("Test %d: REAL=%d, SEAL=%d -> %s\n", tests[i], real, seal,
               (!!real == !!seal) ? "OK" : "FAIL");
    }
  }

  puts("\n---ISPRINT---");
  {
    int tests[] = {'a', '~', ' ', 31, 127, 200, 0};
    for (int i = 0; tests[i]; i++) {
        int real = isprint(tests[i]);
        int seal = sea_isprint(tests[i]);
        printf("Test %d ('%c'): REAL=%d, SEAL=%d -> %s\n", tests[i], tests[i], real, seal,
               (!!real == !!seal) ? "OK" : "FAIL");
    }
  }

  puts("\n---STRLEN---");
  {
    const char *tests[] = {"hello", "", "a", "1234567890", NULL};
    for (int i = 0; tests[i]; i++) {
        size_t real = strlen(tests[i]);
        size_t seal = sea_strlen(tests[i]);
        printf("Test \"%s\": REAL=%zu, SEAL=%zu -> %s\n", tests[i], real, seal,
               (real == seal) ? "OK" : "FAIL");
    }
  }

  puts("\n---MEMSET---");
  {
    struct { int c; size_t len; } tests[] = {
        {'A', 5}, {'B', 10}, {'\0', 3}, {'$', 0}, {'Z', 1}
    };
    for (int i = 0; i < 5; ++i) {
        char real_buf[11] = "xxxxxxxxxx";
        char seal_buf[11] = "xxxxxxxxxx";
        memset(real_buf, tests[i].c, tests[i].len);
        sea_memset(seal_buf, tests[i].c, tests[i].len);
        printf("Test: memset(buf, '%c', %zu) | Result: \"%s\" -> %s\n",
               (char)tests[i].c, tests[i].len, seal_buf,
               (memcmp(real_buf, seal_buf, 11) == 0) ? "OK" : "FAIL");
    }
  }

  puts("\n---BZERO---");
  {
    size_t tests[] = {5, 10, 0, 1, 8};
    for (int i = 0; i < 5; ++i) {
        char real_buf[11] = "xxxxxxxxxx";
        char seal_buf[11] = "xxxxxxxxxx";
        bzero(real_buf, tests[i]);
        sea_bzero(seal_buf, tests[i]);
        printf("Test: bzero(buf, %zu) -> %s\n", tests[i],
               (memcmp(real_buf, seal_buf, 11) == 0) ? "OK" : "FAIL");
    }
  }

  puts("\n---MEMCPY---");
  {
    struct { const char *src; size_t len; } tests[] = {
        {"copy this", 5}, {"abc", 4}, {"", 1}, {"test", 0}, {"single", 1}
    };
    for (int i = 0; i < 5; ++i) {
        char real_dest[20] = {0};
        char seal_dest[20] = {0};
        memcpy(real_dest, tests[i].src, tests[i].len);
        sea_memcpy(seal_dest, tests[i].src, tests[i].len);
        printf("Test: memcpy(dest, \"%s\", %zu) | Result: \"%s\" -> %s\n",
               tests[i].src, tests[i].len, seal_dest,
               (memcmp(real_dest, seal_dest, 20) == 0) ? "OK" : "FAIL");
    }
  }

  puts("\n---MEMMOVE---");
  {
    char r1[] = "abcdefghij", s1[] = "abcdefghij";
    memmove(r1 + 2, r1, 5); sea_memmove(s1 + 2, s1, 5);
    printf("Test: memmove(\"abcdefghij\"+2, src, 5) | Result: \"%s\" -> %s\n", s1, (strcmp(r1, s1) == 0) ? "OK" : "FAIL");

    char r2[] = "abcdefghij", s2[] = "abcdefghij";
    memmove(r2, r2 + 2, 5); sea_memmove(s2, s2 + 2, 5);
    printf("Test: memmove(dst, \"abcdefghij\"+2, 5) | Result: \"%s\" -> %s\n", s2, (strcmp(r2, s2) == 0) ? "OK" : "FAIL");

    char r3[] = "abcdefghij", s3[] = "abcdefghij";
    memmove(r3 + 5, r3, 5); sea_memmove(s3 + 5, s3, 5);
    printf("Test: No Overlap | Result: \"%s\" -> %s\n", s3, (strcmp(r3, s3) == 0) ? "OK" : "FAIL");

    char r4[] = "abcdefghij", s4[] = "abcdefghij";
    memmove(r4, r4, 0); sea_memmove(s4, s4, 0);
    printf("Test: Zero Length | Result: \"%s\" -> %s\n", s4, (strcmp(r4, s4) == 0) ? "OK" : "FAIL");

    char r5[] = "abcdefghij", s5[] = "abcdefghij";
    memmove(r5, r5, 10); sea_memmove(s5, s5, 10);
    printf("Test: Full Move | Result: \"%s\" -> %s\n", s5, (strcmp(r5, s5) == 0) ? "OK" : "FAIL");
  }

  puts("\n---STRLCPY---");
  {
    const char *src = "a long string to copy";
    struct { size_t size; } tests[] = { {10}, {30}, {1}, {0}, {22} };
    for (int i = 0; i < 5; ++i) {
        char real_dest[30] = {0};
        char seal_dest[30] = {0};
        size_t real_ret = strlcpy(real_dest, src, tests[i].size);
        size_t seal_ret = sea_strlcpy(seal_dest, src, tests[i].size);
        printf("Test: strlcpy(dest, \"%s\", %zu) | Result: \"%s\", ret: %zu -> %s\n",
               src, tests[i].size, seal_dest, seal_ret,
               (strcmp(real_dest, seal_dest) == 0 && real_ret == seal_ret) ? "OK" : "FAIL");
    }
  }

  puts("\n---STRLCAT---");
  {
    const char *src = " add this";
    struct { const char* initial_dst; size_t size; } tests[] = {
        {"start", 20}, {"start", 10}, {"start", 6}, {"start", 0}, {"", 20}
    };
    for (int i = 0; i < 5; ++i) {
        char real_dest[25], seal_dest[25];
        strcpy(real_dest, tests[i].initial_dst);
        strcpy(seal_dest, tests[i].initial_dst);
        size_t real_ret = strlcat(real_dest, src, tests[i].size);
        size_t seal_ret = sea_strlcat(seal_dest, src, tests[i].size);
        printf("Test: strlcat(\"%s\", \"%s\", %zu) | Result: \"%s\", ret: %zu -> %s\n",
               tests[i].initial_dst, src, tests[i].size, seal_dest, seal_ret,
               (strcmp(real_dest, seal_dest) == 0 && real_ret == seal_ret) ? "OK" : "FAIL");
    }
  }

  puts("\n---TOUPPER & TOLOWER---");
  {
    char tests[] = {'a', 'Z', 'g', 'H', '5', '#', ' ', '\t', 0};
    printf("Input | REAL u/l | SEAL u/l | Status\n");
    printf("------|----------|----------|--------\n");
    for (int i = 0; tests[i]; i++)
    {
        char c = tests[i];
        int real_upper = toupper(c);
        int seal_upper = sea_toupper(c);
        int real_lower = tolower(c);
        int seal_lower = sea_tolower(c);
        const char *status = (real_upper == seal_upper && real_lower == seal_lower) ? "OK" : "FAIL";
        printf("  '%c' |  '%c'/'%c'  |  '%c'/'%c'  | %s\n",
               c, real_upper, real_lower, seal_upper, seal_lower, status);
    }
  }

  puts("\n---STRCHR & STRRCHR ---");
  {
    struct TestCase tests[] = {
        {"hello world", 'l'},
        {"test", 't'},
        {"bonjour", 'z'},
        {"find the end", '\0'},
        {"abracadabra", 'a'}
    };
    int num_tests = sizeof(tests) / sizeof(tests[0]);
    for (int i = 0; i < num_tests; i++) {
        const char *s = tests[i].string;
        int c = tests[i].character;
        char *real_chr = strchr(s, c);
        char *seal_chr = sea_strchr(s, c);
        char *real_rchr = strrchr(s, c);
        char *seal_rchr = sea_strrchr(s, c);

        printf("strchr(\"%s\", '%c'): -> %s | ", s, c, (real_chr == seal_chr) ? "OK" : "FAIL");
        printf("strrchr(\"%s\", '%c'): -> %s\n", s, c, (real_rchr == seal_rchr) ? "OK" : "FAIL");
    }
  }
  puts("\n---STRSTR---");
  {
    // A structure to hold test cases for clarity
    struct {
        const char *haystack;
        const char *needle;
    } tests[] = {
        {"Hello world, this is a test.", "world"}, // Standard case
        {"Hello world", "earth"},                  // No match
        {"Another test", ""},                      // Empty needle
        {"Start of string", "Start"},              // Match at the beginning
        {"Match at the end", "end"}                // Match at the end
    };

    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++)
    {
        char *real_result = strstr(tests[i].haystack, tests[i].needle);
        char *seal_result = sea_strstr(tests[i].haystack, tests[i].needle);

        printf("Test: strstr(\"%s\", \"%s\") | Result: \"%s\" -> %s\n",
               tests[i].haystack,
               tests[i].needle,
               seal_result ? seal_result : "(null)",
               (real_result == seal_result) ? "OK" : "FAIL");
    }
  }
puts("\n---STRCMP---");
  {
    // A structure to hold strcmp test cases
    struct {
        const char *s1;
        const char *s2;
    } tests[] = {
        {"Hello", "Hello"},      // Equal strings
        {"Hella", "Hello"},      // s1 < s2
        {"Testing", "Test"},     // s1 > s2 (is a prefix of s1)
        {"", ""},                // Both empty
        {"abc", "abd"}           // Difference at the end
    };
    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++) {
        int real_ret = strcmp(tests[i].s1, tests[i].s2);
        int seal_ret = sea_strcmp(tests[i].s1, tests[i].s2);

        // Normalize results to -1, 0, or 1 to safely compare
        int real_sign = (real_ret > 0) - (real_ret < 0);
        int seal_sign = (seal_ret > 0) - (seal_ret < 0);

        printf("Test: strcmp(\"%s\", \"%s\") -> %s\n",
               tests[i].s1, tests[i].s2,
               (real_sign == seal_sign) ? "OK" : "FAIL");
    }
  }

  puts("\n---STRNCMP---");
  {
    // A structure to hold strncmp test cases
    struct {
        const char *s1;
        const char *s2;
        size_t n;
    } tests[] = {
        {"Testabc", "Testing", 4}, // Equal for n bytes
        {"Testabc", "Testing", 7}, // Different before n bytes
        {"abc", "def", 0},         // n is zero
        {"abc", "abcdef", 5},      // s1 ends before n
        {"abc", "abc", 10}         // n is larger than string length
    };
    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++) {
        int real_ret = strncmp(tests[i].s1, tests[i].s2, tests[i].n);
        int seal_ret = sea_strncmp(tests[i].s1, tests[i].s2, tests[i].n);

        // Normalize results to -1, 0, or 1 to safely compare
        int real_sign = (real_ret > 0) - (real_ret < 0);
        int seal_sign = (seal_ret > 0) - (seal_ret < 0);

        printf("Test: strncmp(\"%s\", \"%s\", %zu) -> %s\n",
               tests[i].s1, tests[i].s2, tests[i].n,
               (real_sign == seal_sign) ? "OK" : "FAIL");
    }
  }
  puts("\n---STRNSTR---");
  {
    // A structure to hold strnstr test cases
    struct {
        const char *haystack;
        const char *needle;
        size_t n;
    } tests[] = {
        {"lorem ipsum dolor sit amet", "ipsum", 20}, // Standard match
        {"lorem ipsum dolor sit amet", "dolor", 10}, // Match exists, but its start is after n
        {"find the needle here", "needle", 20},      // Another standard match
        {"a test with an empty needle", "", 5},      // Empty needle
        {"short", "a longer needle", 5}              // Needle is longer than haystack
    };

    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++)
    {
        char *real_result = strnstr(tests[i].haystack, tests[i].needle, tests[i].n);
        char *seal_result = sea_strnstr(tests[i].haystack, tests[i].needle, tests[i].n);

        printf("Test: strnstr(\"%s\", \"%s\", %zu) | Result: \"%s\" -> %s\n",
               tests[i].haystack,
               tests[i].needle,
               tests[i].n,
               seal_result ? seal_result : "(null)",
               (real_result == seal_result) ? "OK" : "FAIL");
    }
  }
  puts("\n---MEMCHR---");
  {
    // A special memory block containing a null byte to test against.
    char mem_with_null[] = {'a', 'b', '\0', 'd', 'e'};

    // A structure to hold the test cases
    struct {
        const void *mem_block;
        int char_to_find;
        size_t size;
        const char *description;
    } tests[] = {
        {"abcdef", 'c', 6, "Standard Match"},
        {"abcdef", 'x', 6, "No Match"},
        {mem_with_null, 'd', 5, "Match After Null"},
        {mem_with_null, '\0', 5, "Find Null Byte"},
        {"abcdef", 'f', 4, "Match Outside n"}
    };

    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++)
    {
        void *real_result = memchr(tests[i].mem_block, tests[i].char_to_find, tests[i].size);
        void *seal_result = sea_memchr((void *)tests[i].mem_block, tests[i].char_to_find, tests[i].size);

        printf("Test: %-18s (char: '%c', n: %zu) -> %s\n",
               tests[i].description,
               (char)tests[i].char_to_find,
               tests[i].size,
               (real_result == seal_result) ? "OK" : "FAIL");
    }
  }
  puts("\n---MEMCMP---");
  {
    struct {
      const char *s1;
      const char *s2;
      size_t size;
      const char *description;
      int expected_sign; // 1 for positive, -1 for negative, 0 for zero
    } tests[] = {
      {"abcdef", "abcdef", 6, "Identical Blocks", 0},
      {"abcdeg", "abcdef", 6, "First Mismatch Positive", 1},
      {"abcdef", "abcdeg", 6, "First Mismatch Negative", -1},
      {"abcdef", "abc", 3, "Identical Up to n", 0},
      {"abcdef", "abcdeg", 3, "Different After n", 0},
      {"\0ghi", "\0jkl", 4, "Blocks With Null", -1},
      {"", "", 0, "Empty Blocks", 0},
      {"abc", "abx", 3, "Mismatch at End", -1},
    };

    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++)
      {
        int real_result = memcmp(tests[i].s1, tests[i].s2, tests[i].size);
        int sea_result = sea_memcmp(tests[i].s1, tests[i].s2, tests[i].size);
        int real_sign = (real_result > 0) ? 1 : (real_result < 0 ? -1 : 0);
        int sea_sign = (sea_result > 0) ? 1 : (sea_result < 0 ? -1 : 0);
        int test_passed = (real_sign == sea_sign);
        if (test_passed && tests[i].expected_sign == 0) {
          test_passed = (real_result == sea_result);
        }

        printf("Test: %-25s | s1: \"%.*s\" | s2: \"%.*s\" | n: %zu -> %s\n",
               tests[i].description,
               (int)tests[i].size,
               tests[i].s1,
               (int)tests[i].size,
               tests[i].s2,
               tests[i].size,
               (test_passed) ? "OK" : "FAIL");
      }
  }

  puts("\n---ISWHITESPACE---");
  {
    // Array of characters to test
    char tests[] = {
        ' ',      // Space
        '\t',     // Tab
        '\n',     // Newline
        '\v',     // Vertical Tab
        '\f',     // Form Feed
        '\r',     // Carriage Return
        'a',      // Lowercase letter
        'Z',      // Uppercase letter
        '7',      // Digit
        '@',      // Symbol
        0         // Null terminator for the loop
    };

    for (int i = 0; tests[i] != 0; i++)
    {
        char c = tests[i];
        int real_result = isspace(c);
        int seal_result = sea_iswhitespace(c);

        printf("Test char (ASCII: %d): REAL=%d, SEAL=%d -> %s\n",
               c,
               real_result,
               seal_result,
               (!!real_result == !!seal_result) ? "OK" : "FAIL");
    }
  }
  puts("\n---ATOI---");
  {
    // A structure to hold atoi test cases
    struct {
        const char *str;
    } tests[] = {
        {"123"},
        {"-456"},
        {"+789"},
        {"  -42"},
        {"99balloons"},
        {"word"},
        {"+-5"},
        {"--2"}
    };

    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++)
    {
        int real_result = atoi(tests[i].str);
        int seal_result = sea_atoi(tests[i].str);

        printf("Test: atoi(\"%s\") | REAL: %d, SEAL: %d -> %s\n",
               tests[i].str,
               real_result,
               seal_result,
               (real_result == seal_result) ? "OK" : "FAIL");
    }
  }
  puts("\n---ATOI_BASE---");
  {
    struct {
        const char *str;
        int base;
    } tests[] = {
        {"123", 10},                      // Standard decimal
        {"  -ff", 16},                    // Hexadecimal with whitespace and sign
        {"10110", 2},                     // Binary
        {"1274", 8},                      // Octal with an invalid digit '7'
        {" \n\t+aB", 12}                  // Base 12 with mixed case and whitespace
    };

    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++)
    {
        // The real function for this is strtol(string, NULL, base)
        int real_result = (int)strtol(tests[i].str, NULL, tests[i].base);
        int seal_result = sea_atoi_base(tests[i].str, tests[i].base);

        printf("Test: atoi_base(\"%s\", %d) | REAL: %d, SEAL: %d -> %s\n",
               tests[i].str,
               tests[i].base,
               real_result,
               seal_result,
               (real_result == seal_result) ? "OK" : "FAIL");
    }

  }
  puts("\n---ATOF---");
  {

    #define EPSILON 1e-9

    struct {
        const char *str;
    } tests[] = {
        {"123.456"},
        {"  -0.0123"},
        {"+500"},
        {".789"},
        {"42.9abc"}
    };

    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++)
    {
        double real_result = atof(tests[i].str);
        double seal_result = sea_atof(tests[i].str);

        // CORRECT WAY TO COMPARE: Check if the absolute difference is negligible
        printf("Test: atof(\"%s\") | REAL: %f, SEAL: %f -> %s\n",
               tests[i].str,
               real_result,
               seal_result,
               (fabs(real_result - seal_result) < EPSILON) ? "OK" : "FAIL");
    }
  }
  puts("\n---ARENA ALLOCATOR---");
  {
    t_mem *arena = NULL;
    void *p1, *p2;

    // --- Initialization Tests ---
    printf("\n## Initialization\n");
    arena = sea_arena_init(0);
    PRINT_TEST("Initialize with size 0 returns non-NULL", arena != NULL);
    if (arena) {
        PRINT_TEST("Initialize with size 0 uses default size", arena->total == ARENA_DEFAULT);
    }
    sea_arena_free(arena);

    arena = sea_arena_init(128);
    PRINT_TEST("Initialize with custom size (128) returns non-NULL", arena != NULL);
    if (arena) {
        PRINT_TEST("Initialize with custom size (128) is correct", arena->total == 128);
    }
    sea_arena_free(arena);

    // --- Basic Allocation & Alignment Tests ---
    printf("\n## Basic Allocation & Alignment\n");
    arena = sea_arena_init(100); // Using a small size for easy testing
    p1 = sea_arena_alloc(arena, 1);
    PRINT_TEST("First allocation (1 byte) is not NULL", p1 != NULL);
    PRINT_TEST("Used size reflects alignment (1 byte -> 16)", arena ? arena->used == 16 : 0);

    p2 = sea_arena_alloc(arena, 10);
    PRINT_TEST("Second allocation (10 bytes) is not NULL", p2 != NULL);
    PRINT_TEST("Second allocation is at correct offset", p2 == (void*)(arena->mem + 16));
    PRINT_TEST("Used size is now 32 (16 + 16)", arena ? arena->used == 32 : 0);

    // --- Data Integrity Test ---
    strcpy((char*)p2, "testing");
    PRINT_TEST("Data integrity check", strcmp((char*)p2, "testing") == 0);
    sea_arena_free(arena);

    // --- Edge Case Allocation Tests ---
    printf("\n## Edge Case Allocations\n");
    arena = sea_arena_init(100);
    PRINT_TEST("Allocate 0 bytes returns NULL", sea_arena_alloc(arena, 0) == NULL);
    PRINT_TEST("Allocate from a NULL arena returns NULL", sea_arena_alloc(NULL, 10) == NULL);

    p1 = sea_arena_alloc(arena, 100 - ARENA_ALIGN); // Allocate almost everything
    p2 = sea_arena_alloc(arena, ARENA_ALIGN);       // Allocate the rest
    PRINT_TEST("Allocate exact remaining space works", p2 != NULL);
    sea_arena_free(arena);

    // --- Chaining Logic Tests ---
    printf("\n## Chaining Logic\n");
    arena = sea_arena_init(64); // Tiny arena to force chaining
    sea_arena_alloc(arena, 32);
    sea_arena_alloc(arena, 32); // Block 1 is now full (32+32=64)
    PRINT_TEST("First block is full", arena ? arena->used == 64 : 0);
    PRINT_TEST("No second block exists yet", arena ? arena->next == NULL : 0);

    p1 = sea_arena_alloc(arena, 1); // This should trigger chaining
    PRINT_TEST("Allocation triggering a new block is not NULL", p1 != NULL);
    PRINT_TEST("A second block was created", arena ? arena->next != NULL : 0);
    if (arena && arena->next) {
        PRINT_TEST("New block has correct used size (1 byte -> 16)", arena->next->used == 16);
    }

    // --- Large Allocation Test ---
    p2 = sea_arena_alloc(arena, ARENA_DEFAULT + 100); // Request larger than default
    PRINT_TEST("Large allocation ( > default) succeeds", p2 != NULL);
    if(arena && arena->next) {
        PRINT_TEST("Large allocation created a correctly sized block", arena->next->next->total >= ARENA_DEFAULT + 100);
    }
    sea_arena_free(arena);

    // --- Freeing Logic Tests ---
    printf("\n## Freeing Logic\n");
    arena = sea_arena_init(100);
    sea_arena_alloc(arena, 1000); // Create a chain of 2 blocks
    sea_arena_alloc(arena, 5000); // Create a chain of 3 blocks
    sea_arena_free(arena);
    PRINT_TEST("Freeing a chain of blocks (check with Valgrind)", 1); // This test passes if it doesn't crash

    sea_arena_free(NULL);
    PRINT_TEST("Freeing a NULL arena does not crash", 1);
  }
  void *sea_memcpy_fast(void *dest, const void *src, size_t n);

  puts("\n---MEMCPY_FAST---");
  {
    struct {
        size_t n;
        const char *description;
    } tests[] = {
        {0, "Zero-length copy"},
        {5, "Small copy (non-SIMD path)"},
        {15, "Boundary copy (non-SIMD path)"},
        {16, "Boundary copy (1 SIMD block)"},
        {17, "1 SIMD block + 1 byte tail"},
        {32, "2 full SIMD blocks"},
        {40, "2 SIMD blocks + 8 byte tail"},
        {100, "Large unaligned copy"},
        {8, "Copy data with null bytes"},
        {25, "Full buffer copy"}
    };

    // Create a large, safe source buffer for all tests
    char source_data[200];
    memset(source_data, 'A', sizeof(source_data)); // Fill with 'A's
    strcpy(source_data + 20, "null\0test"); // Add some unique data

    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++)
    {
        char real_dest[200] = {0};
        char seal_dest[200] = {0};

        memcpy(real_dest, source_data, tests[i].n);
        sea_memcpy_fast(seal_dest, source_data, tests[i].n);

        printf("Test: %-32s (n=%zu) -> %s\n",
               tests[i].description,
               tests[i].n,
               (memcmp(real_dest, seal_dest, sizeof(real_dest)) == 0) ? "OK" : "FAIL");
    }
  }
  puts("\n---STRDUP (HEAP)---");
  {
    // A structure to hold the test cases
    struct {
        const char *source;
        const char *description;
    } tests[] = {
        {"A normal string", "Standard string"},
        {"", "Empty string"},
        {"a", "Single character string"},
        {"A very long string designed to trigger the fast SIMD copy path.", "Long string (SIMD)"},
        {"\t\n a string with whitespace \t\n", "String with whitespace"},
        {NULL, "NULL pointer input"}
    };
    int num_tests = 6;

    for (int i = 0; i < num_tests; i++)
    {
        // Test strdup itself
        char *seal_result = sea_strdup(tests[i].source);
        if (tests[i].source == NULL) {
            printf("Test: %-30s -> %s\n", "strdup(NULL)", (seal_result == NULL) ? "OK" : "FAIL");
        } else {
            int is_ok = (seal_result != NULL && strcmp(seal_result, tests[i].source) == 0);
            printf("Test: %-30s -> %s\n", tests[i].description, is_ok ? "OK" : "FAIL");
        }
        free(seal_result); // Must free the heap-allocated memory
    }

    // Additional specific tests
    char original[] = "original";
    char *dup = sea_strdup(original);
    strcpy(original, "changed");
    printf("Test: %-30s -> %s\n", "Memory independence", (dup && strcmp(dup, "original") == 0) ? "OK" : "FAIL");
    free(dup);

    dup = sea_strdup("test");
    printf("Test: %-30s -> %s\n", "Pointer is a new address", (dup != (void*)"test") ? "OK" : "FAIL");
    free(dup);

    dup = sea_strdup("len_check");
    printf("Test: %-30s -> %s\n", "Correct length + null terminator", (strlen(dup) == 9) ? "OK" : "FAIL");
    free(dup);

    dup = sea_strdup("another");
    printf("Test: %-30s -> %s\n", "Final sanity check", (strcmp(dup, "another") == 0) ? "OK" : "FAIL");
    free(dup);
  }

  puts("\n---ARENA_STRDUP (ARENA)---");
  {
    t_mem *arena = NULL;
    char *s1, *s2;

    arena = sea_arena_init(64);
    printf("Test: %-45s -> %s\n", "Arena initialization", (arena != NULL) ? "OK" : "FAIL");

    // Test 1 & 2: Standard duplication and 'used' pointer check
    s1 = sea_arena_strdup(arena, "first");
    printf("Test: %-45s -> %s\n", "Standard arena duplication", (strcmp(s1, "first") == 0) ? "OK" : "FAIL");
    size_t expected_used = (strlen("first") + 1 + ARENA_ALIGN - 1) & ~(ARENA_ALIGN - 1);
    printf("Test: %-45s -> %s\n", "Arena 'used' pointer advancement", (arena->used == expected_used) ? "OK" : "FAIL");

    // Test 4 & 5: Handle NULL inputs gracefully
    printf("Test: %-45s -> %s\n", "arena_strdup(arena, NULL)", (sea_arena_strdup(arena, NULL) == NULL) ? "OK" : "FAIL");
    printf("Test: %-45s -> %s\n", "arena_strdup(NULL, src)", (sea_arena_strdup(NULL, "test") == NULL) ? "OK" : "FAIL");

    // -- The New, More Robust Chaining Test --
    // We know 16 bytes are used. The total is 64. Remaining is 48.
    // Let's allocate exactly 48 bytes to fill the block. (3 * 16 = 48)
    sea_arena_alloc(arena, 1); // Uses 16 bytes. Total used: 32
    sea_arena_alloc(arena, 1); // Uses 16 bytes. Total used: 48
    sea_arena_alloc(arena, 1); // Uses 16 bytes. Total used: 64. BLOCK IS NOW FULL.

    printf("Test: %-45s -> %s\n", "First block is now full", (arena->used == 64) ? "OK" : "FAIL");
    printf("Test: %-45s -> %s\n", "No second block exists yet", (arena->next == NULL) ? "OK" : "FAIL");

    // This next allocation is GUARANTEED to fail in the first block.
    s2 = sea_arena_strdup(arena, "chain");

    printf("Test: %-45s -> %s\n", "Allocation forces a new block", (arena->next != NULL) ? "OK" : "FAIL");

    int is_in_new_block = 0;
    if (arena->next) { // Check if arena->next is valid before dereferencing
        is_in_new_block = (s2 >= (char*)arena->next->mem && s2 < (char*)arena->next->mem + arena->next->total);
    }
    printf("Test: %-45s -> %s\n", "Pointer is in the new arena block", is_in_new_block ? "OK" : "FAIL");

    // Final check that old data is still valid
    printf("Test: %-45s -> %s\n", "Data integrity of first string", (strcmp(s1, "first") == 0) ? "OK" : "FAIL");

    sea_arena_free(arena);
  }
  puts("\n---STRSUB (HEAP)---");
  {
    // A structure to hold strsub test cases
    struct {
        const char *s;
        unsigned int start;
        size_t len;
        const char *expected;
    } tests[] = {
        {"Hello World", 6, 5, "World"},          // Standard substring
        {"Hello World", 6, 10, "World"},         // Length is too long (clamps to end)
        {"test", 10, 5, ""},                     // Start is out of bounds
        {"abcdef", 0, 3, "abc"},                 // Extract from the beginning
        {"abcdef", 2, 1, "c"}                    // Extract a single character
    };
    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++)
    {
        char *seal_result = sea_strsub(tests[i].s, tests[i].start, tests[i].len);

        int is_ok = (seal_result != NULL && strcmp(seal_result, tests[i].expected) == 0);

        printf("Test: strsub(\"%s\", %u, %zu) -> %s\n",
               tests[i].s, tests[i].start, tests[i].len,
               is_ok ? "OK" : "FAIL");

        free(seal_result); // Important: free the heap-allocated memory
    }
  }
  puts("\n---ARENA_STRSUB---");
  {
    t_mem *arena = sea_arena_init(64); // Small arena to test chaining
    char *s1, *s2, *s3;
    size_t used_before, used_after;

    printf("Test: %-45s -> %s\n", "Arena initialization", arena != NULL ? "OK" : "FAIL");

    // Test 1: Standard substring & arena usage
    used_before = arena->used;
    s1 = sea_arena_strsub(arena, "Hello World", 6, 5); // "World"
    used_after = arena->used;
    printf("Test: %-45s -> %s\n", "Standard arena substring", (strcmp(s1, "World") == 0) ? "OK" : "FAIL");
    size_t expected_size1 = (strlen("World") + 1 + ARENA_ALIGN - 1) & ~(ARENA_ALIGN - 1);
    printf("Test: %-45s -> %s\n", "Arena 'used' pointer correct", (used_after == used_before + expected_size1) ? "OK" : "FAIL");

    // Test 3: Length is too long (should be clamped)
    s2 = sea_arena_strsub(arena, "short", 0, 10);
    printf("Test: %-45s -> %s\n", "Substring with clamped length", (strcmp(s2, "short") == 0) ? "OK" : "FAIL");

    // Test 4: Start is out of bounds (should be empty)
    s3 = sea_arena_strsub(arena, "test", 10, 5);
    printf("Test: %-45s -> %s\n", "Substring with start out of bounds", (strcmp(s3, "") == 0) ? "OK" : "FAIL");

    // Test 5: Force chaining
    sea_arena_alloc(arena, 64); // Fill the rest of the block
    sea_arena_strsub(arena, "force chain", 0, 5); // This must go in a new block
    printf("Test: %-45s -> %s\n", "Substring forces a new block", (arena->next != NULL) ? "OK" : "FAIL");

    sea_arena_free(arena);
  }
  puts("\n---STRJOIN (HEAP)---");
  {
    // A structure to hold strjoin test cases
    struct {
        const char *s1;
        const char *s2;
        const char *expected;
    } tests[] = {
        {"Hello, ", "World!", "Hello, World!"},
        {"First", "", "First"},
        {"", "Second", "Second"},
        {"", "", ""},
        {"A very long string...", "...and another long string.", "A very long string......and another long string."}
    };
    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++)
    {
        char *seal_result = sea_strjoin(tests[i].s1, tests[i].s2);

        int is_ok = (seal_result != NULL && strcmp(seal_result, tests[i].expected) == 0);

        printf("Test: strjoin(\"%s\", \"%s\") -> %s\n",
               tests[i].s1, tests[i].s2,
               is_ok ? "OK" : "FAIL");

        free(seal_result); // Important: free the heap-allocated memory
    }

    // Explicit NULL test
    char *null_result = sea_strjoin("abc", NULL);
    printf("Test: strjoin(\"abc\", NULL) -> %s\n", (null_result == NULL) ? "OK" : "FAIL");
  }
  puts("\n---ARENA_STRJOIN---");
  {
    t_mem *arena = sea_arena_init(64); // Small arena to test chaining
    char *s1, *s2;
    size_t used_before, used_after;

    printf("Test: %-45s -> %s\n", "Arena initialization", arena != NULL ? "OK" : "FAIL");

    // Test 1: Standard join & arena usage
    used_before = arena->used;
    s1 = sea_arena_strjoin(arena, "Hello, ", "World!");
    used_after = arena->used;
    printf("Test: %-45s -> %s\n", "Standard arena join", (strcmp(s1, "Hello, World!") == 0) ? "OK" : "FAIL");
    size_t expected_size1 = (strlen("Hello, World!") + 1 + ARENA_ALIGN - 1) & ~(ARENA_ALIGN - 1);
    printf("Test: %-45s -> %s\n", "Arena 'used' pointer correct", (used_after == used_before + expected_size1) ? "OK" : "FAIL");

    // Test 3: Joining with an empty string
    s2 = sea_arena_strjoin(arena, "Test", "");
    printf("Test: %-45s -> %s\n", "Joining with an empty string", (strcmp(s2, "Test") == 0) ? "OK" : "FAIL");

    // Test 4: Handle NULL inputs gracefully
    printf("Test: %-45s -> %s\n", "arena_strjoin(arena, NULL, s2)", (sea_arena_strjoin(arena, NULL, "s2") == NULL) ? "OK" : "FAIL");

    // Test 5: Force chaining
    sea_arena_strjoin(arena, "fill", "fill"); // Use up space
    sea_arena_strjoin(arena, "fill", "fill"); // Use up more space
    sea_arena_strjoin(arena, "force", "chain"); // This should force a new block
    printf("Test: %-45s -> %s\n", "Join forces a new block", (arena->next != NULL) ? "OK" : "FAIL");

    sea_arena_free(arena);
  }
  puts("\n---STRTRIM---");
  {
    // A structure to hold strtrim test cases
    struct {
        const char *s1;
        const char *set;
        const char *expected;
    } tests[] = {
        {"  hello world  ", " ", "hello world"},
        {"..!!hello world!!..", ".!", "hello world"},
        {"hello world", "xy", "hello world"},
        {"a hello world a", " ", "a hello world a"},
        {"ababab", "ab", ""},
        {"   hello world", " ", "hello world"},
        {"hello world   ", " ", "hello world"},
        {"", "ab", ""},
        {"  hello  ", "", "  hello  "}
    };
    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++)
    {
        char *seal_result = sea_strtrim(tests[i].s1, tests[i].set);

        int is_ok = (seal_result != NULL && strcmp(seal_result, tests[i].expected) == 0);

        printf("Test: strtrim(\"%s\", \"%s\") -> %s\n",
               tests[i].s1, tests[i].set,
               is_ok ? "OK" : "FAIL");

        free(seal_result); // Important: free the heap-allocated memory
    }

    // Explicit NULL test
    char *null_result = sea_strtrim(NULL, "abc");
    printf("Test: strtrim(NULL, \"abc\") -> %s\n", (null_result == NULL) ? "OK" : "FAIL");
  }
  puts("\n---SPLIT (HEAP)---");
  {
    // Helper function to compare two string arrays
    int are_splits_equal(char **result, const char **expected) {
        int i = 0;
        while (expected[i]) {
            if (result[i] == NULL || strcmp(result[i], expected[i]) != 0)
                return 0; // Mismatch or result array is shorter
            i++;
        }
        if (result[i] != NULL)
            return 0; // Result array is longer
        return 1; // Match
    }

    // Helper function to free the memory from a split
    void free_split(char **split_array) {
        if (!split_array) return;
        for (int i = 0; split_array[i]; i++) {
            free(split_array[i]);
        }
        free(split_array);
    }

    // A structure to hold split test cases
    struct {
        const char *s;
        char c;
        const char *expected[10]; // Max 9 words + NULL
    } tests[] = {
        {"hello world from C", ' ', {"hello", "world", "from", "C", NULL}},
        {"  split   this  ", ' ', {"split", "this", NULL}},
        {"oneword", ',', {"oneword", NULL}},
        {"", ' ', {NULL}},
        {",,,", ',', {NULL}},
    };
    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++)
    {
        char **seal_result = sea_split(tests[i].s, tests[i].c);

        int is_ok = (seal_result != NULL && are_splits_equal(seal_result, tests[i].expected));

        printf("Test: split(\"%s\", '%c') -> %s\n",
               tests[i].s, tests[i].c, is_ok ? "OK" : "FAIL");

        free_split(seal_result); // Important: free all allocated memory
    }
  }
  puts("\n---ARENA_SPLIT---");
  {
    // Helper function to compare two string arrays
    int are_splits_equal(char **result, const char **expected) {
        int i = 0;
        while (expected[i]) {
            if (result[i] == NULL || strcmp(result[i], expected[i]) != 0)
                return 0;
            i++;
        }
        if (result[i] != NULL)
            return 0;
        return 1;
    }

    t_mem *arena = sea_arena_init(100); // Small arena for testing
    printf("Test: %-45s -> %s\n", "Arena initialization", arena != NULL ? "OK" : "FAIL");

    // Test 1: Standard split
    const char *s1 = "A simple test";
    const char *expected1[] = {"A", "simple", "test", NULL};
    char **res1 = sea_arena_split(arena, s1, ' ');
    printf("Test: %-45s -> %s\n", "Standard arena split", are_splits_equal(res1, expected1) ? "OK" : "FAIL");

    // Test 2: Split with multiple delimiters
    const char *s2 = "leading,,,and,,trailing";
    const char *expected2[] = {"leading", "and", "trailing", NULL};
    char **res2 = sea_arena_split(arena, s2, ',');
    printf("Test: %-45s -> %s\n", "Multiple delimiters", are_splits_equal(res2, expected2) ? "OK" : "FAIL");

    // Test 3: Empty string
    const char *s3 = "";
    const char *expected3[] = {NULL};
    char **res3 = sea_arena_split(arena, s3, ' ');
    printf("Test: %-45s -> %s\n", "Empty string input", are_splits_equal(res3, expected3) ? "OK" : "FAIL");

    // Test 4: No delimiters
    const char *s4 = "onelongword";
    const char *expected4[] = {"onelongword", NULL};
    char **res4 = sea_arena_split(arena, s4, ' ');
    printf("Test: %-45s -> %s\n", "No delimiters", are_splits_equal(res4, expected4) ? "OK" : "FAIL");

    // Test 5: Force chaining
    // The first block is 100 bytes. The tests above used some space.
    // This split will require more space than is left, forcing a new block.
    const char *s5 = "this final split is a bit longer and should force a new block";
    sea_arena_split(arena, s5, ' ');
    printf("Test: %-45s -> %s\n", "Split forces a new block", (arena->next != NULL) ? "OK" : "FAIL");

    sea_arena_free(arena);
  }
  puts("\n---ITOA---");
  {
    // A structure to hold itoa test cases
    struct {
        int n;
    } tests[] = {
        {12345},
        {-6789},
        {0},
        {2147483647},   // INT_MAX
        {-2147483648}   // INT_MIN
    };
    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++)
    {
        // Use sprintf to generate the official, correct string
        char expected[12]; // Buffer large enough for any 32-bit int
        sprintf(expected, "%d", tests[i].n);

        char *seal_result = sea_itoa(tests[i].n);

        int is_ok = (seal_result != NULL && strcmp(seal_result, expected) == 0);

        printf("Test: itoa(%d) | Expected: \"%s\", SEAL: \"%s\" -> %s\n",
               tests[i].n, expected, seal_result,
               is_ok ? "OK" : "FAIL");

        free(seal_result); // Important: free the heap-allocated memory
    }
  }
  puts("\n---STRMAPI---");
  {


    // A structure to hold the main test cases
    struct {
        const char *s;
        char (*f)(unsigned int, char);
        const char *expected;
        const char *description;
    } tests[] = {
        {"hello", map_toupper_func, "HELLO", "Lowercase to Uppercase"},
        {"WoRlD", map_toupper_func, "WORLD", "Mixedcase to Uppercase"},
        {"123!@#", map_toupper_func, "123!@#", "Non-alpha with toupper"},
        {"", map_toupper_func, "", "Empty string"},
        {"abc", map_add_index_func, "ace", "Add index to char"},
        {"", map_add_index_func, "", "Empty string with add_index"},
        {"NoChange", map_identity_func, "NoChange", "Identity function (no change)"},
    };
    int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++)
    {
        char *seal_result = sea_strmapi(tests[i].s, tests[i].f);

        int is_ok = (seal_result != NULL && strcmp(seal_result, tests[i].expected) == 0);

        printf("Test: %-28s -> %s\n",
               tests[i].description,
               is_ok ? "OK" : "FAIL");

        free(seal_result); // Important: free the heap-allocated memory
    }

    // --- Explicit NULL edge case tests ---

    // Test 8: NULL string input
    char *null_s_result = sea_strmapi(NULL, map_toupper_func);
    printf("Test: %-28s -> %s\n", "NULL string input", (null_s_result == NULL) ? "OK" : "FAIL");

    // Test 9: NULL function input
    char *null_f_result = sea_strmapi("hello", NULL);
    printf("Test: %-28s -> %s\n", "NULL function input", (null_f_result == NULL) ? "OK" : "FAIL");

    // Test 10: Both inputs are NULL
    char *null_both_result = sea_strmapi(NULL, NULL);
    printf("Test: %-28s -> %s\n", "Both inputs are NULL", (null_both_result == NULL) ? "OK" : "FAIL");
  }
  puts("\n---STRITERI---");
  {
    // --- Test Cases ---

    // Test 1: To Uppercase
    char s1[] = "Hello World";
    char expected1[] = "HELLO WORLD";
    sea_striteri(s1, map_to_upper);
    printf("Test: To Uppercase -> %s\n", (strcmp(s1, expected1) == 0) ? "OK" : "FAIL");

    // Test 2: Add Index
    char s2[] = "abcde";
    char expected2[] = "acegi"; // 'a'+0, 'b'+1, 'c'+2, 'd'+3, 'e'+4
    sea_striteri(s2, map_add_index);
    printf("Test: Add Index -> %s\n", (strcmp(s2, expected2) == 0) ? "OK" : "FAIL");

    // Test 3: Change all chars to 'x'
    char s3[] = "12345";
    char expected3[] = "xxxxx";
    sea_striteri(s3, map_to_x);
    printf("Test: Change to 'x' -> %s\n", (strcmp(s3, expected3) == 0) ? "OK" : "FAIL");

    // Test 4: Empty string (should not crash)
    char s4[] = "";
    char expected4[] = "";
    sea_striteri(s4, map_to_upper);
    printf("Test: Empty string -> %s\n", (strcmp(s4, expected4) == 0) ? "OK" : "FAIL");

    // Test 5: NULL inputs (should not crash)
    sea_striteri(NULL, map_to_upper);
    sea_striteri(s1, NULL);
    printf("Test: NULL inputs -> OK\n");
  }
  puts("\n---FD WRITER FUNCTIONS---");
  {
    #define TEST_FILE "test_output.tmp"
    int fd;
    char buffer[128];
    ssize_t bytes_read;

    // --- Test 1: sea_putchar_fd ---
    fd = open(TEST_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd != -1) {
        sea_putchar_fd('C', fd);
        close(fd);
        fd = open(TEST_FILE, O_RDONLY);
        bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        buffer[bytes_read] = '\0';
        close(fd);
        printf("Test: putchar_fd('C') -> %s\n", (strcmp(buffer, "C") == 0) ? "OK" : "FAIL");
    }

    // --- Test 2: sea_putstr_fd ---
    fd = open(TEST_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd != -1) {
        sea_putstr_fd("Hello World", fd);
        close(fd);
        fd = open(TEST_FILE, O_RDONLY);
        bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        buffer[bytes_read] = '\0';
        close(fd);
        printf("Test: putstr_fd(\"Hello World\") -> %s\n", (strcmp(buffer, "Hello World") == 0) ? "OK" : "FAIL");
    }

    // --- Test 3: sea_putendl_fd ---
    fd = open(TEST_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd != -1) {
        sea_putendl_fd("Line with newline", fd);
        close(fd);
        fd = open(TEST_FILE, O_RDONLY);
        bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        buffer[bytes_read] = '\0';
        close(fd);
        printf("Test: putendl_fd(\"...\") -> %s\n", (strcmp(buffer, "Line with newline\n") == 0) ? "OK" : "FAIL");
    }

    // --- Test 4: sea_putnbr_fd (Positive) ---
    fd = open(TEST_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd != -1) {
        sea_putnbr_fd(12345, fd);
        close(fd);
        fd = open(TEST_FILE, O_RDONLY);
        bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        buffer[bytes_read] = '\0';
        close(fd);
        printf("Test: putnbr_fd(12345) -> %s\n", (strcmp(buffer, "12345") == 0) ? "OK" : "FAIL");
    }

    // --- Test 5: sea_putnbr_fd (Negative) ---
    fd = open(TEST_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd != -1) {
        sea_putnbr_fd(-6789, fd);
        close(fd);
        fd = open(TEST_FILE, O_RDONLY);
        bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        buffer[bytes_read] = '\0';
        close(fd);
        printf("Test: putnbr_fd(-6789) -> %s\n", (strcmp(buffer, "-6789") == 0) ? "OK" : "FAIL");
    }

    // --- Test 6: sea_putnbr_fd (INT_MIN) ---
    fd = open(TEST_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd != -1) {
        sea_putnbr_fd(-2147483648, fd);
        close(fd);
        fd = open(TEST_FILE, O_RDONLY);
        bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        buffer[bytes_read] = '\0';
        close(fd);
        printf("Test: putnbr_fd(INT_MIN) -> %s\n", (strcmp(buffer, "-2147483648") == 0) ? "OK" : "FAIL");
    }

    unlink(TEST_FILE); // Clean up and delete the temporary file
  }
  puts("\n---LSTNEW (HEAP)---");
  {
    t_list *node1, *node2;
    int content1 = 42;
    char *content2 = "hello";

    // Test 1: Create a basic node with content
    node1 = sea_lstnew(&content1);
    PRINT_TEST("Create a node with int content",
               node1 != NULL && node1->content == &content1 && node1->next == NULL);

    // Test 2: Create a node with NULL content
    node2 = sea_lstnew(NULL);
    PRINT_TEST("Create a node with NULL content",
               node2 != NULL && node2->content == NULL && node2->next == NULL);
    free(node2); // Clean up

    // Test 3: Linking two nodes
    node2 = sea_lstnew(content2);
    node1->next = node2;
    PRINT_TEST("Linking two nodes together",
               node1->next == node2 && strcmp(node1->next->content, "hello") == 0);

    // Test 4: Content integrity (content is not copied)
    content1 = 99; // Modify the original content
    PRINT_TEST("Content is a shallow copy (pointer)",
               *(int*)node1->content == 99);

    // Test 5: Memory independence
    free(node1);
    free(node2);
    PRINT_TEST("Nodes can be freed individually",
               strcmp(content2, "hello") == 0); // Original content is untouched
  }
  puts("\n---ARENA_LSTNEW---");
  {
    t_mem *arena = sea_arena_init(sizeof(t_list) * 2); // Arena with space for exactly 2 nodes
    t_list *node1, *node2, *node3;
    int content1 = 42;
    size_t used_before, used_after;
    size_t aligned_node_size = (sizeof(t_list) + ARENA_ALIGN - 1) & ~(ARENA_ALIGN - 1);

    PRINT_TEST("Arena initialization", arena != NULL);

    // Test 1: Create a basic node in the arena
    used_before = arena->used;
    node1 = sea_arena_lstnew(arena, &content1);
    used_after = arena->used;
    PRINT_TEST("Create a node in the arena",
               node1 != NULL && *(int*)node1->content == 42);

    // Test 2: Arena 'used' pointer advanced correctly
    PRINT_TEST("Arena 'used' pointer advanced correctly",
               used_after == used_before + aligned_node_size);

    // Test 3: Creating a second node
    node2 = sea_arena_lstnew(arena, NULL);
    PRINT_TEST("Create a second node with NULL content",
               node2 != NULL && node2->content == NULL);

    // Test 4: Check if nodes are contiguous in memory
    PRINT_TEST("Nodes are contiguous in arena memory",
               (unsigned char*)node2 == (unsigned char*)node1 + aligned_node_size);

    // Test 5: Force chaining by allocating a third node
    node3 = sea_arena_lstnew(arena, "force chain");
    PRINT_TEST("Allocation forces a new block",
               arena->next != NULL && node3 != NULL);

    sea_arena_free(arena);
  }
  puts("\n---LSTADD_FRONT---");
  {
      // Test 1: Add to an empty list
      t_list *list1 = NULL;
      t_list *new_node1 = sea_lstnew("first node");
      sea_lstadd_front(&list1, new_node1);
      int is_ok1 = (list1 == new_node1 && list1->next == NULL);
      printf("Test: Add node to an empty list -> %s\n", is_ok1 ? "OK" : "FAIL");
      free(new_node1);
      // Test 2: Add to a non-empty list
      t_list *list2 = sea_lstnew("original head");
      t_list *new_node2 = sea_lstnew("new head");
      t_list *original_head = list2;
      sea_lstadd_front(&list2, new_node2);
      int is_ok2 = (list2 == new_node2 && list2->next == original_head);
      printf("Test: Add node to a non-empty list -> %s\n", is_ok2 ? "OK" : "FAIL");
      free(new_node2);
      free(original_head);
      // Test 3: Add a NULL new node
      t_list *list3 = sea_lstnew("original head");
      t_list *new_node3 = NULL;
      t_list *list3_original_head = list3;
      sea_lstadd_front(&list3, new_node3);
      int is_ok3 = (list3 == list3_original_head && list3->next == NULL);
      printf("Test: Add a NULL new node -> %s\n", is_ok3 ? "OK" : "FAIL");
      free(list3);
      // Test 4: Multiple additions
      t_list *list4 = NULL;
      t_list *node_a = sea_lstnew("A");
      t_list *node_b = sea_lstnew("B");
      t_list *node_c = sea_lstnew("C");
      sea_lstadd_front(&list4, node_a);
      sea_lstadd_front(&list4, node_b);
      sea_lstadd_front(&list4, node_c);
      int is_ok4 = (list4 == node_c && list4->next == node_b && list4->next->next == node_a && list4->next->next->next == NULL);
      printf("Test: Multiple additions (C, B, A) -> %s\n", is_ok4 ? "OK" : "FAIL");
      free(node_c);
      free(node_b);
      free(node_a);
  }
  puts("\n---LSTSIZE---");
  {

    t_list *head = NULL;
    int content = 42;

    // Test 1: Test with a NULL list
    PRINT_TEST("Size of a NULL list is 0", sea_lstsize(head) == 0);

    // Test 2: Test a list with one node
    lst_add_front(&head, sea_lstnew(&content));
    PRINT_TEST("Size of a 1-node list is 1", sea_lstsize(head) == 1);

    // Test 3: Test a list with two nodes
    lst_add_front(&head, sea_lstnew(&content));
    PRINT_TEST("Size of a 2-node list is 2", sea_lstsize(head) == 2);

    // Test 4: Test a list with three nodes
    lst_add_front(&head, sea_lstnew(&content));
    PRINT_TEST("Size of a 3-node list is 3", sea_lstsize(head) == 3);

    // Test 5: Test size from the middle of the list
    PRINT_TEST("Size from the 2nd node is 2", sea_lstsize(head->next) == 2);

    // Test 6: Test size from the last node of the list
    PRINT_TEST("Size from the last node is 1", sea_lstsize(head->next->next) == 1);

    free_test_list(head); // Clean up the first list
    head = NULL;

    // Test 7-10: Test with a longer, programmatically generated list
    int list_len = 100;
    for (int i = 0; i < list_len; i++) {
        lst_add_front(&head, sea_lstnew(&content));
    }
    PRINT_TEST("Size of a 100-node list is 100", sea_lstsize(head) == 100);
    PRINT_TEST("Size from the 50th node is 51", sea_lstsize(head->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next) == 51); // A bit silly but demonstrates the point
    PRINT_TEST("Size from the 99th node is 2", sea_lstsize(head->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next) == 2);
    PRINT_TEST("Sanity check size is still 100", sea_lstsize(head) == 100);

   free_test_list(head); // Clean up the second list
  }
  puts("\n---LSTLAST---");
  {
    t_list *head = NULL;
    t_list *last_node = NULL;

    // Test 1: Test with a NULL list
    PRINT_TEST("Last node of a NULL list is NULL", sea_lstlast(head) == NULL);

    // --- Heap-Allocated List Tests ---
    t_list *heap_node_a = sea_lstnew("A");
    t_list *heap_node_b = sea_lstnew("B");
    t_list *heap_node_c = sea_lstnew("C");

    // Test 2: Test a single-node list
    head = heap_node_a;
    last_node = sea_lstlast(head);
    PRINT_TEST("Last node of a 1-node list is the head", last_node == heap_node_a);

    // Test 3 & 4: Test a multi-node list
    head->next = heap_node_b;
    head->next->next = heap_node_c;
    last_node = sea_lstlast(head);
    PRINT_TEST("Last node of a 3-node list is correct", last_node == heap_node_c);
    PRINT_TEST("Content of last node is correct ('C')", strcmp(last_node->content, "C") == 0);

    // Test 5: Test from the middle of a list
    last_node = sea_lstlast(head->next); // Start search from node B
    PRINT_TEST("Last node from the middle is still 'C'", last_node == heap_node_c);

    // Test 6: Check that the 'next' of the last node is NULL
    PRINT_TEST("'next' pointer of the last node is NULL", last_node->next == NULL);

    // Test 7: Test after adding a new node to the back
    t_list *heap_node_d = sea_lstnew("D");
    sea_lstadd_back(&head, heap_node_d);
    last_node = sea_lstlast(head);
    PRINT_TEST("Finds the new last node after lstadd_back", last_node == heap_node_d);

    free_test_list(head); // Clean up ALL heap-allocated nodes (A, B, C, and D)

    // --- Arena-Allocated List Test ---
    t_mem *arena = sea_arena_init(0);
    head = NULL; // Reset head for the new list

    // Test 8, 9, 10: Use NEW variable names to avoid conflicts
    t_list *arena_node_a = sea_arena_lstnew(arena, "Arena A");
    t_list *arena_node_b = sea_arena_lstnew(arena, "Arena B");
    arena_node_a->next = arena_node_b;
    head = arena_node_a;
    last_node = sea_lstlast(head);
    PRINT_TEST("Works with arena-allocated nodes", last_node == arena_node_b);
    PRINT_TEST("Content of arena-based last node is correct", strcmp(last_node->content, "Arena B") == 0);
    PRINT_TEST("Arena list sanity check", sea_lstlast(arena_node_b) == arena_node_b);
    sea_arena_free(arena); // Clean up arena and all nodes within it
  }
puts("\n---LSTDELONE---");
  {
      // Test 1: Standard deletion
    g_del_called_flag = 0;
    int *content1 = (int*)malloc(sizeof(int));
    t_list *node1 = sea_lstnew(content1);
    sea_lstdelone(node1, test_del_func);
    PRINT_TEST("Deletes a node and calls the del function", g_del_called_flag == 1);

    // Test 2: Deleting a node with NULL content
    g_del_called_flag = 0;
    t_list *node2 = sea_lstnew(NULL);
    sea_lstdelone(node2, test_del_func);
    PRINT_TEST("Handles a node with NULL content", g_del_called_flag == 1);

    // Test 3: Deleting with a NULL 'del' function (node is freed, content is not)
    t_list *node3 = sea_lstnew((void*)"unfreed content");
    sea_lstdelone(node3, NULL);
    PRINT_TEST("Handles a NULL del function (no crash)", 1);

    // Test 4: Deleting a NULL node
    g_del_called_flag = 0;
    sea_lstdelone(NULL, test_del_func);
    PRINT_TEST("Handles a NULL node input (no crash)", g_del_called_flag == 0);

    // Test 5: Does not affect the next node
    t_list *node_a = sea_lstnew((void*)"A");
    t_list *node_b = sea_lstnew((void*)"B");
    node_a->next = node_b;
    sea_lstdelone(node_a, noop_del);
    PRINT_TEST("Does not free the next node", strcmp((char*)node_b->content, "B") == 0);
    free(node_b); // Manually clean up the remaining node for this specific test
  }
  puts("\n---LSTCLEAR---");
  {
    // Test 1: Clear a standard list of 3 nodes
    g_del_counter = 0;
    t_list *list1 = sea_lstnew(malloc(1));
    sea_lstadd_back(&list1, sea_lstnew(malloc(1)));
    sea_lstadd_back(&list1, sea_lstnew(malloc(1)));
    sea_lstclear(&list1, free); // Use standard free as the deleter
    PRINT_TEST("Clear a 3-node list (head is NULL)", list1 == NULL);


    // Test 2: Clear a single-node list
    t_list *list2 = sea_lstnew(malloc(1));
    sea_lstclear(&list2, free);
    PRINT_TEST("Clear a single-node list (head is NULL)", list2 == NULL);

    // Test 3: Clear an already empty list
    t_list *list3 = NULL;
    sea_lstclear(&list3, free);
    PRINT_TEST("Clear an empty list (head is NULL)", list3 == NULL);

    // Test 4: Clear with a NULL pointer-to-pointer (no crash)
    sea_lstclear(NULL, free);
    PRINT_TEST("Clear with a NULL parameter (no crash)", 1);

    // Test 5: Clear with a NULL 'del' function (no crash)
    // This test WILL leak memory by design, which is the correct behavior.
    t_list *list5 = sea_lstnew(malloc(1));
    sea_lstadd_back(&list5, sea_lstnew(malloc(1)));
    sea_lstclear(&list5, NULL);
    PRINT_TEST("Clear with a NULL del function (head is NULL)", list5 == NULL);
  }
puts("\n---LSTITER---");
    {
        // Test 1: Iterate over a multi-node list with a modifying function
        t_list *lst1 = sea_lstnew(sea_strdup("hello"));
        lst1->next = sea_lstnew(sea_strdup("world"));
        sea_lstiter(lst1, &simple_toupper_func);
        PRINT_TEST("Multi-node list, applies uppercase",
            strcmp(lst1->content, "HELLO") == 0 && strcmp(lst1->next->content, "WORLD") == 0);
        free_list(lst1);

        // Test 2: Iterate over an empty list (NULL list pointer)
        g_iter_called_count = 0;
        sea_lstiter(NULL, &test_count_func);
        PRINT_TEST("NULL list pointer input", g_iter_called_count == 0);

        // Test 3: Iterate over a single-node list
        g_iter_called_count = 0;
        t_list *lst3 = sea_lstnew(sea_strdup("a"));
        sea_lstiter(lst3, &test_count_func);
        PRINT_TEST("Single-node list", g_iter_called_count == 1);
        free_list(lst3);

        // Test 4: Iterate with a NULL function pointer
        g_iter_called_count = 0;
        t_list *lst4 = sea_lstnew(sea_strdup("test"));
        sea_lstiter(lst4, NULL);
        PRINT_TEST("NULL function pointer input (no crash)", 1);
        free_list(lst4);

        // Test 5: Iterate over a list with some empty content
        t_list *lst5 = sea_lstnew(sea_strdup("case"));
        lst5->next = sea_lstnew(sea_strdup(""));
        lst5->next->next = sea_lstnew(sea_strdup("sensitive"));
        sea_lstiter(lst5, &simple_toupper_func);
        PRINT_TEST("List with empty string content",
            strcmp(lst5->content, "CASE") == 0 && strcmp(lst5->next->content, "") == 0 && strcmp(lst5->next->next->content, "SENSITIVE") == 0);
        free_list(lst5);

        // Test 6: Iteration with a node having NULL content
        g_iter_called_count = 0;
        t_list *lst6 = sea_lstnew(sea_strdup("valid"));
        lst6->next = sea_lstnew(NULL); // Node with NULL content
        lst6->next->next = sea_lstnew(sea_strdup("valid again"));
        sea_lstiter(lst6, &test_count_func);
        PRINT_TEST("List with NULL content in one node", g_iter_called_count == 3);
        sea_lstclear(&lst6, free);

        // Test 7: A no-op function that only counts calls
        g_iter_called_count = 0;
        t_list *lst7 = sea_lstnew(sea_strdup("a"));
        lst7->next = sea_lstnew(sea_strdup("b"));
        lst7->next->next = sea_lstnew(sea_strdup("c"));
        sea_lstiter(lst7, &test_count_func);
        PRINT_TEST("Correct number of calls for a 3-node list", g_iter_called_count == 3);
        free_list(lst7);
    }
    puts("\n---LSTMAP---");
    {
        // --- Test 1: Basic multi-node mapping ---
        t_list *orig_lst1 = sea_lstnew(sea_strdup("one"));
        sea_lstadd_back(&orig_lst1, sea_lstnew(sea_strdup("two")));
        t_list *new_lst1 = sea_lstmap(orig_lst1, &map_strdup_toupper, free);

        // Build expected list
        t_list *expected_lst1 = sea_lstnew(sea_strdup("ONE"));
        sea_lstadd_back(&expected_lst1, sea_lstnew(sea_strdup("TWO")));

        PRINT_TEST("Multi-node list mapping", new_lst1 != NULL && compare_lists(new_lst1, expected_lst1));
        printf("  Test: %-50s -> %s\n", "Original list is unchanged", (strcmp(orig_lst1->content, "one") == 0) ? "OK" : (char*)orig_lst1->content);

        sea_lstclear(&orig_lst1, free);
        sea_lstclear(&new_lst1, free);
        sea_lstclear(&expected_lst1, free);

        // --- Test 2: Empty list input ---
        t_list *new_lst2 = sea_lstmap(NULL, &map_strdup_toupper, free);
        PRINT_TEST("NULL list input", new_lst2 == NULL);

        // --- Test 3: NULL 'f' function ---
        t_list *orig_lst3 = sea_lstnew(sea_strdup("test"));
        t_list *new_lst3 = sea_lstmap(orig_lst3, NULL, free);
        PRINT_TEST("NULL 'f' function input", new_lst3 == NULL);
        sea_lstclear(&orig_lst3, free);

        // --- Test 4: NULL 'del' function ---
        t_list *orig_lst4 = sea_lstnew(sea_strdup("test"));
        t_list *new_lst4 = sea_lstmap(orig_lst4, &map_strdup_toupper, NULL);
        PRINT_TEST("NULL 'del' function input", new_lst4 == NULL);
        sea_lstclear(&orig_lst4, free);

        // --- Test 5: List with NULL content ---
        t_list *orig_lst5 = sea_lstnew(sea_strdup("a"));
        sea_lstadd_back(&orig_lst5, sea_lstnew(NULL)); // NULL content
        sea_lstadd_back(&orig_lst5, sea_lstnew(sea_strdup("c")));

        t_list *new_lst5 = sea_lstmap(orig_lst5, &map_strdup_toupper, free);

        // Build expected list
        t_list *expected_lst5 = sea_lstnew(sea_strdup("A"));
        sea_lstadd_back(&expected_lst5, sea_lstnew(NULL));
        sea_lstadd_back(&expected_lst5, sea_lstnew(sea_strdup("C")));

        PRINT_TEST("List with NULL content", new_lst5 != NULL && compare_lists(new_lst5, expected_lst5));

        sea_lstclear(&orig_lst5, free);
        sea_lstclear(&new_lst5, free);
        sea_lstclear(&expected_lst5, free);
    }

    puts("\n---ARENA_LSTMAP---");
    {
        // --- Test 1: Basic mapping in arena ---
        t_mem *arena1 = sea_arena_init(1024); // 1KB arena
        t_list *orig_lst1 = sea_lstnew(sea_strdup("one")); // Original list is on heap
        sea_lstadd_back(&orig_lst1, sea_lstnew(sea_strdup("two")));

        t_list *new_lst1 = sea_arena_lstmap(arena1, orig_lst1, &map_strdup_toupper, free);

        // Build expected list (for comparison, content is on heap)
        t_list *expected_lst1 = sea_lstnew(sea_strdup("ONE"));
        sea_lstadd_back(&expected_lst1, sea_lstnew(sea_strdup("TWO")));

        PRINT_TEST("Arena: Multi-node list mapping", new_lst1 != NULL && compare_lists(new_lst1, expected_lst1));

        // Check that new nodes are actually in the arena
        PRINT_TEST("Arena: New list head is in arena", (unsigned char*)new_lst1 >= arena1->mem && (unsigned char*)new_lst1 < (arena1->mem + arena1->total));
        PRINT_TEST("Arena: New list next node is in arena", (unsigned char*)new_lst1->next >= arena1->mem && (unsigned char*)new_lst1->next < (arena1->mem + arena1->total));

        // Cleanup: Free original list (heap), expected list (heap),
        // and arena (which frees new nodes).
        // The *content* of new_lst1 was heap-allocated by map_strdup_toupper,
        // so we must free it before freeing the arena.
        sea_lstiter(new_lst1, free); // Use lstiter to free only content
        sea_arena_free(arena1);      // Frees the arena and the nodes (new_lst1)
        sea_lstclear(&orig_lst1, free);
        sea_lstclear(&expected_lst1, free);

        // --- Test 2: Allocation failure (arena too small) ---
        size_t node_size = sizeof(t_list);
        t_mem *small_arena = sea_arena_init(node_size * 2); // Arena for only 2 nodes

        t_list *orig_lst2 = sea_lstnew(sea_strdup("a"));
        sea_lstadd_back(&orig_lst2, sea_lstnew(sea_strdup("b")));
        sea_lstadd_back(&orig_lst2, sea_lstnew(sea_strdup("c"))); // 3 nodes

        t_list *new_lst2 = sea_arena_lstmap(small_arena, orig_lst2, &map_strdup_toupper, free);

        // Test that the allocation SUCCEEDED by growing
        PRINT_TEST("Arena: Allocation succeeds by growing", new_lst2 != NULL);
        if (small_arena) {
            // Test that the *first* block is full
            PRINT_TEST("Arena: Original block is full", small_arena->used == node_size * 2);
            // Test that a *new* block was created
            PRINT_TEST("Arena: A new block was created", small_arena->next != NULL);
        }

        sea_lstclear(&orig_lst2, free);
        sea_arena_free(small_arena);

        // --- Test 3: NULL 'arena' input ---
        t_list *orig_lst3 = sea_lstnew(sea_strdup("test"));
        t_list *new_lst3 = sea_arena_lstmap(NULL, orig_lst3, &map_strdup_toupper, free);
        PRINT_TEST("Arena: NULL 'arena' input", new_lst3 == NULL);
        sea_lstclear(&orig_lst3, free);

        // --- Test 4: NULL 'f' or 'del' function ---
        t_mem *arena4 = sea_arena_init(1024);
        t_list *orig_lst4 = sea_lstnew(sea_strdup("test"));

        t_list *new_lst4_f = sea_arena_lstmap(arena4, orig_lst4, NULL, free);
        PRINT_TEST("Arena: NULL 'f' function input", new_lst4_f == NULL);

        t_list *new_lst4_del = sea_arena_lstmap(arena4, orig_lst4, &map_strdup_toupper, NULL);
        PRINT_TEST("Arena: NULL 'del' function input", new_lst4_del == NULL);

        sea_lstclear(&orig_lst4, free);
        sea_arena_free(arena4);
    }
  puts("\nDone!");
  return (0);
}
