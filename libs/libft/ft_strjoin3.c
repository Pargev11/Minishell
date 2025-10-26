/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 17:20:44 by pamalkha          #+#    #+#             */
/*   Updated: 2025/10/26 14:51:47 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin3(char *s1, char *s2)
{
	char	*res;
	char	*ptr;
	int		i;

	if (!s1)
	{
		s1 = (char *)malloc(sizeof(char));
		s1[0] = 0;
	}
	if (!s1 || !s2)
		return (0);
	res = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	ptr = res;
	if (!res)
		return (0);
	i = 0;
	while (s1[i])
		*res++ = s1[i++];
	i = 0;
	while (s2[i])
		*res++ = s2[i++];
	free(s1);
	free(s2);
	*res = 0;
	return (ptr);
}
