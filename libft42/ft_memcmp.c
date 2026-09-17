/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 08:46:26 by tomartin          #+#    #+#             */
/*   Updated: 2021/05/25 13:04:30 by tomartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*p1;
	unsigned char	*p2;
	int				check;

	check = 0;
	p1 = (unsigned char *) s1;
	p2 = (unsigned char *) s2;
	while (n != 0 && check == 0)
	{
		check = *p1 - *p2;
		p1++;
		p2++;
		n--;
	}
	return (check);
}
