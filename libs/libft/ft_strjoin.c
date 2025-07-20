/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:21:48 by pamalkha          #+#    #+#             */
/*   Updated: 2025/01/18 17:46:41 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		size;
	char	*res;
	char	*ptr;
	int		i;

	size = ft_strlen(s1) + ft_strlen(s2);
	res = (char *)malloc(sizeof(char) * (size + 1));
	ptr = res;
	if (!res)
		return (0);
	i = 0;
	while (s1[i])
	{
		*res = s1[i++];
		res++;
	}
	i = 0;
	while (s2[i])
	{
		*res = s2[i++];
		res++;
	}
	*res = 0;
	return (ptr);
}
