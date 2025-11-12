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
/*      Filename: sea_lstnew.c                                                */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/09/02 22:34:34 by espadara                              */
/*      Updated: 2025/09/03 13:41:08 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

t_list	*sea_lstnew(void *content)
{
  t_list *new_node = NULL;

  new_node = malloc(sizeof(t_list));
  if (!new_node)
    return (NULL);
  new_node->content = content;
  new_node->next = NULL;

 return (new_node);
}

t_list	*sea_arena_lstnew(t_mem *arena, void *content)
{
  t_list *new_node = NULL;

  if (!arena)
    return (NULL);
  new_node = sea_arena_alloc(arena, sizeof(t_list));
  if (!new_node)
    return (NULL);
  new_node->content = content;
  new_node->next = NULL;
  return (new_node);
}
