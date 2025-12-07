/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:31:00 by pargev            #+#    #+#             */
/*   Updated: 2025/12/07 23:09:00 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redirections_count(char **cmds, int *quote_mask)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (cmds[i])
	{
		if (!quote_mask[i] && is_redirection(cmds[i]))
			count += 2;
		i++;
	}
	if (count == 0)
		return (-1);
	return (i - count);
}

void	remove_redirects2(char **cmds, int *quote_mask, char **new_cmd)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (cmds[i])
	{
		if (!(!quote_mask[i] && is_redirection(cmds[i])))
		{
			new_cmd[j] = ft_strdup(cmds[i]);
			free(cmds[i]);
			j++;
		}
		else
		{
			free(cmds[i]);
			free(cmds[++i]);
		}
		i++;
	}
	new_cmd[j] = 0;
}

void	remove_redirects(char ***cmds, int i, int **quote_mask)
{
	int		count;
	char	**new_cmd;

	count = redirections_count(cmds[i], quote_mask[i]);
	if (count < 0)
		return ;
	new_cmd = (char **)malloc(sizeof(char *) * (count + 1));
	remove_redirects2(cmds[i], quote_mask[i], new_cmd);
	free(cmds[i]);
	cmds[i] = new_cmd;
}
