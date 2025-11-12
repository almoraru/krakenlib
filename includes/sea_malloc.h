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
/*      Filename: malloc.h                                                    */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/11/11 22:35:26 by espadara                              */
/*      Updated: 2025/11/11 22:35:26 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEA_MALLOC_H
#define SEA_MALLOC_H

/*
** ---------- INCLUDES ----------
*/
#include <fcntl.h>
#include "sea_core.h"
#include "sea_printf.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include <sys/mman.h>
#include <pthread.h>

/*
** ---------- BONUS GLOBAL ----------
*/

extern pthread_mutex_t  g_malloc_mutex;

/*
** ---------- SIZE THESHOLDS ----------
*/

#define ZONE_TINY       (1 << 10)
#define ZONE_SMALL      (1 << 6)
#define MALLOC_ZONE     (1 << 7)
#define MASK_0XFFF      (1 << 12) - 1
#define HEXA_LIMIT 0x10
#define TRUE 1
#define FALSE 0

/*
** ---------- STRUCTS ----------
*/

typedef enum
{
    MALLOC_TINY,
    MALLOC_SMALL,
    MALLOC_LARGE
}                   t_malloc_type;


typedef struct      s_block
{
    struct s_block  *next;
    struct s_block  *prev;
    size_t          size;
    size_t          align;
}                   t_block;

typedef struct      s_page
{
    struct s_page   *next;
    struct s_page   *prev;
    t_block         *alloc;
    t_block         *free;
}                   t_page;

typedef struct      s_malloc_pages
{
    t_page          *tiny;
    t_page          *small;
    t_block         *large;
}                   t_malloc_pages;

extern  t_malloc_pages g_malloc_pages;


/*
** ---------- MAIN FUNCTIONS ----------
*/
void                *malloc(size_t size);
void                free(void *ptr);
void                *realloc(void *ptr, size_t size);
void                *calloc(size_t count, size_t size);
void                *reallocf(void *ptr, size_t size);

/*
** ---------- UTIL FUNCTIONS ----------
*/
size_t              align_mem(size_t size, size_t mask);
bool                check_block(const void *ptr, size_t size);
void                putaddr(void *addr);
size_t              page_size(size_t type);

/*
** ---------- PRINT FUNTCIONS ----------
*/
void                show_alloc_mem();
void                show_alloc_mem_ex(void *ptr);
void                show_alloc_mem_minimap();
void                ft_putsizebase(size_t nb, int size_base);

/*
** ---------- ERROR FUNCTIONS ----------
*/
bool                error_malloc(const int err, const char *str);


#endif
