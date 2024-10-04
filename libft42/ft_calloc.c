/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/23 20:50:22 by tomartin          #+#    #+#             */
/*   Updated: 2021/05/27 09:15:17 by tomartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*p;
	size_t	i;
	void	*z;

	i = nmemb * size;
	p = malloc (i);
	if (!p)
		return (NULL);
	z = p;
	while (i > 0)
	{
		*(unsigned char *) z++ = '\0';
		i--;
	}
	return (p);
}
