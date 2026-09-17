/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 08:45:25 by tomartin          #+#    #+#             */
/*   Updated: 2021/05/25 13:02:54 by tomartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*s;
	unsigned char	*d;

	if (!dest && !src && n > 0)
		return (NULL);
	s = (unsigned char *) src;
	d = (unsigned char *) dest;
	if (s < d)
	{
		while (n--)
			d[n] = s[n];
	}
	else if (s > d)
		ft_memcpy(d, s, n);
	return ((void *)dest);
}
