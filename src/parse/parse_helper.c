/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:48:32 by vlchinen          #+#    #+#             */
/*   Updated: 2025/11/01 19:51:33 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int command_len(t_list *cmd)
{
    int	size;

	size = 0;
	while (cmd != NULL && ft_strncmp((char *)cmd->content, "|", 2) != 0)
	{
		size++;
		cmd = cmd->next;
	}
	return (size);
}

int cmds_count(t_list *cmd)
{
    int	count;

	count = 0;
	if (cmd != NULL)
		count++;
	while (cmd != NULL)
	{
		if (ft_strncmp((char *)(cmd->content), "|", 2) == 0)
			count++;
		cmd = cmd->next;
	}
	return (count);
}

char	***allocate_cmds(t_list **cmds_list)
{
	t_list	*cmd;
	char	***cmds;
	int		count;
	int		len;
	int		i;
	int		j;
	
	cmd = *cmds_list;
	count = cmds_count(cmd);
	cmds = (char ***)malloc(sizeof(char **) * (count + 1));
	if (!cmds)
		return (0);
	i = 0;
	while (i < count)
	{
		len = command_len(cmd);
		cmds[i] = (char **)malloc(sizeof(char *) * (len + 1));
		j = 0;
		while (j < len)
		{
			cmds[i][j] = ft_strdup((char *)(cmd->content));
			cmd = cmd->next;
			if (cmd && ft_strncmp((char *)(cmd->content), "|", 2) == 0)
				cmd = cmd->next;
			j++;
		}
		cmds[i][j] = 0;
		i++;
	}
	cmds[i] = 0;
	return (cmds);
}