/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printfp_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 16:55:04 by pamalkha          #+#    #+#             */
/*   Updated: 2025/10/18 16:55:04 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_putcharp(char c)
{
	write(2, &c, 1);
	return (1);
}

size_t	ft_putstrp(char *s)
{
	size_t	size;

	if (!s)
		return (ft_putstr("(null)"));
	size = ft_strlen(s);
	write (2, s, size);
	return (size);
}
