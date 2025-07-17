/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlchinen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 17:35:54 by vlchinen          #+#    #+#             */
/*   Updated: 2025/01/17 11:44:21 by vlchinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char			*casted_dst;
	const char		*casted_src;

	casted_dst = dst;
	casted_src = src;
	if (dst <= src)
	{
		ft_memcpy(dst, src, len);
	}
	else
	{
		while (len > 0)
		{
			casted_dst[len - 1] = casted_src[len - 1];
			len--;
		}
	}
	return (dst);
}
