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
/*      Updated: 2025/11/23 17:48:52 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
#define MALLOC_H

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
#include <stdint.h>
#include <pthread.h>

/*
** ---------- SIZE THESHOLDS ----------
*/

# define PAGE_SIZE       4096

/* ** 1. TINY Definition
** Range: 1 to 128 bytes (n = 128)
** Zone Size: Must hold 100 blocks. 128 * 100 = 12800.
** Aligned to Page: 16384 bytes (4 pages)
*/

# define TINY_BLOCK_MAX  128
# define TINY_ZONE_SIZE  16384

/* ** 2. SMALL Definition
** Range: 129 to 1024 bytes (m = 1024)
** Zone Size: Must hold 100 blocks. 1024 * 100 = 102400.
** Aligned to Page: 106496 bytes (26 pages)
*/

# define SMALL_BLOCK_MAX 8192
# define SMALL_ZONE_SIZE (1024 * 1024)

/*
** Alignment: Minimum block size (16 bytes for 128-bit SIMD safety) = ez memfastcpy
*/

# define MIN_ALIGNMENT   16

/* ** Size Classes:
** We don't just dump everything in a zone. We segregate by size.
** Tiny: 16, 32, 48, ... 128 (8 classes)
** Small: 144, 160, ... 1024 (Many classes)
*/
# define MAX_TINY_CLASSES (TINY_BLOCK_MAX / MIN_ALIGNMENT)
# define MAX_SMALL_CLASSES (SMALL_BLOCK_MAX / MIN_ALIGNMENT)

# define CACHE_SIZE 4

/*
** ---------- STRUCTS ----------
*/

/* ** The Bitmap:
    ** 0 = Free, 1 = Used.
    ** Tiny Zone (16KB) / Min Block (16B) = 1024 blocks max.
    ** 1024 bits / 64 bits per int = 16 integers.
*/

//This structure sits at the VERY BEGINNING of every mmap'd zone (N or M bytes).
typedef struct	s_slab
{
    struct s_slab *next;
    struct s_slab *prev;

    size_t block_size;
    size_t total_blocks;
    size_t free_count;

    uint64_t bitmap[16];
}	t_slab;


/* ** t_heap: The Global Manager
** Instead of one list, we have an array of lists.
** request 30 bytes -> round to 32 -> go to index 1 -> O(1) lookup.
** large is still a linked liste
*/
typedef struct s_heap
{
    t_slab *tiny[MAX_TINY_CLASSES];
    t_slab *small[MAX_SMALL_CLASSES];
    t_slab *large;
    t_slab *cache_large;
    size_t cache_count;
}	t_heap;

/*
** ---------- GLOBALS -------------
*/
extern t_heap g_heap;
extern pthread_mutex_t g_malloc_mutex;

/*
** ---------- PROTOTYPES ----------
*/
void	*sea_malloc(size_t size);
void	sea_free(void *ptr);
void	*sea_realloc(void *ptr, size_t size);
void	*sea_calloc(size_t count, size_t size);

/* Helper functions */
void	show_alloc_mem(void);
void	show_alloc_mem_ex(void *ptr);
t_slab	*find_slab_by_ptr(void *ptr, int *type_out);

#endif
