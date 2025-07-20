/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlchinen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 15:59:18 by vlchinen          #+#    #+#             */
/*   Updated: 2025/02/20 15:02:07 by vlchinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t		i;
	char		*casted_dst;
	const char	*casted_src;

	casted_dst = dst;
	casted_src = src;
	i = 0;
	if (!dst && !src)
		return (NULL);
	while (i < n)
	{
		casted_dst[i] = casted_src[i];
		i++;
	}
	return (dst);
}
