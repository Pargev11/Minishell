/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlchinen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 11:35:02 by vlchinen          #+#    #+#             */
/*   Updated: 2025/02/20 11:39:15 by vlchinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static ssize_t	l(char const *s1, char const *set)
{
	size_t	i;

	i = 0;
	while (s1[i])
	{
		if (!ft_strchr(set, s1[i]))
			return (i);
		i++;
	}
	return (-1);
}

static ssize_t	r(char const *s1, char const *set, size_t i)
{
	i--;
	while ((long)i >= 0)
	{
		if (!ft_strchr(set, s1[i]))
			return (i);
		i--;
	}
	return (-1);
}

static size_t	trimmed_len(size_t b1, size_t b2, char const *s1)
{
	if (b1 == b2 && !*s1)
		return (0);
	else
		return (b2 - b1 + 1);
}

char	*trim(ssize_t b1, ssize_t b2, char const *s1, char *str)
{
	ssize_t	i;
	ssize_t	j;

	i = 0;
	j = 0;
	while (s1[i])
	{
		if (i >= b1 && i <= b2)
			str[j++] = s1[i];
		i++;
	}
	str[j] = '\0';
	return (str);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	size_t	str_size;
	ssize_t	b1;
	ssize_t	b2;

	b1 = l(s1, set);
	b2 = r(s1, set, ft_strlen(s1));
	if (b1 < 0 || b2 < 0)
	{
		str = malloc(sizeof(char));
		if (str)
			*str = '\0';
		return (str);
	}
	str_size = trimmed_len(b1, b2, s1);
	str = malloc(sizeof(char) * (str_size + 1));
	if (!str)
		return (0);
	return (trim(b1, b2, s1, str));
}
