/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:21:48 by pamalkha          #+#    #+#             */
/*   Updated: 2025/01/22 15:05:35 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	srcsize;
	size_t	destsize;
	size_t	i;

	srcsize = ft_strlen(src);
	destsize = ft_strlen(dst);
	i = 0;
	if (size <= destsize)
		return (size + srcsize);
	while (src[i] && (destsize + i) < (size - 1))
	{
		dst[destsize + i] = src[i];
		i++;
	}
	dst[destsize + i] = 0;
	return (srcsize + destsize);
}
