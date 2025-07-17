/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlchinen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 18:02:09 by vlchinen          #+#    #+#             */
/*   Updated: 2025/02/20 12:29:09 by vlchinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

void	ft_putnbr_fd(int n, int fd)
{
	int		rem;
	int		quot;
	int		m;
	char	c;

	m = 0;
	if (n < 0)
	{
		m = 1;
		write(fd, "-", 1);
	}
	if (!m)
	{
		rem = n % 10;
		quot = n / 10;
	}
	else
	{
		rem = -(n % 10);
		quot = -(n / 10);
	}
	c = rem + '0';
	if (quot)
		ft_putnbr_fd(quot, fd);
	write(fd, &c, 1);
}
