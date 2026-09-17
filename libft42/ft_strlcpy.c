/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 08:30:20 by tomartin          #+#    #+#             */
/*   Updated: 2021/05/26 09:48:30 by tomartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	y;
	size_t	x;

	x = 0;
	if (!dest || !src)
		return (0);
	y = ft_strlen(src);
	if (size == 0)
		return (y);
	while (size > 1 && src[x])
	{
		dest[x] = src[x];
		x++;
		size--;
	}
	dest[x] = '\0';
	return (y);
}
