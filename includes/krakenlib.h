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
/*      Filename: krakenlib.h                                                 */
/*      By: almoraru <almoraru@student.42.fr>                                */
/*      Created: 2025-11-12 22:42:34 by almoraru                             */
/*      Updated: 2025-11-12 22:42:34 by almoraru                             */
/*                                                                            */
/* ************************************************************************** */

#ifndef KRAKENLIB_H
# define KRAKENLIB_H

/* Base System Includes */
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>
# include <fcntl.h>
# include <immintrin.h>
# include <pthread.h>
# include <sys/mman.h>
# include <limits.h>
# include <math.h>
# include <stdbool.h>
# include <ctype.h>
# include <string.h>
# include <bsd/string.h>

/* Version Info */
# define KRAKENLIB_VERSION "1.0.0"
# define KRAKENLIB_MAJOR 1
# define KRAKENLIB_MINOR 0
# define KRAKENLIB_PATCH 0

/* Module Headers */
# include "sea_core.h"      // Core utilities (strings, memory, lists)
# include "sea_printf.h"    // Formatted output
# include "sea_gnl.h"       // Get Next Line
# include "sea_malloc.h"    // Memory allocation

/*
** ============================================================================
**                         🐙 THE KRAKEN AWAKENS 🐙
** ============================================================================
**
** Krakenlib is a unified C library containing all the sea libraries:
**
**   ⚓ Core Utilities  - String manipulation, memory operations, lists
**   🖨️  Printf          - Formatted output without malloc
**   📖 Get Next Line   - Efficient file reading
**   💾 Memory Manager  - Custom malloc/free implementation
**
** All functions keep their original sea_* prefix for compatibility.
** Import everything with: #include <krakenlib.h>
**
** ============================================================================
*/

#endif
