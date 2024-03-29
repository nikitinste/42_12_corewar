/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_o.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggerhold <ggerhold@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/30 16:26:56 by ggerhold          #+#    #+#             */
/*   Updated: 2019/10/30 16:26:59 by ggerhold         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_lib.h"

static void	update_placeholder(t_placeholder *ph, uintmax_t num)
{
	size_t	numlen;
	size_t	prefixlen;

	ph->flags &= ~SPACE;
	ph->flags &= ~PLUS;
	ph->flags &= ~APOSTROPHE;
	if (ph->flags & MINUS || ph->precision != -1)
		ph->flags &= ~ZERO;
	if ((ph->flags & HASH) && !num && !ph->precision)
		ph->precision = 1;
	if (!num)
		ph->flags &= ~HASH;
	numlen = 0;
	if (num || ph->precision)
		numlen = ft_unsignedlen(num, ph->base);
	prefixlen = 0;
	if (ph->flags & HASH)
		prefixlen = ft_strlen(get_prefix(ph->base, ph->flags));
	ph->precision = FT_MAX(ph->precision, (int)(numlen + prefixlen));
	ph->width = FT_MAX(ph->width, ph->precision);
	ph->padding = ph->width - ph->precision;
}

char		*handle_o(t_placeholder *ph, va_list args)
{
	uintmax_t	num;
	char		*str;

	update_placeholder(ph, (num = get_unsigned(ph, args)));
	if (!(str = ft_strnew(ph->width)))
		exit(1);
	if (!(ph->flags & MINUS) && !(ph->flags & ZERO))
		while (ph->padding > 0 && ph->padding--)
			*str++ = ' ';
	if (ph->flags & HASH)
	{
		strcat_ptr(&str, get_prefix(ph->base, ph->flags));
		ph->precision -= ft_strlen(get_prefix(ph->base, ph->flags));
	}
	if (ph->precision)
		strcatunbr_ptr(ph, num, &str);
	if (ph->flags & MINUS)
		while (ph->padding > 0 && ph->padding--)
			*str++ = ' ';
	return (str - ph->width);
}
