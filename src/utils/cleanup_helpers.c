/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlchinen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:24:50 by vlchinen          #+#    #+#             */
/*   Updated: 2025/07/20 17:24:52 by vlchinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_arr(char **arr, char ***cmds)
{
	char	**arrbk;
	char	***cmdsbk;

	if (cmds)
		arr = *cmds;
	if (cmds)
		cmdsbk = cmds;
	arrbk = arr;
	while (*arr)
	{
		free(*arr);
		arr++;
		if (cmds && !*arr && cmds[1])
		{
			free(*cmds);
			arr = *(++cmds);
		}
	}
	if (!cmds)
		free(arrbk);
	else
	{
		free(*cmds);
		free(cmdsbk);
	}
}
