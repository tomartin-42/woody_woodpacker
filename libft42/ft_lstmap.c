/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 18:55:04 by tomartin          #+#    #+#             */
/*   Updated: 2021/05/30 18:38:07 by tomartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*l;
	t_list	*l_aux;

	if (!f || !lst)
		return (NULL);
	l = NULL;
	while (lst)
	{
		l_aux = ft_lstnew(f(lst->content));
		if (!l_aux)
			ft_lstclear(&l_aux, del);
		ft_lstadd_back(&l, l_aux);
		lst = lst->next;
	}
	return (l);
}
