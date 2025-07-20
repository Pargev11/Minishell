/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:21:48 by pamalkha          #+#    #+#             */
/*   Updated: 2025/01/24 17:20:45 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*ptr;
	unsigned int	i;
	size_t			size;
	char			*res;

	if (ft_strlen(s) <= start)
		return (ft_strdup(""));
	size = len;
	if (ft_strlen(&s[start]) < len)
		size = ft_strlen(&s[start]);
	ptr = (char *)malloc(sizeof(char) * (size + 1));
	res = ptr;
	if (!ptr)
		return (0);
	i = start;
	while (size--)
	{
		*ptr = s[i];
		ptr++;
		i++;
	}
	*ptr = 0;
	return (res);
}
