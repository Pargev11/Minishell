/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:21:48 by pamalkha          #+#    #+#             */
/*   Updated: 2025/01/22 11:38:07 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

void	*ft_calloc(size_t count, size_t size)
{
	void	*res;
	size_t	all_size;

	all_size = count * size;
	if (all_size == 0)
		all_size = 1;
	else if ((all_size != 0 && all_size / count != size))
		return (0);
	res = (void *)malloc(all_size);
	if (!res)
		return (0);
	ft_memset(res, 0, all_size);
	return (res);
}
