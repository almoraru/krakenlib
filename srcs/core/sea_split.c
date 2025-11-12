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
/*      Filename: sea_split.c                                                 */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/09/01 21:58:43 by espadara                              */
/*      Updated: 2025/09/03 22:14:23 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_core.h"

static size_t	count_words(char const *s, char c)
{
  size_t count = 0;
  int in_word = 0;

  while (*s)
    {
      if (*s != c && in_word == 0)
        {
          in_word = 1;
          count++;
          }
      else if (*s == c)
        in_word = 0;
      s++;
    }
  return (count);
}

static void	free_split(char **split_array)
{
  size_t i = 0;
  while (split_array[i])
    {
      free(split_array[i]);
      i++;
    }
  free(split_array);
}

char	**sea_split(char const *s, char c)
{
  char **result = NULL;
  size_t word_len = 0;
  size_t i = 0;

  if (!s)
    return (NULL);
  result = malloc((count_words(s, c) + 1) * sizeof(char *));
  if (!result)
    return (NULL);
  while (*s)
    {
      while (*s && *s == c)
        s++;
      if (*s)
        {
          word_len = 0;
          while (s[word_len] && s[word_len] != c)
            word_len++;
          result[i] = malloc(word_len + 1);
          if (!result[i])
            {
              free_split(result);
              return (NULL);
            }
          sea_memcpy_fast(result[i], s, word_len);
          result[i++][word_len] = 0;
          s += word_len;
        }
    }
  result[i] = NULL;
  return (result);
}

char	**sea_arena_split(t_mem *arena, char const *s, char c)
{
  char **result = NULL;
  char *str_data = NULL;
  size_t word_count = 0;
  size_t total_len = 0;
  size_t i = 0;

  if (!arena || !s)
    return (NULL);
  while (s[i])
    {
      while (s[i] && s[i] == c)
        i++;
      if (s[i])
        {
          word_count++;
          while (s[i] && s[i] != c)
            {
              total_len++;
              i++;
            }
        }
    }
  result = sea_arena_alloc(arena, (word_count + 1) * sizeof(char *) + total_len + word_count);
  if (!result)
    return (NULL);
  str_data = (char *)(result + word_count + 1);
  i = 0;
  while (*s)
    {
      while (*s && *s == c)
        s++;
      if (*s)
        {
          result[i++] = str_data;
          while (*s && *s != c)
            *str_data++ = *s++;
          *str_data++ = 0;
        }
    }
  result[i] = NULL;
  return (result);
}
