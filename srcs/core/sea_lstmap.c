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
/*      Filename: sea_lstmap.c                                                */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/10/26 23:48:14 by espadara                              */
/*      Updated: 2025/10/27 00:05:02 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

/* --- ARENA-SAFE HELPER --- */

/*
 * Helper function to safely clear the content of the new list
 * in case of an allocation failure.
 *
 * NOTICE: It does NOT free(lst) because the node (lst) itself
 * was allocated in the arena and cannot be freed individually.
 * It only calls del() on the content.
 */
static void sea_lstclear_partial_arena(t_list *lst, void (*del)(void *))
{
    t_list *temp_node;

    while (lst)
    {
        temp_node = lst->next;
        del(lst->content);
        // No free(lst) here!
        lst = temp_node;
    }
}

static void sea_lstclear_partial(t_list *lst, void (*del)(void *))
{
    t_list *temp_node;

    if (!del)
        return;

    while (lst)
    {
        temp_node = lst->next;
        del(lst->content);
        free(lst);
        lst = temp_node;
    }
}

t_list	*sea_lstmap(t_list *lst, void *(*f)(void*), void (*del)(void*))
{
  t_list  *new_head;
  t_list  *new_tail;
  t_list  *new_node;
  void    *new_content;

    if (!f || !del)
        return (NULL);

    new_head = NULL;
    new_tail = NULL;
    while (lst)
    {
        new_content = f(lst->content);
        new_node = (t_list *)malloc(sizeof(t_list));
        if (!new_node)
        {
            del(new_content);
            sea_lstclear_partial(new_head, del);
            return (NULL);
        }
        new_node->content = new_content;
        new_node->next = NULL;
        if (new_head == NULL)
        {
            new_head = new_node;
            new_tail = new_node;
        }
        else
        {
            new_tail->next = new_node;
            new_tail = new_node;
        }
        lst = lst->next;
    }
    return (new_head);
}

t_list  *sea_arena_lstmap(t_mem *arena, t_list *lst, void *(*f)(void *), void (*del)(void *))
{
  t_list  *new_head;
  t_list  *new_tail;
  t_list  *new_node;
  void    *new_content;

  if (!arena || !f || !del)
    return (NULL);

  new_head = NULL;
  new_tail = NULL;
    while (lst)
      {
        new_content = f(lst->content);
        new_node = (t_list *)sea_arena_alloc(arena, sizeof(t_list));
        if (!new_node)
          {
            del(new_content);
            sea_lstclear_partial_arena(new_head, del);
            return (NULL);
          }
        new_node->content = new_content;
        new_node->next = NULL;
        if (new_head == NULL)
        {
          new_head = new_node;
          new_tail = new_node;
        }
        else
          {
            new_tail->next = new_node;
            new_tail = new_node;
          }
        lst = lst->next;
    }
    return (new_head);
}
