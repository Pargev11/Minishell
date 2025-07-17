/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlchinen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:42:52 by vlchinen          #+#    #+#             */
/*   Updated: 2025/01/23 21:31:21 by vlchinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	real_len;
	size_t	i;
	char	*substr;

	if (start > ft_strlen(s))
		start = ft_strlen(s);
	real_len = ft_strlen(&s[start]);
	if (real_len < len)
		len = real_len;
	substr = malloc(sizeof(char) * (len + 1));
	if (!substr)
		return (0);
	i = 0;
	while (i < len)
	{
		substr[i] = s[start];
		s++;
		i++;
	}
	substr[i] = '\0';
	return (substr);
}
