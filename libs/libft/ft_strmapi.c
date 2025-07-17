/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlchinen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 17:11:59 by vlchinen          #+#    #+#             */
/*   Updated: 2025/01/19 17:12:03 by vlchinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	len;
	size_t	i;
	char	*s_r;

	len = ft_strlen(s);
	s_r = malloc(sizeof(char) * (len + 1));
	if (!s_r)
		return (0);
	i = 0;
	while (i < len)
	{
		s_r[i] = f(i, s[i]);
		i++;
	}
	s_r[i] = '\0';
	return (s_r);
}
