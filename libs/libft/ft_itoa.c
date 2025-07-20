/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlchinen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 19:23:23 by vlchinen          #+#    #+#             */
/*   Updated: 2025/01/25 20:40:08 by vlchinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*allocate_num(int n)
{
	char	*str;
	int		len;

	len = 0;
	if (n <= 0)
		len++;
	while (n)
	{
		n /= 10;
		len++;
	}
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (0);
	return (str);
}

static char	*rev_str(char *str)
{
	size_t	i;
	size_t	len;
	char	tmp;

	len = ft_strlen(str);
	i = 0;
	while (i < len / 2)
	{
		tmp = str[i];
		str[i] = str[len - i - 1];
		str[len - i - 1] = tmp;
		i++;
	}
	return (str);
}

char	*ft_itoa(int n)
{
	char	*s;
	char	*backup;
	int		rem;

	s = allocate_num(n);
	if (!s)
		return (0);
	backup = s;
	if (!n)
		*s++ = '0';
	while (n)
	{
		rem = n % 10;
		if (rem < 0)
			rem *= -1;
		*s = rem + '0';
		s++;
		if (n < 0 && n > -10)
			*s++ = '-';
		n /= 10;
	}
	s[0] = '\0';
	return (rev_str(backup));
}
