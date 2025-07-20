/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:21:48 by pamalkha          #+#    #+#             */
/*   Updated: 2025/01/22 16:04:12 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	number_size(int nb)
{
	int	res;

	res = 0;
	while (nb / 10 != 0)
	{
		res++;
		nb /= 10;
	}
	return (res);
}

static int	ten_pow(int n)
{
	int	res;

	res = 1;
	while (n > 0)
	{
		res *= 10;
		n--;
	}
	return (res);
}

void	ft_putnbr_fd(int n, int fd)
{
	long long	nb2;
	int			size;
	char		tmp;

	size = number_size(n);
	if (n == 0)
		write(fd, "0", 1);
	else
	{
		nb2 = n;
		if (nb2 < 0)
		{
			write(fd, "-", 1);
			nb2 = -nb2;
		}
		while (size >= 0)
		{
			tmp = nb2 / ten_pow(size) + '0';
			nb2 %= ten_pow(size);
			write(fd, &tmp, 1);
			size--;
		}
	}
}
