/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_with_quotes2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlchinen <vlchinen@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 22:24:44 by vlchinen          #+#    #+#             */
/*   Updated: 2025/09/21 22:25:03 by vlchinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**complete_free(char **arr)
{
	size_t	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	return (0);
}

int	count_words(char const *s, char const *set)
{
	int		sep[2];
	char	quote_type;
	int		w_c;

	w_c = 0;
	sep[1] = 1;
	while (*s)
	{
		sep[0] = sep[1];
		if (!quote_type && (*s == '\'' || *s == '"'))
			quote_type = *s;
		else if (*s == quote_type)
			quote_type = '\0';
		if (ft_strchr(set, *s) && !quote_type)
			sep[1] = 1;
		else
			sep[1] = 0;
		if (!sep[1] && sep[0])
			w_c++;
		s++;
	}
	if (quote_type)
		return (-1);
	return (w_c);
}
