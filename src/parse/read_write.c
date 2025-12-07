/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_write.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:31:00 by pargev            #+#    #+#             */
/*   Updated: 2025/12/07 22:10:43 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	char_cmds_count(char ***cmds)
{
	int	count;

	count = 0;
	while (cmds[count])
		count++;
	return (count);
}

int	changes_shell_state(char **cmd)
{
	if (!ft_strncmp(cmd[0], "cd", 3))
		return (1);
	if (!ft_strncmp(cmd[0], "exit", 5))
		return (1);
	if (!ft_strncmp(cmd[0], "export", 7) && cmd[1])
		return (1);
	if (!ft_strncmp(cmd[0], "unset", 6))
		return (1);
	return (0);
}

void	free_cmds(t_cmds *cmds)
{
	int	i;

	if (cmds->cmds)
	{
		if (*(cmds->cmds))
		{
			i = 0;
			while (cmds->cmds[i])
			{
				free_arr(cmds->cmds[i], NULL);
				i++;
			}
		}
		free(cmds->cmds);
	}
	if (cmds->quote_mask)
		free_mask(cmds->quote_mask);
	if (cmds->stdin_fd)
		free(cmds->stdin_fd);
}

void	analize_command(char *command, t_minishell *data)
{
	t_cmds	cmds;

	if (!command)
	{
		exit_cmd(NULL, data);
		end_program(data);
		exit(data->exit_code);
	}
	if (*command)
	{
		add_history(command);
		cmds = parse_words(command, data);
		if (cmds.cmds && *(cmds.cmds))
		{
			signal(SIGINT, print_nl_handler);
			if (char_cmds_count(cmds.cmds) == 1
				&& changes_shell_state(cmds.cmds[0]))
				execute_command(&cmds, data);
			else
				execute_pipeline(&cmds, char_cmds_count(cmds.cmds), data);
			signal(SIGINT, interrupt_signal);
		}
		free_cmds(&cmds);
	}
	free(command);
}
