/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:31:00 by pargev            #+#    #+#             */
/*   Updated: 2025/12/07 22:21:47 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_builtin(char **cmds, t_minishell *data)
{
	data->exit_code = 0;
	if (!ft_strncmp(cmds[0], "cd", 3))
		cd(cmds, data);
	else if (!ft_strncmp(cmds[0], "pwd", 4))
		pwd(cmds, data);
	else if (!ft_strncmp(cmds[0], "exit", 5))
		exit_cmd(cmds, data);
	else if (!ft_strncmp(cmds[0], "echo", 5))
		echo(cmds, data);
	else if (!ft_strncmp(cmds[0], "export", 7))
		export(cmds, data);
	else if (!ft_strncmp(cmds[0], "env", 4))
		env(cmds, data);
	else if (!ft_strncmp(cmds[0], "unset", 6))
		unset(cmds, data);
	else
		return (0);
	return (1);
}

int	std_backup(int std_arr[2], int fd1, int fd2, enum e_Mode mode)
{
	if (mode == BACKUP)
	{
		std_arr[0] = dup(fd1);
		if (std_arr[0] == -1)
			return (1);
		std_arr[1] = dup(fd2);
		if (std_arr[1] == -1)
		{
			close(std_arr[0]);
			return (1);
		}
	}
	else if (mode == RESTORE)
	{
		if (dup2(std_arr[0], fd1) == -1)
			return (1);
		close(std_arr[0]);
		if (dup2(std_arr[1], fd2) == -1)
			return (1);
		close(std_arr[1]);
	}
	return (0);
}

void	execute_command(t_cmds *cmds, t_minishell *data)
{
	pid_t	pid;
	int		status;
	int		fd_index;
	int		std_backup_arr[2];

	fd_index = 0;
	std_backup(std_backup_arr, STDIN_FILENO, STDOUT_FILENO, BACKUP);
	data->exit_code = handle_redirects(cmds, 0, &fd_index);
	if (!data->exit_code && cmds->cmds[0] && cmds->cmds[0][0]
		&& !run_builtin(cmds->cmds[0], data))
	{
		pid = fork();
		if (pid == 0)
		{
			exec(cmds->cmds[0], data);
			exit(data->exit_code);
		}
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			data->exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			data->exit_code = 128 + WTERMSIG(status);
	}
	std_backup(std_backup_arr, STDIN_FILENO, STDOUT_FILENO, RESTORE);
}
