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
/*      Filename: sea_printf_handlers.c                                       */
/*      By: espadara <espadara@pirate.capn.gg>                                */
/*      Created: 2025/11/02 15:13:25 by espadara                              */
/*      Updated: 2025/11/11 17:01:20 by espadara                              */
/*                                                                            */
/* ************************************************************************** */
#include "sea_printf.h"

void	sea_handle_char(t_sea_state *state)
{
  char	c;

  c = va_arg(state->args, int);
  if (!(state->flags.bits & FLAG_MINUS))
    sea_handle_width(state, 1, 0);
  sea_putchar_buf(state, c);
  if (state->flags.bits & FLAG_MINUS)
    sea_handle_width(state, 1, 0);
}

void	sea_handle_string(t_sea_state *state)
{
  char	*s;
  int		len;

    s = va_arg(state->args, char *);
    if (s == NULL)
      s = "(null)";
    len = sea_strlen(s);
    if ((state->flags.bits & FLAG_HAS_PRECISION)
        && state->flags.precision < len)
      len = state->flags.precision;
    if (!(state->flags.bits & FLAG_MINUS))
      sea_handle_width(state, len, 0);
    sea_putstr_buf(state, s, len);
    if (state->flags.bits & FLAG_MINUS)
      sea_handle_width(state, len, 0);
}

void	sea_handle_pointer(t_sea_state *state)
{
  unsigned long long	p;
  char				*s;
  int					len;

  p = (unsigned long long)va_arg(state->args, void *);
  if (p == 0)
	{
      s = "(nil)";
      len = 5;
      if ((state->flags.bits & FLAG_HAS_PRECISION)
          && state->flags.precision == 0)
        len = 0;
      if (!(state->flags.bits & FLAG_MINUS))
        sea_handle_width(state, len, 0);
      sea_putstr_buf(state, s, len);
      if (state->flags.bits & FLAG_MINUS)
        sea_handle_width(state, len, 0);
      return ;
	}
  s = sea_utoa_base_buf(state, p, "0123456789abcdef");
  len = sea_strlen(s) + 2;
  if (!(state->flags.bits & FLAG_MINUS))
      sea_handle_width(state, len, 0);
  sea_putstr_buf(state, "0x", 2);
  sea_putstr_buf(state, s, sea_strlen(s));
  if (state->flags.bits & FLAG_MINUS)
      sea_handle_width(state, len, 0);
}

void	sea_handle_int(t_sea_state *state)
{
	long long	n;
	char		*s;
	int			len;
	int			is_neg;
	int			prefix_len;
	char		prefix[2];
	int			is_zero_padded;

	n = va_arg(state->args, int);
	is_neg = (n < 0);
	s = sea_itoa_buf(state, n);
	len = sea_strlen(s);
	if (n == 0 && (state->flags.bits & FLAG_HAS_PRECISION)
		&& state->flags.precision == 0)
		len = 0;
	sea_handle_precision(state, &s, &len, is_neg);
	is_neg = (s[0] == '-');
	prefix_len = 0;
	if (is_neg)
	{
		prefix[0] = '-';
		prefix_len = 1;
		s++;
		len--;
	}
	else if (state->flags.bits & FLAG_PLUS)
		prefix[prefix_len++] = '+';
	else if (state->flags.bits & FLAG_SPACE)
		prefix[prefix_len++] = ' ';

	is_zero_padded = (state->flags.bits & FLAG_ZERO)
		&& !(state->flags.bits & FLAG_MINUS)
		&& !(state->flags.bits & FLAG_HAS_PRECISION);

	if (!(state->flags.bits & FLAG_MINUS))
	{
		if (is_zero_padded)
		{
			sea_putstr_buf(state, prefix, prefix_len);
			sea_handle_width(state, len + prefix_len, 1);
		}
		else
		{
			sea_handle_width(state, len + prefix_len, 0);
            sea_putstr_buf(state, prefix, prefix_len);
		}
	}
	else
		sea_putstr_buf(state, prefix, prefix_len);
	sea_putstr_buf(state, s, len);
	if (state->flags.bits & FLAG_MINUS)
		sea_handle_width(state, len + prefix_len, 0);
}

void	sea_handle_unsigned(t_sea_state *state)
{
	unsigned int	n;
	char			*s;
	int				len;
	int				is_zero_padded;

	n = va_arg(state->args, unsigned int);
	s = sea_utoa_base_buf(state, n, "0123456789");
	len = sea_strlen(s);
	if (n == 0 && (state->flags.bits & FLAG_HAS_PRECISION)
		&& state->flags.precision == 0)
		len = 0;
	sea_handle_precision(state, &s, &len, 0);

	is_zero_padded = (state->flags.bits & FLAG_ZERO)
		&& !(state->flags.bits & FLAG_MINUS)
		&& !(state->flags.bits & FLAG_HAS_PRECISION);

	if (!(state->flags.bits & FLAG_MINUS))
		sea_handle_width(state, len, is_zero_padded);
	sea_putstr_buf(state, s, len);
	if (state->flags.bits & FLAG_MINUS)
		sea_handle_width(state, len, 0);
}

void	sea_handle_hex(t_sea_state *state, int is_upper)
{
	unsigned int	n;
	char			*s;
	char			*base;
	int				len;
	int				prefix_len;
	int				is_zero_padded;

	n = va_arg(state->args, unsigned int);
	if (is_upper)
		base = "0123456789ABCDEF";
	else
		base = "0123456789abcdef";
	s = sea_utoa_base_buf(state, n, base);
	len = sea_strlen(s);
	if (n == 0 && (state->flags.bits & FLAG_HAS_PRECISION)
		&& state->flags.precision == 0)
		len = 0;
	if (n == 0)
		state->flags.bits &= ~FLAG_HASH;
	sea_handle_precision(state, &s, &len, 0);
	prefix_len = (state->flags.bits & FLAG_HASH) ? 2 : 0;

	is_zero_padded = (state->flags.bits & FLAG_ZERO)
		&& !(state->flags.bits & FLAG_MINUS)
		&& !(state->flags.bits & FLAG_HAS_PRECISION);

	if (!(state->flags.bits & FLAG_MINUS))
	{
		if (is_zero_padded)
		{
			if (prefix_len)
				sea_putstr_buf(state, is_upper ? "0X" : "0x", 2);
			sea_handle_width(state, len + prefix_len, 1);
		}
		else
		{
			sea_handle_width(state, len + prefix_len, 0);
			if (prefix_len)
				sea_putstr_buf(state, is_upper ? "0X" : "0x", 2);
		}
	}
	else if (prefix_len)
		sea_putstr_buf(state, is_upper ? "0X" : "0x", 2);
	sea_putstr_buf(state, s, len);
	if (state->flags.bits & FLAG_MINUS)
		sea_handle_width(state, len + prefix_len, 0);
}

void	sea_handle_percent(t_sea_state *state)
{
	if (!(state->flags.bits & FLAG_MINUS))
		sea_handle_width(state, 1, 0);
	sea_putchar_buf(state, '%');
	if (state->flags.bits & FLAG_MINUS)
		sea_handle_width(state, 1, 0);
}

void	sea_handle_float(t_sea_state *state)
{
	double	d;
	char	*s;
	int		len;
	int		is_neg;
	int		prefix_len;
	char	prefix[2];

	d = va_arg(state->args, double);
	is_neg = (d < 0.0);
	if (is_neg)
		d = -d;
	s = sea_ftoa_buf(state, d, &len);
	prefix_len = 0;
	if (is_neg)
		prefix[prefix_len++] = '-';
	else if (state->flags.bits & FLAG_PLUS)
		prefix[prefix_len++] = '+';
	else if (state->flags.bits & FLAG_SPACE)
		prefix[prefix_len++] = ' ';
	if (!(state->flags.bits & FLAG_MINUS))
	{
		if (state->flags.bits & FLAG_ZERO)
		{
            sea_putstr_buf(state, prefix, prefix_len);
			sea_handle_width(state, len + prefix_len, 1);
		}
		else
		{
			sea_handle_width(state, len + prefix_len, 0);
			sea_putstr_buf(state, prefix, prefix_len);
		}
	}
	else
		sea_putstr_buf(state, prefix, prefix_len);
	sea_putstr_buf(state, s, len);
	if (state->flags.bits & FLAG_MINUS)
		sea_handle_width(state, len + prefix_len, 0);
}
