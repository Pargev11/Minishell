/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_write.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:31:00 by pargev            #+#    #+#             */
/*   Updated: 2025/12/07 20:22:49 by pargev           ###   ########.fr       */
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

void	execute_pipeline(t_cmds *cmds, int commands_count, t_minishell *data)
{
	int		i;
	int		fd_index;
	int		fd[2];
	pid_t	*pids;
	int		in_fd;

	// printf("=====\n");
	in_fd = STDIN_FILENO;
	pids = (pid_t *)malloc(sizeof(pid_t) * commands_count);
	if (!pids)
		return ;
	i = 0;
	fd_index = 0;
	signal(SIGINT, print_nl_handler);
	while (i < commands_count)
	{
		if (i < commands_count - 1)
			pipe(fd);
		pids[i] = fork();
		if (pids[i] == 0)
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
			data->exit_code = handle_redirects(cmds, i, &fd_index);
			if (!data->exit_code && cmds->cmds[i] && cmds->cmds[i][0] && !run_builtin(cmds->cmds[i], data))
				exec(cmds->cmds[i], data);
			exit(data->exit_code);
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
	signal(SIGINT, interrupt_signal);
}

void	execute_command(t_cmds *cmds, t_minishell *data)
{
	pid_t	pid;
	int		status;
	int		fd_index;

	fd_index = 0;
	data->exit_code = handle_redirects(cmds, 0, &fd_index);
	if (cmds->cmds[0] && cmds->cmds[0][0] && !run_builtin(cmds->cmds[0], data))
	{
		signal(SIGINT, print_nl_handler);
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
		signal(SIGINT, interrupt_signal);
	}
}

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

void	analize_command(char *command, t_minishell *data)
{
	t_cmds	cmds;
	int		i;
	
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
			if (char_cmds_count(cmds.cmds) == 1 && changes_shell_state(cmds.cmds[0]))
				execute_command(&cmds, data);
			else
				execute_pipeline(&cmds, char_cmds_count(cmds.cmds), data);
			i = 0;
			while (cmds.cmds[i])
			{
				free_arr(cmds.cmds[i], NULL);
				i++;
			}
		}
		if (cmds.cmds)
			free(cmds.cmds);
		if (cmds.quote_mask)
			free_mask(cmds.quote_mask);
		if (cmds.stdin_fd)
			free(cmds.stdin_fd);
	}
	free(command);
}
