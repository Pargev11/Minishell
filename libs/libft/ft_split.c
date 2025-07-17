/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlchinen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:51:00 by vlchinen          #+#    #+#             */
/*   Updated: 2025/02/20 16:01:58 by vlchinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(char const *s, char c)
{
	int		sep[2];
	size_t	w_c;

	w_c = 0;
	sep[1] = 1;
	while (*s)
	{
		sep[0] = sep[1];
		if (*s == c)
			sep[1] = 1;
		else
			sep[1] = 0;
		if (!sep[1] && sep[0])
			w_c++;
		s++;
	}
	return (w_c);
}

static char	*worddup(const char *s, char c)
{
	size_t	i;
	char	*buff;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	buff = malloc(sizeof(char) * (i + 1));
	if (!buff)
		return (0);
	i = 0;
	while (s[i] && s[i] != c)
	{
		buff[i] = s[i];
		i++;
	}
	buff[i] = '\0';
	return (buff);
}

static char	**complete_free(char **arr)
{
	size_t	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	return (0);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;
	char	**backup;
	int		sep[2];

	arr = malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!arr)
		return (0);
	backup = arr;
	sep[1] = 1;
	while (*s)
	{
		sep[0] = sep[1];
		if (*s++ == c)
			sep[1] = 1;
		else
			sep[1] = 0;
		if (!sep[1] && sep[0])
		{
			*arr++ = worddup(s - 1, c);
			if (!arr[-1])
				return (complete_free(backup));
		}
	}
	*arr = 0;
	return (backup);
}
