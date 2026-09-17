/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/23 19:02:23 by tomartin          #+#    #+#             */
/*   Updated: 2021/05/26 12:52:08 by tomartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (*s)
	{
		i++;
		s++;
	}
	while (*s != (char)c)
	{
		if (i == 0)
			return (NULL);
		s--;
		i--;
	}
	if (*s == (char)c)
		return ((char *)s);
	return (NULL);
}
