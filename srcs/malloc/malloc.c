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
/*      Filename: malloc.c                                                    */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/11/11 22:36:00 by espadara                              */
/*      Updated: 2025/11/13 08:53:42 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_malloc.h"

t_malloc_pages g_malloc_pages = {0, 0, 0};
pthread_mutex_t g_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;

static inline void *block_create(t_block **free, t_block **alloc,
                                 const size_t size)
{
t_block *new_block;
    t_block *next_free;

    if (!free || !alloc || !*free)
      return NULL;

    new_block = *free;
    next_free = new_block->next;

    *free = next_free;
    if (*free) {
        // DEBUG: Check if next_free points to valid memory
        if ((void *)*free < (void *)0x1000) {  // Likely invalid
            write(2, "ERROR: Invalid free block pointer\n", 35);
            return NULL;
        }
        (*free)->prev = NULL;
    }
    new_block->next = *alloc;
    new_block->prev = NULL;
    new_block->size = size;

    if (*alloc)
      (*alloc)->prev = new_block;
    *alloc = new_block;

    return ((void *)new_block + sizeof(t_block));
}

static inline void mem_init_zone(t_page **page,
                                 t_page *mem, const size_t zone_size)
{
  t_block *free_block;

  free_block = (void *)mem + sizeof(t_page);
  mem->prev = NULL;
  if ((mem->next = *page))
    mem->next->prev = mem;
  *page = mem;
  mem->alloc = NULL;
  mem->free = free_block;
  while ((void *)free_block + (zone_size + sizeof(t_block)) * 2 <
         (void *)mem + zone_size * MALLOC_ZONE)
  {
    free_block->next = (void *)free_block + sizeof(t_block) + zone_size;
    free_block->next->prev = free_block;
    free_block = free_block->next;
  }
  free_block->next = NULL;
}

static inline void *malloc_tiny_small(t_page **page,
                                      const size_t zone_size, const size_t size)
{
  t_page *mem;

  mem = *page;
  while (mem && !mem->free)
    mem = mem->next;
  if (!mem)
  {
    if ((mem = mmap(0, zone_size * MALLOC_ZONE, PROT_READ | PROT_WRITE,
                    MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
      return ((void *)error_malloc(2, "Could not allocate large malloc"));
    mem_init_zone(page, mem, zone_size);
  }
  return (block_create(&mem->free, &mem->alloc, align_mem(size, 31)));
}

static void *malloc_large(size_t size)
{
  const size_t msize = align_mem(size + sizeof(t_block), MASK_0XFFF);
  t_block *block;

  if (((block = mmap(0, msize, PROT_READ | PROT_WRITE,
                     MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED))
    return ((void *)error_malloc(2, "Could not allocate large malloc"));
  block->size = align_mem(size, 31);
  block->prev = NULL;
  if ((block->next = g_malloc_pages.large))
    g_malloc_pages.large->prev = block;
  g_malloc_pages.large = block;
  return ((void *)block + sizeof(t_block));
}

__attribute__((hot))
void *malloc(size_t size)
{
  const size_t type = (size > ZONE_TINY) + (size > ZONE_SMALL);
  void *ptr = NULL;

  if (!size)
    return (NULL);
  pthread_mutex_lock(&g_malloc_mutex);
  if (type == MALLOC_TINY)
    ptr = malloc_tiny_small(&g_malloc_pages.tiny, ZONE_TINY, size);
  else if (type == MALLOC_SMALL)
    ptr = malloc_tiny_small(&g_malloc_pages.small, ZONE_SMALL, size);
  else
    ptr = malloc_large(size);
  pthread_mutex_unlock(&g_malloc_mutex);
  return (ptr);
}
