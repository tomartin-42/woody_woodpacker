/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/24 09:05:17 by tomartin          #+#    #+#             */
/*   Updated: 2021/05/27 11:03:40 by tomartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	int	res;

	res = 0;
	while ((*s1 || *s2) && (n > 0))
	{
		res = (unsigned char)*s1 - (unsigned char)*s2;
		if (res != 0)
			return (res);
		s1++;
		s2++;
		n--;
	}
	return (res);
}
