/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_write.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:31:00 by pargev            #+#    #+#             */
/*   Updated: 2025/11/15 14:49:44 by pamalkha         ###   ########.fr       */
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
	return (exit_code);
}

void	execute_pipeline(char ***cmds, int commands_count, t_minishell *data)
{
	int		i;
	int		fd[2];
	pid_t	*pids;
	int		in_fd;

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
			handle_redirects(cmds, i);
			if (cmds[i] && !run_builtin(cmds[i], data))
				exec(cmds[i], data);
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
	char	*last_cmd;
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
		if (cmds && *cmds)
		{
			execute_pipeline(cmds, char_cmds_count(cmds), data);
			last_cmd = NULL;
			if (!cmds[char_cmds_count(cmds) - 1][1] || !cmds[char_cmds_count(cmds) - 1][2])
				last_cmd = ft_strdup(cmds[char_cmds_count(cmds) - 1][0]);
			i = 0;
			while (cmds[i])
			{
				free_arr(cmds[i], NULL);
				i++;
			}
			if (last_cmd && !ft_strncmp(last_cmd, "exit", 5))
			{
				free(last_cmd);
				end_program(data);
				exit(data->exit_code);
			}
			free(last_cmd);
		}
		if (cmds)
			free(cmds);
	}
	free(command);
}
