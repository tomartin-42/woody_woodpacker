/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/24 12:14:52 by tomartin          #+#    #+#             */
/*   Updated: 2021/05/26 12:47:53 by tomartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
static int ft_checking(char a, char const *set) {
  while (*set) {
    if (a == *set)
      return (1);
    set++;
  }
  return (0);
}

char *ft_strtrim(char const *s1, char const *set) {
  int i;
  int j;
  int k;
  char *p;

  i = 0;
  k = 0;
  if (!s1)
    return (NULL);
  j = ft_strlen(s1);
  j--;
  while (ft_checking(s1[i], set) && *s1)
    i++;
  while (ft_checking(s1[j], set) && j >= i)
    j--;
  p = malloc((j - i) + 2);
  if (!p) {
    return (NULL);
  }
  while (i <= j)
    p[k++] = s1[i++];
  p[k] = '\0';
  return (p);
}
