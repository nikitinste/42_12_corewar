/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strupr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggerhold <ggerhold@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/30 16:20:18 by ggerhold          #+#    #+#             */
/*   Updated: 2019/10/30 16:20:18 by ggerhold         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strupr(char *s)
{
	size_t i;

	if (s)
	{
		i = 0;
		while (s[i])
		{
			s[i] = (char)ft_toupper(s[i]);
			i++;
		}
	}
	return (s);
}
