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
/*      Filename: realloc.c                                                   */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/11/11 22:39:32 by espadara                              */
/*      Updated: 2025/11/13 09:05:30 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_malloc.h"

bool check_block(const void *ptr)
{
    t_page *page;
    t_block *block;
    void *block_addr;

    if (!ptr)
        return (false);

    // Sanity check the global state
    if (g_malloc_pages.tiny && ((void *)g_malloc_pages.tiny < (void *)0x1000 ||
                                (void *)g_malloc_pages.tiny > (void *)0x7fffffffffff))
    {
        write(2, "ERROR: g_malloc_pages.tiny is corrupted!\n", 42);
        return (false);
    }

    if (g_malloc_pages.small && ((void *)g_malloc_pages.small < (void *)0x1000 ||
                                 (void *)g_malloc_pages.small > (void *)0x7fffffffffff))
    {
        write(2, "ERROR: g_malloc_pages.small is corrupted!\n", 43);
        return (false);
    }
  block = g_malloc_pages.large;
  while (block)
    {
      block_addr = (void *)block + sizeof(t_block);
      if (block_addr == ptr)
        return (true);
      block = block->next;
    }

    page = g_malloc_pages.small;
    while (page)
      {
        block = page->alloc;
        while (block)
          {
            block_addr = (void *)block + sizeof(t_block);
            if (block_addr == ptr)
              return (true);
            block = block->next;
          }
        page = page->next;
      }

    page = g_malloc_pages.tiny;
    while (page)
      {
        block = page->alloc;
        while (block)
          {
            block_addr = (void *)block + sizeof(t_block);
            if (block_addr == ptr)
              return (true);
            block = block->next;
          }
        page = page->next;
      }

    return (false);
}

void *realloc(void *ptr, size_t size)
{
    void *new;
  t_block *block_header;

  sea_printf("DEBUG realloc: ptr=%p, size=%u\n", ptr, size);

  if (!ptr)
  {
    sea_printf("  ptr is NULL, calling malloc\n");
    return (malloc(size));
  }
  if (!size)
  {
    sea_printf("  size is 0, freeing\n");
    free(ptr);
    return (NULL);
  }

  pthread_mutex_lock(&g_malloc_mutex);

  sea_printf("  Calling check_block(%p, %d)\n", ptr, ZONE_SMALL + 1);
  int valid = check_block(ptr);
  sea_printf("  check_block returned: %d\n", valid);

  if (!valid)
  {
    pthread_mutex_unlock(&g_malloc_mutex);
    sea_printf("  check_block FAILED, returning NULL\n");
    return (NULL);
  }

  block_header = ptr - sizeof(t_block);
  sea_printf("  block_header=%p, current size=%u, requested size=%u\n",
             block_header, block_header->size, size);

  if (size <= block_header->size)
  {
    sea_printf("  Size fits in current block, updating size\n");
    block_header->size = align_mem(size, 31);
    pthread_mutex_unlock(&g_malloc_mutex);
    return (ptr);
  }

  pthread_mutex_unlock(&g_malloc_mutex);

  sea_printf("  Need larger block, calling malloc(%u)\n", size);
  new = malloc(size);
  if (!new)
  {
    sea_printf("  malloc FAILED\n");
    return (NULL);
  }

  sea_printf("  Copying %u bytes from %p to %p\n", block_header->size, ptr, new);
  sea_memcpy_fast(new, ptr, block_header->size);

  sea_printf("  Freeing old block %p\n", ptr);
  free(ptr);

  sea_printf("  Returning new block %p\n", new);
  return (new);
}

void *reallocf(void *ptr, size_t size)
{
  void *new;

  if (!(new = realloc(ptr, size)))
    free(ptr);
  return (new);
}

void *calloc(size_t count, size_t size)
{
  void *ptr;

  if ((ptr = malloc(count * size)))
    sea_bzero(ptr, count * size);
  return (ptr);
}
