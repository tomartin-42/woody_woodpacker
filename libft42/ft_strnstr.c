/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/24 10:06:21 by tomartin          #+#    #+#             */
/*   Updated: 2021/05/30 19:38:11 by tomartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;

	i = ft_strlen(needle);
	if (!*needle)
		return ((char *) haystack);
	while (*haystack && len-- > 0)
	{
		if (*needle == *haystack)
		{
			if (ft_strncmp (needle, haystack, i) == 0)
			{
				if (len + 1 >= i)
					return ((char *)haystack);
			}
		}
		haystack++;
	}
	return (NULL);
}
