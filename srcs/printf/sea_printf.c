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
/*      Updated: 2025/11/30 13:07:13 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_printf.h"

static t_handler g_handlers[128] = {0};

static void init_handlers(void)
{
    if (g_handlers[(int)'d']) return;

    g_handlers['c'] = sea_handle_char;
    g_handlers['s'] = sea_handle_string;
    g_handlers['p'] = sea_handle_pointer;
    g_handlers['d'] = sea_handle_int;
    g_handlers['i'] = sea_handle_int;
    g_handlers['u'] = sea_handle_unsigned;
    g_handlers['x'] = sea_handle_hex_lower;
    g_handlers['X'] = sea_handle_hex_upper;
    g_handlers['%'] = sea_handle_percent;
    g_handlers['f'] = sea_handle_float;
}
int	sea_printf(const char *format, ...){
  t_sea_state state;

  init_handlers();
  sea_bzero(&state, sizeof(state));
  va_start(state.args, format);
  while (*format)
    {
      if (*format == '%')
        {
            format++;
            sea_memset(&state.flags, 0, sizeof(t_flags));
            sea_parse_flags(&format, &state);
            sea_parse_length(&format, &state);
            unsigned char c = (unsigned char)*format;
            if (c < 128 && g_handlers[c])
            {
                g_handlers[c](&state);
            }
            else
            {
                if (c != '\0')
                    sea_putchar_buf(&state, c);
            }

            if (*format) format++;
        } else {
        sea_putchar_buf(&state, *format);
        format++;
      }
    }
  sea_state_flush(&state);
  va_end(state.args);
  return (state.total_len);
}
