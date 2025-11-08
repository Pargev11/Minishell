/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:31:00 by pargev            #+#    #+#             */
/*   Updated: 2025/11/08 19:39:08 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_redirects(char ***cmds, int i)
{
	int		j;
	int		k;
	int		count;
	char	**new_cmd;

	count = 0;
	j = 0;
	while (cmds[i][j])
	{
		if (!ft_strncmp(cmds[i][j], ">", 2) || !ft_strncmp(cmds[i][j], "<", 2) || !ft_strncmp(cmds[i][j], ">>", 3))
			count += 2;
		j++;
	}
	if (count == 0)
		return ;
	count = j - count;
	new_cmd = (char **)malloc(sizeof(char *) * (count + 1));
	j = 0;
	k = 0;
	while (cmds[i][j])
	{
		if (ft_strncmp(cmds[i][j], ">", 2) && ft_strncmp(cmds[i][j], "<", 2) && ft_strncmp(cmds[i][j], ">>", 3))
		{
			new_cmd[k] = ft_strdup(cmds[i][j]);
			free(cmds[i][j]);
			k++;
		}
		else
		{
			free(cmds[i][j]);
			free(cmds[i][++j]);
		}
		j++;
	}
	new_cmd[k] = 0;
	free(cmds[i]);
	cmds[i] = new_cmd;
}

void	handle_redirects(char ***cmds, int i)
{
	int		j;
	int		ft;
	char	*error_str;
	char	*operator;

	j = 0;
	while (cmds[i][j])
	{
		operator = cmds[i][j];
		if (!ft_strncmp(operator, "<", 2) || !ft_strncmp(operator, ">", 2) || !ft_strncmp(operator, ">>", 3))
		{
			j++;
			if (operator == NULL)
			{
				ft_printfp("syntax error near unexpected token `newline'\n");
				exit(2);
			}
			if (!ft_strncmp(operator, "<", 2))
				ft = open(cmds[i][j], O_RDONLY);
			else if (!ft_strncmp(operator, ">", 2))
				ft = open(cmds[i][j], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else if (!ft_strncmp(operator, ">>", 3))
				ft = open(cmds[i][j], O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (ft < 0)
			{
				error_str = ft_strjoin(operator, ": ");
				perror(error_str);
				free(error_str);
				exit(1);
			}
			else
			{
				if (!ft_strncmp(operator, "<", 2))
					dup2(ft, STDIN_FILENO);
				else if (!ft_strncmp(operator, ">", 2) || !ft_strncmp(operator, ">>", 3))
					dup2(ft, STDOUT_FILENO);
				close(ft);
			}
		}
		j++;
	}
	remove_redirects(cmds, i);
}
