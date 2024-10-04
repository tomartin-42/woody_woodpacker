/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 11:01:05 by tomartin          #+#    #+#             */
/*   Updated: 2021/05/27 12:35:16 by tomartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*p;
	size_t	i;
	size_t	j;

	i = 0;
	if (!s)
		return (NULL);
	j = ft_strlen(s);
	if (j < len)
		p = ft_calloc (sizeof (char), (j + 1));
	else
		p = ft_calloc (sizeof (char), (len + 1));
	if (!p)
		return (NULL);
	if (j >= start)
	{
		while (i < len && s[start])
			p[i++] = s[start++];
		p[i] = '\0';
	}
	return (p);
}
