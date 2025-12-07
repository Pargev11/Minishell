/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:31:00 by pargev            #+#    #+#             */
/*   Updated: 2025/12/07 22:21:28 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_exit_code(int commands_count, pid_t *pids)
{
	int	i;
	int	status;
	int	exit_code;

	if (!pids)
		return (0);
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

void	pipe_redirect_stdio(int in_fd, int fd[2], int i, int commands_count)
{
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (i < commands_count - 1)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
	}
}

void	pipe_execute_cmd(t_cmds *cmds, int i, int *fd_index, t_minishell *data)
{
	data->exit_code = handle_redirects(cmds, i, fd_index);
	if (!data->exit_code && cmds->cmds[i] && cmds->cmds[i][0]
		&& !run_builtin(cmds->cmds[i], data))
		exec(cmds->cmds[i], data);
}

int	pipe_close_fds(int in_fd, int fd[2], int i, int commands_count)
{
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	if (i < commands_count - 1)
	{
		close(fd[1]);
		in_fd = fd[0];
	}
	return (in_fd);
}

void	execute_pipeline(t_cmds *cmds, int commands_count, t_minishell *data)
{
	int		i;
	int		fd_index;
	int		fd[2];
	pid_t	*pids;
	int		in_fd;

	pids = (pid_t *)malloc(sizeof(pid_t) * commands_count);
	i = 0;
	fd_index = 0;
	in_fd = STDIN_FILENO;
	while (pids && i < commands_count)
	{
		if (i < commands_count - 1)
			pipe(fd);
		pids[i] = fork();
		if (pids[i] == 0)
		{
			pipe_redirect_stdio(in_fd, fd, i, commands_count);
			pipe_execute_cmd(cmds, i, &fd_index, data);
			exit(data->exit_code);
		}
		in_fd = pipe_close_fds(in_fd, fd, i, commands_count);
		i++;
	}
	data->exit_code = check_exit_code(commands_count, pids);
}
