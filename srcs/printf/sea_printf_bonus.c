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
/*      Filename: sea_printf_bonus.c                                          */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/11/02 15:27:49 by espadara                              */
/*      Updated: 2025/11/11 16:46:13 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_printf.h"

static int	sea_printf_atoi(const char **format)
{
  int	num;

  num = 0;
  while (sea_isdigit(**format))
    {
      num = num * 10 + (**format - '0');
      (*format)++;
	}
  return (num);
}

void	sea_parse_flags(const char **format, t_sea_state *state)
{
  while (**format)
	{
      if (**format == '-')
        state->flags.bits |= FLAG_MINUS;
      else if (**format == '0')
        state->flags.bits |= FLAG_ZERO;
      else if (**format == '#')
        state->flags.bits |= FLAG_HASH;
      else if (**format == ' ')
        state->flags.bits |= FLAG_SPACE;
      else if (**format == '+')
        state->flags.bits |= FLAG_PLUS;
      else
        break ;
      (*format)++;
	}
  if (state->flags.bits & FLAG_MINUS)
    state->flags.bits &= ~FLAG_ZERO;
  if (sea_isdigit(**format))
    state->flags.width = sea_printf_atoi(format);
  if (**format == '.')
	{
      state->flags.bits |= FLAG_HAS_PRECISION;
      (*format)++;
      state->flags.precision = sea_printf_atoi(format);
	}
}

void	sea_handle_width(t_sea_state *state, int len, int is_zero_padded)
{
  char	pad_char;
  int		i;

  if (state->flags.width <= len)
      return ;
  if (is_zero_padded && (state->flags.bits & FLAG_ZERO)
        && !(state->flags.bits & FLAG_MINUS))
      pad_char = '0';
  else
      pad_char = ' ';
  i = 0;
  while (i < state->flags.width - len)
      {
        sea_putchar_buf(state, pad_char);
        i++;
      }
}

void sea_handle_precision(t_sea_state *state, char **str, int *len, int is_neg)
{
    int new_len;
    int zeros_needed;
    int i;
    int src_offset;

    if (!(state->flags.bits & FLAG_HAS_PRECISION)
        || state->flags.precision < *len)
        return;

    new_len = state->flags.precision;
    if (is_neg)
        new_len++;
    zeros_needed = new_len - *len;

    char *temp = state->conversion + 2048;  // Use second half as temp
    char *p = temp;
    i = 0;
    src_offset = 0;

    if (is_neg)
    {
        *p++ = '-';
        src_offset++;
        (*len)--;
    }
    while (i < zeros_needed)
    {
        *p++ = '0';
        i++;
    }
    sea_memcpy_fast(p, *str + src_offset, *len);
    p += *len;
    *p = '\0';
    sea_memcpy_fast(state->conversion, temp, new_len);
    state->conversion[new_len] = '\0';

    *str = state->conversion;
    *len = new_len;
}
