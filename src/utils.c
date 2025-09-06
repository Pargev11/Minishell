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

int	get_arr_sz(char **arr_2d)
{
	int	i;

	i = 0;
	while (*arr_2d)
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