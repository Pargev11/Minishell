/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:21:48 by pamalkha          #+#    #+#             */
/*   Updated: 2025/01/22 16:03:42 by pamalkha         ###   ########.fr       */
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

static void	ft_putnbr(int nb, char *buffer)
{
	long long	nb2;
	int			size;
	char		tmp;

	size = number_size(nb);
	if (nb == 0)
		*buffer++ = '0';
	else
	{
		nb2 = nb;
		if (nb2 < 0)
		{
			*buffer++ = '-';
			nb2 = -nb2;
		}
		while (size >= 0)
		{
			tmp = nb2 / ten_pow(size) + '0';
			nb2 %= ten_pow(size);
			*buffer++ = tmp;
			size--;
		}
	}
	*buffer = 0;
}

char	*ft_itoa(int n)
{
	char	*ptr;
	int		size;

	size = number_size(n);
	if (n < 0)
		size++;
	ptr = (char *)malloc(sizeof(char) * (size + 2));
	if (!ptr)
		return (0);
	ft_putnbr(n, ptr);
	return (ptr);
}
