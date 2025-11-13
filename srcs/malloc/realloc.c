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
/*      Updated: 2025/11/13 08:11:35 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_malloc.h"

bool check_block(const void *ptr, size_t size)
{
  void *block_addr;
  t_block *block;

  if (size > ZONE_SMALL)
    block = g_malloc_pages.large;
  else if (size > ZONE_TINY)
    block = (t_block *)g_malloc_pages.small + sizeof(t_page);
  else
    block = (t_block *)g_malloc_pages.tiny + sizeof(t_page);
  block_addr = block + sizeof(t_block);
  while (block && block_addr < ptr)
  {
    if (block_addr == ptr)
      return (true);
    block = block->next;
    block_addr = block + sizeof(t_block);
  }
  return (false);
}

void *realloc(void *ptr, size_t size)
{
  void *new;
  t_block *block_header;

  if (!ptr)
    return (malloc(size));
  if (!size)
  {
    free(ptr);
    return (NULL);
  }

  pthread_mutex_lock(&g_malloc_mutex);

  if (!check_block(ptr, ZONE_SMALL + 1))
  {
    pthread_mutex_unlock(&g_malloc_mutex);
    return (NULL);
  }

  block_header = ptr - sizeof(t_block);

  if (size <= block_header->size)
  {
    block_header->size = align_mem(size, 31);
    pthread_mutex_unlock(&g_malloc_mutex);
    return (ptr);
  }

  pthread_mutex_unlock(&g_malloc_mutex);

  new = malloc(size);
  if (!new)
    return (NULL);

  sea_memcpy_fast(new, ptr, block_header->size);
  free(ptr);

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
