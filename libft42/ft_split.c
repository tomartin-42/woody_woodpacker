/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/25 09:02:22 by tomartin          #+#    #+#             */
/*   Updated: 2021/05/29 17:57:30 by tomartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_tableword(const char *s, char c)
{
	int	i;

	i = 0;
	if (!*s)
		return (0);
	if (*s != c && *(s + 1))
		i = 1;
	while (*(s + 1))
	{
		if (*s == c && *(s + 1) != c && *(s + 1) != '\0')
			i++;
		s++;
	}
	return (i);
}

static void	ft_insert_table(char **table, char const *s, char c, int words)
{
	int	i;
	int	j;
	int	k;
	int	w;

	w = 0;
	i = 0;
	while (words-- > 0)
	{
		while (s[i] == c && s[i])
			i++;
		j = i;
		while (s[i] != c && s[i])
			i++;
		table[w] = (char *)malloc(sizeof(char) * (i - j + 1));
		k = 0;
		while (j < i)
			table[w][k++] = s[j++];
		table[w][k] = '\0';
		w++;
	}
	table[w] = NULL;
}

char	**ft_split(char const *s, char c)
{
	char			**table;
	unsigned int	words;

	if (!s)
		return (NULL);
	words = ft_tableword(s, c);
	table = (char **)malloc(sizeof(char *) * (words + 1));
	if (!table)
		return (NULL);
	ft_insert_table(table, s, c, words);
	if (!table)
		return (NULL);
	return (table);
}
