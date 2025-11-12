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
/*      Filename: sea_printf.h                                                */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/11/02 14:16:42 by espadara                              */
/*      Updated: 2025/11/11 16:26:40 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEA_PRINTF_H
# define SEA_PRINTF_H


/* INCLUDES */
# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>
# include <math.h>
/* SEA LIB */
# include "sea_core.h"

/* DEFINES */

# define FLAG_MINUS		1	// (1 << 0)
# define FLAG_ZERO			2	// (1 << 1)
# define FLAG_HASH			4	// (1 << 2)
# define FLAG_SPACE		8	// (1 << 3)
# define FLAG_PLUS			16	// (1 << 4)
# define FLAG_HAS_PRECISION 32	// (1 << 5)
# define PAGE               4096

/* STRUCTURES */
typedef struct s_flags
{
	int	bits;
	int	width;
	int	precision;
}	t_flags;

typedef struct s_sea_state
{
	char buffer[PAGE];
	char conversion[PAGE];
	size_t buf_pos;
	size_t conv_pos;
	va_list	args;
	t_flags	flags;
	int		total_len;
}	t_sea_state;

/* PROTOTYPES  */

/* --- Main Functions --- */

int	sea_printf(const char *format, ...);
void	sea_parse_conversion(const char **format, t_sea_state *state);

/* --- Flag & Bonus Parsers (sea_printf_bonus.c) --- */

void	sea_parse_flags(const char **format, t_sea_state *state);
void	sea_apply_padding(t_sea_state *state, char *str, int len, int is_char);
void	sea_handle_width(t_sea_state *state, int len, int is_zero_padded);
void	sea_handle_precision(t_sea_state *state, char **str, int *len, int is_neg);

/* --- Conversion Handlers (sea_printf_handlers.c) --- */

void	sea_handle_char(t_sea_state *state);
void	sea_handle_string(t_sea_state *state);
void	sea_handle_pointer(t_sea_state *state);
void	sea_handle_int(t_sea_state *state);
void	sea_handle_unsigned(t_sea_state *state);
void	sea_handle_hex(t_sea_state *state, int is_upper);
void	sea_handle_percent(t_sea_state *state);
void	sea_handle_float(t_sea_state *state);

/* --- Buffer Functions (sea_printf_buffer.c) --- */

void    sea_state_init(t_sea_state *state);
void    sea_state_flush(t_sea_state *state);
void    sea_putchar_buf(t_sea_state *state, char c);
void    sea_putstr_buf(t_sea_state *state, const char *s, size_t len);
char    *sea_itoa_buf(t_sea_state *state, long long n);
char    *sea_utoa_base_buf(t_sea_state *state, unsigned long long n,
            char *base_chars);
char	*sea_ftoa_buf(t_sea_state *state, double d, int *out_len);

#endif
