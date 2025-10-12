/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlchinen <vlchinen@student.42yerevan.am>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 20:47:35 by vlchinen          #+#    #+#             */
/*   Updated: 2025/08/24 20:47:37 by vlchinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*str_rm_idx(char *str, size_t idx)
{
	char	*p1;
	char	*p2;

	p1 = ft_substr(str, 0, idx);
	if (!p1)
		return (NULL);
	p2 = ft_substr(str, idx + 1, SIZE_MAX);
	if (!p2)
		return (free(p1), NULL);
	str = ft_strjoin(p1, p2);
	return (free(p1), free(p2), str);
}

int	get_arr_sz(char **arr_2d)
{
	int	i;

	i = 0;
	while (arr_2d && *arr_2d)
	{
		i++;
		arr_2d++;
	}
	return (i);
}

int	is_dir(char const *path)
{
	struct stat	buf;

	stat(path, &buf);
	if (S_ISDIR(buf.st_mode))
		return (1);
	return (0);
}
