/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_write.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:31:00 by pargev            #+#    #+#             */
/*   Updated: 2025/10/12 22:43:12 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_builtin(char **cmds, t_minishell *data)
{
	if (!ft_strncmp(cmds[0], "cd", 3))
		return (cd(cmds, data));
	else if (!ft_strncmp(cmds[0], "pwd", 4))
		return (pwd(cmds));
	else if (!ft_strncmp(cmds[0], "exit", 5))
		return (exit_cmd(cmds, data));
	else if (!ft_strncmp(cmds[0], "echo", 5))
		return (echo(cmds, data));
	else if (!ft_strncmp(cmds[0], "export", 7))
		return (export(cmds, data));
	else if (!ft_strncmp(cmds[0], "env", 4))
		return (env(cmds, data));
	else if (!ft_strncmp(cmds[0], "unset", 6))
		return (unset(cmds, data));
	return (-1);
}

void	analize_command(char *command, t_minishell *data)
{
	char	**cmds;
	int		exit_code;

	if (!command)
		exit_cmd(NULL, data);
	if (*command)
	{
		add_history(command);
		cmds = parse_words(command, data);
		if (!*cmds)
			return (free(cmds), free(command));
		exit_code = run_builtin(cmds, data);
		if (exit_code < 0)
			exit_code = exec(cmds, data);
		data->exit_code = exit_code;
		free_arr(cmds, NULL);
	}
	free(command);
}
