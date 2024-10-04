/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 13:03:39 by tomartin          #+#    #+#             */
/*   Updated: 2021/05/26 13:03:44 by tomartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear (t_list **lst, void (*del)(void *))
{
	t_list	*l;

	if (lst)
	{
		while (*lst)
		{
			l = (*lst)->next;
			ft_lstdelone(*lst, del);
			(*lst) = l;
		}
	}	
}
