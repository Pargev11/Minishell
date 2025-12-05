/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_write.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:31:00 by pargev            #+#    #+#             */
/*   Updated: 2025/12/06 00:49:06 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_builtin(char **cmds, t_minishell *data)
{
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
	signal(SIGINT, interrupt_signal);
	return (exit_code);
}

void	execute_pipeline(char ***cmds, int commands_count, int **quote_mask, t_minishell *data)
{
	int		i;
	int		fd[2];
	pid_t	*pids;
	int		in_fd;
	int		std_backup[2];
	char	buf[4096];
	ssize_t n;

	std_backup[0] = dup(STDIN_FILENO);
	std_backup[1] = dup(STDOUT_FILENO);
	in_fd = STDIN_FILENO;
	pids = (pid_t *)malloc(sizeof(pid_t) * commands_count);
	if (!pids)
		return ;
	i = 0;
	signal(SIGINT, print_nl_handler);
	while (i < commands_count)
	{
		pipe(fd);
		pids[i] = fork();
		if (pids[i] == 0)
		{
			if (in_fd != STDIN_FILENO)
			{
				dup2(in_fd, STDIN_FILENO);
				close(in_fd);
			}
			dup2(fd[1], STDOUT_FILENO);
			close(fd[0]);
			close(fd[1]);
			data->exit_code = handle_redirects(cmds, i, quote_mask, data, std_backup);
			if (!data->exit_code && cmds[i] && cmds[i][0] && !run_builtin(cmds[i], data))
				exec(cmds[i], data);
			exit(data->exit_code);
		}
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		close(fd[1]);
		in_fd = fd[0];
		i++;
	}
	data->exit_code = check_exit_code(commands_count, pids);
	dup2(std_backup[1], STDOUT_FILENO);
	close(std_backup[1]);
	while ((n = read(fd[0], buf, sizeof(buf))) > 0)
		write(STDOUT_FILENO, buf, n);
	close(fd[0]);
}

void	execute_command(char ***cmds, int **quote_mask, t_minishell *data)
{
	pid_t	pid;
	int		status;
	int		std_backup[2];

	std_backup[0] = dup(STDIN_FILENO);
	std_backup[1] = dup(STDOUT_FILENO);
	data->exit_code = handle_redirects(cmds, 0, quote_mask, data, std_backup);
	if (cmds[0] && cmds[0][0] && !run_builtin(cmds[0], data))
	{
		signal(SIGINT, print_nl_handler);
		pid = fork();
		if (pid == 0)
		{
			exec(cmds[0], data);
			exit(data->exit_code);
		}
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			data->exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			data->exit_code = 128 + WTERMSIG(status);
		signal(SIGINT, interrupt_signal);
	}
	dup2(std_backup[0], STDIN_FILENO);
	dup2(std_backup[1], STDOUT_FILENO);
	close(std_backup[1]);
	close(std_backup[1]);
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
	char	***cmds;
	// char	*last_cmd;
	int		***quote_mask;
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
		quote_mask = (int ***)malloc(sizeof(int **));
		cmds = parse_words(command, quote_mask, data);
		if (cmds && *cmds)
		{
			if (char_cmds_count(cmds) == 1 && changes_shell_state(cmds[0]))
				execute_command(cmds, *quote_mask, data);
			else
				execute_pipeline(cmds, char_cmds_count(cmds), *quote_mask, data);
			// last_cmd = NULL;
			// if (!cmds[char_cmds_count(cmds) - 1][1] || !cmds[char_cmds_count(cmds) - 1][2])
			// 	last_cmd = ft_strdup(cmds[char_cmds_count(cmds) - 1][0]);
			// if (last_cmd && !ft_strncmp(last_cmd, "exit", 5))
			// {
			// 	free(last_cmd);
			// 	end_program(data);
			// 	exit(data->exit_code);
			// }
			// free(last_cmd);

			i = 0;
			while (cmds[i])
			{
				free_arr(cmds[i], NULL);
				i++;
			}
		}
		if (cmds)
		{
			free(cmds);
			free_mask(quote_mask);
		}
		else
			free(quote_mask);
	}
	free(command);
}
