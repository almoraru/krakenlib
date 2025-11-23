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
/*      Updated: 2025/11/23 16:52:00 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_malloc.h"

static size_t dump_line(const char *ptr, const size_t size)
{
    size_t i = 0;

    sea_printf("%p: ", ptr);
    while (i < 16)
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
    while (i < 16 && i < size)
    {
        char c = ptr[i];
        if (c >= 32 && c <= 126)
            sea_printf("%c", c);
        else
            sea_printf(".");
        i++;
    }
    return (i);
}

static void print_slab(t_slab *slab, const char *zone_name, size_t *total)
{
    int         i, j;
    uint64_t    map;
    void        *addr;
    void        *end;

    sea_printf("%s : %p\n", zone_name, slab);
    for (i = 0; i < 16; i++)
    {
        map = slab->bitmap[i];

        if (map == 0)
            continue;

        for (j = 0; j < 64; j++)
        {
            if ((map >> j) & 1)
                {
                    addr = (char *)slab + sizeof(t_slab) +
                       ((i * 64 + j) * slab->block_size);

                    end = (char *)addr + slab->block_size;
                    sea_printf("%p - %p : %u bytes\n", addr, end, slab->block_size);
                    *total += slab->block_size;
            }
        }
    }
}

static void print_large(t_slab *slab, size_t *total)
{
    void *addr;
    void *end;

    sea_printf("LARGE : %p\n", slab);
    addr = (void *)(slab + 1);
    end = (char *)addr + slab->block_size;

    sea_printf("%p - %p : %u bytes\n", addr, end, slab->block_size);
    *total += slab->block_size;
}

__attribute__((visibility("default")))
void show_alloc_mem(void)
{
    size_t  total = 0;
    int     i;
    t_slab  *slab;

    pthread_mutex_lock(&g_malloc_mutex);
    for (i = 0; i < MAX_TINY_CLASSES; i++)
        {
            slab = g_heap.tiny[i];
            while (slab)
                {
                    print_slab(slab, "TINY", &total);
                    slab = slab->next;
                }
        }
    for (i = 0; i < MAX_SMALL_CLASSES; i++)
        {
            slab = g_heap.small[i];
            while (slab)
                {
                    print_slab(slab, "SMALL", &total);
                    slab = slab->next;
                }
        }
    slab = g_heap.large;
    while (slab)
        {
            print_large(slab, &total);
            slab = slab->next;
        }

    sea_printf("Total : %u bytes\n", total);
    pthread_mutex_unlock(&g_malloc_mutex);
}

__attribute__((visibility("default")))
void show_alloc_mem_ex(void *ptr)
{
    t_slab  *slab;
    int     type;
    size_t  size;
    size_t  i;
    size_t  dump_size;

    if (!ptr)
        return;

    pthread_mutex_lock(&g_malloc_mutex);

    slab = find_slab_by_ptr(ptr, &type);

    if (slab)
    {
        size = slab->block_size;
        sea_printf("Memory area of %u bytes starting at %p:\n", size, ptr);

        i = 0;
        while (size)
        {
            dump_size = size;
            size_t printed = dump_line((char *)ptr + i, dump_size);
            size -= printed;
            i += printed;
            sea_printf("\n");
            if (i > 1024 && type == 2)
            {
                sea_printf("... (truncated)\n");
                break;
            }
        }
    }
    else
        {
            sea_printf("Memory address [%p] was not allocated by sea_malloc\n", ptr);
        }
    pthread_mutex_unlock(&g_malloc_mutex);
}
