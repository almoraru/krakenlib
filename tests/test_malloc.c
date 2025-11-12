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
/*      Filename: test.c                                                      */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/11/11 22:41:16 by espadara                              */
/*      Updated: 2025/11/11 22:41:19 by espadara                              */
/*                                                                            */
/* ************************************************************************** */
#include "includes/malloc.h"
#include <stdio.h>

int main(int ac, char **av)
{
    (void)ac;
    (void)av;
    void *ptr[1024];

    for (int i = 0; i < 20; i++)
    {
        if ((i % 2) == 0)
            ptr[i] = malloc(600);
        else if ((i % 5) == 0)
            ptr[i] = malloc(5000);
        else if ((i % 3) == 0)
            ptr[i] = malloc(200);
        else if ((i % 8) == 0)
            ptr[i] = malloc(1000);
        else if ((i % 7) == 0)
            ptr[i] = malloc(10000000);
        else
            ptr[i] = malloc(50);
    }
    show_alloc_mem();
    sea_printf("\n------------BONUS------------\n");
    char *poof = "cocococococcococo1231232132132131ZAAAAAAAAAWAAAAAAAAAAAAAAAAAAAAAARUUUUUUUUUUUUUUUDOOOOOOOOOOOOOOO";
    int len = sea_strlen(poof);
    sea_memcpy(ptr[3], (void *)poof, len);
    show_alloc_mem_ex(ptr[3]);
    for (int i = 0; i < 20; i++)
        free(ptr[i]);

    return (0);
}
