/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlchinen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 20:19:50 by vlchinen          #+#    #+#             */
/*   Updated: 2025/02/20 15:32:27 by vlchinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>
#include <stdint.h>

void	*ft_calloc(size_t count, size_t size)
{
	size_t	objs_s;
	void	*ptr;

	if (!count || !size)
		return (malloc(1));
	if (count > SIZE_MAX / count)
		return (0);
	objs_s = count * size;
	ptr = malloc(objs_s);
	if (!ptr)
		return (0);
	while (objs_s > 0)
	{
		objs_s--;
		((char *)ptr)[objs_s] = 0;
	}
	return (ptr);
}
