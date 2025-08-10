/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_write.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:31:00 by pargev            #+#    #+#             */
/*   Updated: 2025/08/10 17:47:59 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//actually "pwd" (as well as "cd") should take one and only one arg, but... will fix later
int	run_builtin(char *command, char **cmds, t_minishell *data)
{
	if (!ft_strncmp(cmds[0], "cd", 3))
		return (cd(cmds, data));
	else if (!ft_strncmp(cmds[0], "pwd", 4))
		return (pwd(cmds));
	else if (!ft_strncmp(cmds[0], "exit", 5))
		return (exit_cmd(cmds, data));
	else if (!ft_strncmp(cmds[0], "echo", 5))
		return (echo(command + 5, cmds, data));
	return (0);
}

void	analize_command(char *command, t_minishell *data)
{
	char	**cmds;

	if (!command)
		exit_cmd(NULL, data);
	if (*command)
	{
		add_history(command);
		cmds = parse_words(command);
		if (!run_builtin(command, cmds, data))
			exec(cmds);
		free_arr(cmds, NULL);
	}
	free(command);
}
