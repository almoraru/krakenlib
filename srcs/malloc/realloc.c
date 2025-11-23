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
/*      Updated: 2025/11/23 16:52:20 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_malloc.h"

__attribute__((visibility("default")))
void *sea_realloc(void *ptr, size_t size)
{
  t_slab  *slab;
  size_t  old_size;
  void    *new_ptr;
  int     type;

  if (!ptr)
    return (sea_malloc(size));
  if (size == 0)
    {
      sea_free(ptr);
      return (NULL);
    }
  pthread_mutex_lock(&g_malloc_mutex);
  slab = find_slab_by_ptr(ptr, &type);
  if (!slab)
    {
      pthread_mutex_unlock(&g_malloc_mutex);
      return (NULL);
    }
  old_size = slab->block_size;
  if (size <= old_size)
    {
      pthread_mutex_unlock(&g_malloc_mutex);
      return (ptr);
    }
  pthread_mutex_unlock(&g_malloc_mutex);
  new_ptr = sea_malloc(size);
  if (!new_ptr)
    return (NULL);
  sea_memcpy_fast(new_ptr, ptr, old_size);
  sea_free(ptr);

  return (new_ptr);
}

__attribute__((visibility("default")))
void *sea_reallocf(void *ptr, size_t size)
{
  void *new_ptr;
  new_ptr = sea_realloc(ptr, size);
  if (!new_ptr && ptr)
    sea_free(ptr);
  return (new_ptr);
}

__attribute__((visibility("default")))
void *sea_calloc(size_t count, size_t size)
{
  void *ptr;
  size_t total;

  total = count * size;
  if (count != 0 && total / count != size)
    return (NULL);

  ptr = sea_malloc(total);
  if (ptr)
    sea_bzero(ptr, total);
  return (ptr);
}
