/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 08:49:56 by tomartin          #+#    #+#             */
/*   Updated: 2021/05/21 09:14:03 by tomartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dest, const void *src, int c, size_t n)
{
	char	*s;
	char	*d;
	size_t	i;

	i = 0;
	s = (char *) src;
	d = (char *) dest;
	while (i < n)
	{
		*d = *s;
		if (*(unsigned char *)s == (unsigned char) c)
			return ((void *)d + 1);
		d++;
		s++;
		i++;
	}
	if (i == n)
		return (NULL);
	else
		return ((void *)d + 1);
}
