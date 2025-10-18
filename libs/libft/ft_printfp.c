/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printfp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 16:54:52 by pamalkha          #+#    #+#             */
/*   Updated: 2025/10/18 16:54:52 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	printnb_in_base2(ssize_t nbr, char *base, size_t base_len)
{
	char	res[20];
	int		i;
	size_t	size;

	size = 0;
	i = 0;
	while (nbr != 0)
	{
		res[i] = base[nbr % base_len];
		nbr /= base_len;
		i++;
	}
	size = i;
	while (--i >= 0)
		ft_putcharp(res[i]);
	return (size);
}

size_t	nb_in_base(ssize_t nbr, char *base, size_t base_len, const char c)
{
	size_t	size;

	size = 0;
	if (c == 'd' || c == 'i')
	{
		if (nbr < 0)
		{
			size += ft_putcharp('-');
			nbr = -nbr;
		}
	}
	if (c == 'p')
		size += ft_putstrp("0x");
	if (nbr == 0)
		return (ft_putcharp('0') + size);
	size += printnb_in_base2(nbr, base, base_len);
	return (size);
}

size_t	print_variable(va_list args, const char c)
{
	size_t	size;
	char	*hex;
	char	*hex2;

	hex = "0123456789abcdef";
	hex2 = "0123456789ABCDEF";
	size = 0;
	if (c == 'c')
		size += ft_putcharp(va_arg(args, int));
	else if (c == 's')
		size += ft_putstrp(va_arg(args, char *));
	else if (c == '%')
		size += ft_putcharp('%');
	else if (c == 'd' || c == 'i')
		size += nb_in_base(va_arg(args, int), "0123456789", 10, c);
	else if (c == 'u')
		size += nb_in_base(va_arg(args, unsigned int), "0123456789", 10, c);
	else if (c == 'x')
		size += nb_in_base(va_arg(args, unsigned int), hex, 16, c);
	else if (c == 'X')
		size += nb_in_base(va_arg(args, unsigned int), hex2, 16, c);
	else if (c == 'p')
		size += nb_in_base(va_arg(args, long long), hex, 16, c);
	return (size);
}

int	ft_printfp(const char *str, ...)
{
	size_t	size;
	char	*sep;
	va_list	args;

	size = 0;
	va_start(args, str);
	sep = "cspdiuxX";
	while (*str)
	{
		if (*str == '%' && ft_strchr2(sep, *++str))
			size += print_variable(args, *str);
		else
			size += ft_putcharp(*str);
		str++;
	}
	va_end(args);
	return (size);
}
