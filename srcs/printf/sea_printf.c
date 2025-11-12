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
/*      Filename: sea_printf.c                                                */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/11/02 14:18:18 by espadara                              */
/*      Updated: 2025/11/11 16:53:15 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_printf.h"

void	sea_parse_conversion(const char **format, t_sea_state *state)
{
  char	c = **format;

  if (c == 'c')
    sea_handle_char(state);
  else if (c == 's')
    sea_handle_string(state);
  else if (c == 'p')
    sea_handle_pointer(state);
  else if (c == 'd' || c == 'i')
    sea_handle_int(state);
  else if (c == 'u')
    sea_handle_unsigned(state);
  else if (c == 'x')
    sea_handle_hex(state, 0);
  else if (c == 'X')
    sea_handle_hex(state, 1);
  else if (c == '%')
    sea_handle_percent(state);
  else if (c == 'f')
    sea_handle_float(state);
  else
    sea_putchar_buf(state, c);
  (*format)++;
}

int	sea_printf(const char *format, ...){
  t_sea_state state;

  sea_bzero(&state, sizeof(state));
  va_start(state.args, format);
  while (*format)
    {
      if (*format == '%')
        {
          format++;
          sea_memset(&state.flags, 0, sizeof(t_flags));
          sea_parse_flags(&format, &state);
          sea_parse_conversion(&format, &state);
        } else {
        sea_putchar_buf(&state, *format);
        format++;
      }
    }
  sea_state_flush(&state);
  va_end(state.args);
  return (state.total_len);
}
