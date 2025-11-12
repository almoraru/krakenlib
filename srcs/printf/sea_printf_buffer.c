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
/*      Filename: sea_printf_buffer.c                                         */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/11/11 16:12:24 by espadara                              */
/*      Updated: 2025/11/11 16:30:15 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_printf.h"

void sea_state_init(t_sea_state *state)
{
  *state = (t_sea_state){0};
}

void sea_state_flush(t_sea_state *state)
{
  if (state->buf_pos > 0)
    {
      write(1, state->buffer, state->buf_pos);
      state->buf_pos = 0;
    }
}

void sea_putchar_buf(t_sea_state *state, char c)
{
  if (state->buf_pos >= PAGE)
    sea_state_flush(state);
  state->buffer[state->buf_pos++] = c;
  state->total_len++;
}

void sea_putstr_buf(t_sea_state *state, const char *s, size_t len)
{
  while (len > 0)
    {
      size_t available = PAGE - state->buf_pos;
      size_t to_copy = (len < available) ? len : available;
      sea_memcpy_fast(state->buffer + state->buf_pos, s, to_copy);
      state->buf_pos += to_copy;
      state->total_len += to_copy;

      if (state->buf_pos >= PAGE)
        sea_state_flush(state);

      s += to_copy;
      len -= to_copy;
    }
}

char *sea_itoa_buf(t_sea_state *state, long long n)
{
    char temp[32];  // Stack buffer for reversal
    int i = 0;
    int is_neg = 0;
    unsigned long long num;

    state->conv_pos = 0;
    if (n < 0)
    {
        is_neg = 1;
        num = -n;
    }
    else
        num = n;
    if (num == 0)
        temp[i++] = '0';
    while (num > 0)
    {
        temp[i++] = '0' + (num % 10);
        num /= 10;
    }
    if (is_neg)
        temp[i++] = '-';
    while (i > 0)
        state->conversion[state->conv_pos++] = temp[--i];
    state->conversion[state->conv_pos] = '\0';
    return state->conversion;
}

char *sea_utoa_base_buf(t_sea_state *state, unsigned long long n,
                        char *base_chars)
{
    char temp[64];  // Stack buffer for reversal
    int i = 0;
    int base_len = sea_strlen(base_chars);

    state->conv_pos = 0;
    if (n == 0)
    {
        state->conversion[state->conv_pos++] = '0';
        state->conversion[state->conv_pos] = '\0';
        return state->conversion;
    }
    while (n > 0)
    {
        temp[i++] = base_chars[n % base_len];
        n /= base_len;
    }
    while (i > 0)
        state->conversion[state->conv_pos++] = temp[--i];
    state->conversion[state->conv_pos] = '\0';
    return state->conversion;
}

char *sea_ftoa_buf(t_sea_state *state, double d, int *out_len)
{
    int prec;
    double rounder;
    long long int_part;
    double frac_part;
    char *p;
    int i;
    int is_neg;

    state->conv_pos = 0;
    prec = 6;
    if (state->flags.bits & FLAG_HAS_PRECISION)
        prec = state->flags.precision;

    if (isnan(d))
    {
        sea_memcpy_fast(state->conversion, "nan", 3);
        state->conversion[3] = '\0';
        *out_len = 3;
        return state->conversion;
    }
    if (isinf(d))
    {
        if (d < 0)
        {
            sea_memcpy_fast(state->conversion, "-inf", 4);
            state->conversion[4] = '\0';
            *out_len = 4;
        }
        else
        {
            sea_memcpy_fast(state->conversion, "inf", 3);
            state->conversion[3] = '\0';
            *out_len = 3;
        }
        return state->conversion;
    }
    is_neg = (d < 0);
    if (is_neg)
        d = -d;

    rounder = 0.5;
    i = 0;
    while (i++ < prec)
        rounder /= 10.0;
    d += rounder;
    int_part = (long long)d;
    frac_part = d - (double)int_part;

    p = state->conversion;

    if (is_neg)
        *p++ = '-';
    char temp_int[32];
    int temp_idx = 0;
    unsigned long long num = int_part;

    if (num == 0)
        temp_int[temp_idx++] = '0';
    else
    {
        while (num > 0)
        {
            temp_int[temp_idx++] = '0' + (num % 10);
            num /= 10;
        }
    }
    while (temp_idx > 0)
        *p++ = temp_int[--temp_idx];
    if (prec > 0 || (state->flags.bits & FLAG_HASH))
        *p++ = '.';

    i = 0;
    while (i++ < prec)
    {
        frac_part *= 10.0;
        *p++ = (int)frac_part + '0';
        frac_part -= (int)frac_part;
    }
    *p = '\0';
    *out_len = p - state->conversion;
    state->conv_pos = *out_len;

    return state->conversion;
}
