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
/*      Filename: errors.c                                                    */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/11/23 16:52:41 by espadara                              */
/*      Updated: 2025/11/23 16:52:50 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_malloc.h"

bool error_malloc(const int err, const char *str)
{
  static const char *msg[3] = {
    "  -> in _",
    "Invalid block: ",
    "mmap MAP_FAILED error: "};
  sea_printf("%s%s\n", msg[err], str);
  return (false);
}
