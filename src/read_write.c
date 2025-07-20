/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_write.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:31:00 by pargev            #+#    #+#             */
/*   Updated: 2025/07/20 23:38:58 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//actually "pwd" (as well as "cd") should take one and only one arg, but... will fix later
int	run_builtin(char **cmds, t_minishell *data)
{
	if (!ft_strncmp(cmds[0], "cd", 3))
		return (cd(cmds, data));
	else if (!ft_strncmp(cmds[0], "pwd", 4))
		return (pwd(cmds));
	return (0);
}

void	analize_command(char *command, t_minishell *data)
{
	char	**cmds;

	if (!command)
	{
		printf("exit\n");
		end_program(data);
		exit(0);
	}
	if (*command)
	{
		add_history(command);
		cmds = parse_words(command);
		if (!run_builtin(cmds, data))
			exec(cmds);
		free_arr(cmds, NULL);
	}
	free(command);
}
