/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlchinen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:02:31 by vlchinen          #+#    #+#             */
/*   Updated: 2025/01/23 20:39:21 by vlchinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	needle_size;

	needle_size = ft_strlen(needle);
	if (!needle_size)
		return ((char *)haystack);
	i = 0;
	while (haystack[i] && i < len)
	{
		if (!ft_strncmp(&haystack[i], needle, needle_size)
			&& (i + needle_size) <= len)
			return ((char *)&haystack[i]);
		i++;
	}
	return (0);
}
