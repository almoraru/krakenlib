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
/*      Filename: display.c                                                   */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/11/11 22:40:37 by espadara                              */
/*      Updated: 2025/11/11 22:57:37 by espadara                              */
/*                                                                            */
/* ************************************************************************** */
#include "sea_malloc.h"

static size_t show_alloc_tiny_small(t_page *mem,
                                    const char *size_str)
{
    t_block *block = mem->alloc;
    size_t total = 0;

    while (mem)
    {
        sea_printf("%s : %p\n", size_str, mem);
        while (block)
        {
            sea_printf("%p - %p : %u bytes\n", (void *)block + sizeof(t_block),
                      (void *)block + sizeof(t_block) + block->size, block->size);
            total += block->size;
            block = block->next;
        }
        mem = mem->next;
    }
    return (total);
}

static size_t show_alloc_large(void)
{
    t_block *block = g_malloc_pages.large;
    size_t total = 0;

    sea_printf("LARGE : %p\n", g_malloc_pages.large);
    while (block)
    {
        sea_printf("%p - %p : %u bytes\n", (void *)block + sizeof(t_block), (void *)block + sizeof(t_block) + block->size, block->size);
        total += block->size;
        block = block->next;
    }
    return (total);
}

static size_t dump_line(const char *ptr, const size_t size)
{
    size_t i = 0;

    sea_printf("%.16p: ", ptr);
    while (i < HEXA_LIMIT)
    {
        if (!(i & 1))
            sea_printf(" ");
        if (size <= i)
            sea_printf("  ");
        else
            sea_printf("%02x", (unsigned char)ptr[i]);
        i++;
    }
    sea_printf("  ");
    i = 0;
    while (i < HEXA_LIMIT && i < size)
    {
        sea_isprint(ptr[i]) ? sea_printf("%c", ptr[i]) : sea_printf(".");
        i++;
    }
    return (i);
}

void show_alloc_mem_ex(void *ptr)
{
    const char *zones[3] = {"Tiny", "Small", "Large"};
    size_t size;
    size_t i;

    pthread_mutex_lock(&g_malloc_mutex);
    if (ptr)
    {
        size = ((t_block *)(ptr - sizeof(t_block)))->size;
        sea_printf("%s memory area of %u bytes starting at %p:\n",
                  zones[page_size(size)], size, ptr);
        while (size)
        {
            i = dump_line(ptr, size);
            size -= i;
            ptr += i;
            sea_printf("\n");
        }
    }
    else
        sea_printf("Memory address [%p] was not allocated by malloc\n", ptr);
    pthread_mutex_unlock(&g_malloc_mutex);
}

void show_alloc_mem(void)
{
    size_t total = 0;

    pthread_mutex_lock(&g_malloc_mutex);
    if (g_malloc_pages.tiny)
        total += show_alloc_tiny_small(g_malloc_pages.tiny, "TINY");
    if (g_malloc_pages.small)
        total += show_alloc_tiny_small(g_malloc_pages.small, "SMALL");
    if (g_malloc_pages.large)
        total += show_alloc_large();
    sea_printf("Total : %u bytes\n", total);
    pthread_mutex_unlock(&g_malloc_mutex);
}
