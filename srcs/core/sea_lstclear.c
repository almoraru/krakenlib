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
/*      Filename: sea_lstclear.c                                              */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/09/04 22:08:33 by espadara                              */
/*      Updated: 2025/09/04 22:33:17 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

void	sea_lstclear(t_list **lst, void (*del)(void*))
{
  if (!lst || !*lst)
    return ;
  t_list *list = *lst;
  while (list)
    {
      t_list *next_node = list->next;
      sea_lstdelone(list, del);
      list = next_node;
    }
  *lst = NULL;
}
