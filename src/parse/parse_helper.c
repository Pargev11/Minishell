/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:48:32 by vlchinen          #+#    #+#             */
/*   Updated: 2025/11/22 22:56:53 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	command_len(t_list *cmd)
{
	int	size;

	size = 0;
	while (cmd != NULL && !(!get_cmd(cmd)->quotes && !ft_strncmp(get_cmd(cmd)->cmd, "|", 2)))
	{
		size++;
		cmd = cmd->next;
	}
	return (size);
}

int	cmds_count(t_list *cmd)
{
	int	count;

	count = 0;
	if (cmd != NULL)
		count++;
	while (cmd != NULL)
	{
		if (!get_cmd(cmd)->quotes && ft_strncmp(get_cmd(cmd)->cmd, "|", 2) == 0)
			count++;
		cmd = cmd->next;
	}
	return (count);
}

char	**allocate_cmd(t_list **cmd, int **quote_mask, int i)
{
	int		len;
	char	**cmd_c;
	int		j;

	len = command_len(*cmd);
	cmd_c = (char **)malloc(sizeof(char *) * (len + 1));
	quote_mask[i] = (int *)malloc(sizeof(int) * (len + 1));
	if (!cmd_c || !quote_mask[i])
		return (0);
	j = 0;
	while (j < len)
	{
		cmd_c[j] = ft_strdup(get_cmd(*cmd)->cmd);
		quote_mask[i][j] = get_cmd(*cmd)->quotes;
		*cmd = (*cmd)->next;
		if (*cmd && !get_cmd(*cmd)->quotes && ft_strncmp(get_cmd(*cmd)->cmd, "|", 2) == 0)
			*cmd = (*cmd)->next;
		j++;
	}
	cmd_c[j] = 0;
	quote_mask[i][j] = 0;
	return (cmd_c);
}

char	***allocate_cmds(t_list **cmds_list, int ***quote_mask)
{
	t_list	**cmd;
	char	***cmds;
	int		count;
	int		i;

	cmd = (t_list **)malloc(sizeof(t_list *));
	if (!cmd)
		return (0);
	*cmd = *cmds_list;
	count = cmds_count(*cmd);
	cmds = (char ***)malloc(sizeof(char **) * (count + 1));
	*quote_mask = (int **)malloc(sizeof(int *) * (count + 1));
	if (!cmds || !*quote_mask)
		return (0);
	i = 0;
	while (i < count)
	{
		cmds[i] = allocate_cmd(cmd, *quote_mask, i);
		i++;
	}
	cmds[i] = 0;
	(*quote_mask)[i] = 0;
	free(cmd);
	return (cmds);
}
