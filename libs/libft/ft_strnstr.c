/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:21:48 by pamalkha          #+#    #+#             */
/*   Updated: 2025/01/18 15:32:19 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	i2;
	size_t	j;

	if (*needle == 0)
		return ((char *)haystack);
	i = 0;
	while (haystack[i])
	{
		i2 = i;
		j = 0;
		while (needle[j] && haystack[i2] == needle[j] && i2 < len)
		{
			j++;
			i2++;
		}
		if (needle[j] == 0)
			return ((char *)&haystack[i]);
		i++;
	}
	return (0);
}
