
#include "sea_malloc.h"

static void free_unused_mem(const int malloc_size, t_page *mem)
{
  size_t const zone_sizes[2] = {ZONE_TINY, ZONE_SMALL};

  if (mem->prev)
    mem->prev->next = mem->next;
  else
  {
    if (malloc_size == MALLOC_TINY)
      g_malloc_pages.tiny = mem->next;
    else
      g_malloc_pages.small = mem->next;
  }
  if (mem->next)
    mem->next->prev = mem->prev;
  munmap(mem, MALLOC_ZONE * zone_sizes[malloc_size]);
}

static inline void free_not_large(t_block *block,
                                  const int malloc_size, t_page *mem)
{
  if (block->prev)
    block->prev->next = block->next;
  else
    mem->alloc = block->next;
  if (block->next)
    block->next->prev = block->prev;
  block->prev = NULL;
  block->next = mem->free;
  if (mem->free)
    mem->free->prev = block;
  mem->free = block;
  if (!mem->alloc)
    free_unused_mem(malloc_size, mem);
}

static inline void free_large(t_block *block)
{
  const size_t msize = align_mem(block->size + sizeof(t_block), MASK_0XFFF);

  if (block->prev)
    block->prev->next = block->next;
  else
    g_malloc_pages.large = block->next;
  if (block->next)
    block->next->prev = block->prev;
  munmap(block, msize);
}

static void free_block(t_block *block)
{
  const int type = page_size(block->size);
  size_t const zone_sizes[2] = {ZONE_TINY, ZONE_SMALL};
  t_page *mem;

  if (type == MALLOC_LARGE)
    free_large(block);
  else
  {
    mem = (type == MALLOC_TINY) ? g_malloc_pages.tiny : g_malloc_pages.small;

    while (mem)
    {
      if ((void *)block >= (void *)mem &&
          (void *)block < (void *)mem + MALLOC_ZONE * zone_sizes[type])
        break;
      mem = mem->next;
    }

    if (!mem)
    {
      write(2, "ERROR: Attempting to free invalid block\n", 41);
      return;
    }

    free_not_large(block, type, mem);
  }
}

__attribute__((hot))
void free(void *ptr)
{
  pthread_mutex_lock(&g_malloc_mutex);
  if (ptr && check_block(ptr))
    free_block(ptr - sizeof(t_block));
  pthread_mutex_unlock(&g_malloc_mutex);
}
