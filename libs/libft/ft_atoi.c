/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlchinen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:54:41 by vlchinen          #+#    #+#             */
/*   Updated: 2025/01/25 19:28:48 by vlchinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int	nb;
	int	s;

	nb = 0;
	s = 1;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if ((str[0] != '+' && str[0] != '-' && !ft_isdigit(str[0]))
		|| (!ft_isdigit(str[1]) && str[1] && !ft_isdigit(str[0])))
		return (0);
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			s = -1;
		str++;
	}
	while (ft_isdigit(*str))
	{
		nb *= 10;
		nb += *str - '0';
		str++;
	}
	return (nb * s);
}
