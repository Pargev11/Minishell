/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_write.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:31:00 by pargev            #+#    #+#             */
/*   Updated: 2025/11/01 22:54:36 by pargev           ###   ########.fr       */
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

int	check_exit_code(int commands_count, pid_t *pids)
{
	int	i;
	int	status;
	int	exit_code;

	i = 0;
	exit_code = 0;
	while (i < commands_count)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
			exit_code = 128 + WTERMSIG(status);
		i++;
	}
	free(pids);
	return (exit_code);
}

void	execute_pipeline(char ***cmds, int commands_count, t_minishell *data)
{
	int		i;
	int 	fd[2];
	pid_t	*pids;
	int		in_fd;
	int		exit_code;

	in_fd = STDIN_FILENO;
	pids = (pid_t *)malloc(sizeof(pid_t) * commands_count);
	if (!pids)
		return ;
	i = 0;
	while (i < commands_count)
	{
		if (i < commands_count - 1)
			pipe(fd);
		pids[i] = fork();
		if (pids[i] == 0)
		{
			if (in_fd != STDIN_FILENO) {
				dup2(in_fd, STDIN_FILENO);
				close(in_fd);
            }
			if (i < commands_count - 1)
			{
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
			}
			exit_code = run_builtin(cmds[i], data);
			if (exit_code < 0)
				exec(cmds[i], data);
			exit(exit_code);
		}
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		if (i < commands_count - 1)
		{
			close(fd[1]);
			in_fd = fd[0];
		}
		i++;
	}
	data->exit_code = check_exit_code(commands_count, pids);
}

int	char_cmds_count(char ***cmds)
{
	int	count;

	count = 0;
	while (cmds[count])
		count++;
	return (count);
}

void	analize_command(char *command, t_minishell *data)
{
	char	***cmds;
	int		exit_code;
	int		i;

	if (!command)
		exit_cmd(NULL, data);
	if (*command)
	{
		add_history(command);
		cmds = parse_words(command, data);
		if (cmds)
		{
			execute_pipeline(cmds, char_cmds_count(cmds), data);
			i = 0;
			while (cmds[i])
			{
				free_arr(cmds[i], NULL);
				i++;
			}
			free(cmds);
		}
	}
	free(command);
}
