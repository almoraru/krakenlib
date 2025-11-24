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
/*      Filename: sealib.h                                                    */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/08/23 15:35:18 by espadara                              */
/*      Updated: 2025/11/24 15:36:13 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEA_CORE_H
# define SEA_CORE_H

/* INCLUDES */

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <ctype.h>
# include <string.h>
# include <bsd/string.h>
# include <math.h>
# include <immintrin.h>
# include <limits.h>
# include <fcntl.h>
# include <sys/mman.h>
# include <stdint.h>

/* DEFINES  */

# define ARENA_DEFAULT 4096
# define ARENA_ALIGN 16

/* STRUCTURES */

typedef struct	s_list
{
  void *content;
  struct s_list *next;
}				t_list;

typedef struct	s_mem
{
  struct s_mem *next;
  size_t total;
  size_t used;
  unsigned char *mem;
}				t_mem;

/* FUNCTIONS */

/* BOOLEANS  */
int	sea_isalpha(int c);
int	sea_isdigit(int c);
int	sea_isalnum(int c);
int	sea_isascii(int c);
int	sea_isprint(int c);
int	sea_iswhitespace(char c);

/* CHARACTERS */
int	sea_toupper(int c);
int	sea_tolower(int c);

/* STRINGS */
size_t	sea_strlen(const char *s);
char	*sea_strcpy(char *dest, const char *src);
char	*sea_strncpy(char *dest, const char *src, size_t dsize);
size_t	sea_strlcpy(char *dst, const char *src, size_t size);
size_t	sea_strlcat(char *dst, const char *src, size_t size);
void	*sea_bzero(void *s, size_t n);
char	*sea_strchr(const char *s, int c);
char	*sea_strrchr(const char *s, int c);
char	*sea_strstr(const char *haystack, const char *needle);
int	sea_strcmp(const char *s1, const char *s2);
int	sea_strncmp(const char *s1, const char *s2, size_t n);
int	sea_strncmp(const char *s1, const char *s2, size_t n);
char	*sea_strnstr(const char *haystack, const char *needle, size_t len);
char	*sea_strdup(const char *s);
char	*sea_arena_strdup(t_mem *arena, const char *src);
char	*sea_strsub(char const *s, unsigned int start, size_t len);
char	*sea_arena_strsub(t_mem *arena, char const *s, unsigned int start, size_t len);
char	*sea_strjoin(char const *s1, char const *s2);
char	*sea_arena_strjoin(t_mem *arena, char const *s1, char const *s2);
char	*sea_strtrim(char const *s1, char const *set);
char	**sea_split(char const *s, char c);
char	**sea_arena_split(t_mem *arena, char const *s, char c);
char	*sea_itoa(int n);
char	*sea_strmapi(char const *s, char (*f)(unsigned int, char));
void	sea_striteri(char *s, void (*f)(unsigned int, char *));

/* MEMORY */
void	*sea_memset(void *s, int c, size_t n);
void	*sea_memcpy(void *dest, const void *src, size_t n);
void	*sea_memmove(void *dest, const void *src, size_t n);
void	*sea_memchr(const void *s, int c, size_t n);
int	sea_memcmp(const void *s1, const void *s2, size_t n);
t_mem	*sea_arena_init(size_t size);
void	*sea_arena_alloc(t_mem *arena, size_t size);
void	sea_arena_free(t_mem *arena);
void	*sea_memcpy_fast(void *dest, const void *src, size_t n);

/* CONVERSIONS */
int	sea_atoi(const char *nptr);
int	sea_atoi_base(const char *str, int base);
double	sea_atof(const char *str);

/* INPUT - OUTPUT  */
void	sea_putchar_fd(char c, int fd);
void	sea_putstr_fd(char const *s, int fd);
void	sea_putendl_fd(char *s, int fd);
void	sea_putnbr_fd(int n, int fd);

/* LISTS  */
t_list	*sea_lstnew(void *content);
t_list	*sea_arena_lstnew(t_mem *arena, void *content);
void	sea_lstadd_front(t_list **lst, t_list *new);
int	sea_lstsize(t_list *lst);
t_list	*sea_lstlast(t_list *lst);
void	sea_lstadd_back(t_list **lst, t_list *new);
void	sea_lstdelone(t_list *lst, void (*del)(void*));
void	sea_lstclear(t_list **lst, void (*del)(void*));
void	sea_lstiter(t_list *lst, void (*f)(void*));
t_list	*sea_lstmap(t_list *lst, void *(*f)(void*), void (*del)(void*));
t_list  *sea_arena_lstmap(t_mem *arena, t_list *lst, void *(*f)(void *), void (*del)(void *));



#endif
