/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_write.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:31:00 by pargev            #+#    #+#             */
/*   Updated: 2025/11/08 19:14:49 by pamalkha         ###   ########.fr       */
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

void	remove_redirects(char ***cmds, int i)
{
	int		j;
	int		k;
	int		count;
	char	**new_cmd;

	count = 0;
	j = 0;
	while (cmds[i][j])
	{
		if (!ft_strncmp(cmds[i][j], ">", 2) || !ft_strncmp(cmds[i][j], "<", 2))
			count += 2;
		j++;
	}
	if (count == 0)
		return ;
	count = j - count;
	new_cmd = (char **)malloc(sizeof(char *) * (count + 1));
	j = 0;
	k = 0;
	while (cmds[i][j])
	{
		if (ft_strncmp(cmds[i][j], ">", 2) && ft_strncmp(cmds[i][j], "<", 2))
		{
			new_cmd[k] = ft_strdup(cmds[i][j]);
			free(cmds[i][j]);
			k++;
		}
		else
		{
			free(cmds[i][j]);
			free(cmds[i][++j]);
		}
		j++;
	}
	new_cmd[k] = 0;
	free(cmds[i]);
	cmds[i] = new_cmd;
}



void	handle_redirects(char ***cmds, int i)
{
	int		j;
	int		ft;
	char	*error_str;
	char	*operator;

	j = 0;
	while (cmds[i][j])
	{
		operator = cmds[i][j];
		if (!ft_strncmp(operator, "<", 2) || !ft_strncmp(operator, ">", 2))
		{
			j++;
			if (operator == NULL)
			{
				ft_printfp("syntax error near unexpected token `newline'\n");
				exit(2);
			}
			if (!ft_strncmp(operator, "<", 2))
				ft = open(cmds[i][j], O_RDONLY);
			else if (!ft_strncmp(operator, ">", 2))
				ft = open(cmds[i][j], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (ft < 0)
			{
				error_str = ft_strjoin(operator, ": ");
				perror(error_str);
				free(error_str);
				exit(1);
			}
			else
			{
				if (!ft_strncmp(operator, "<", 2))
					dup2(ft, STDIN_FILENO);
				else if (!ft_strncmp(operator, ">", 2))
				{
					dup2(ft, STDOUT_FILENO);
				}
				close(ft);
			}
		}
		j++;
	}
	remove_redirects(cmds, i);
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
		if (cmds)
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
			free(cmds);
			if (last_cmd && !ft_strncmp(last_cmd, "exit", 5))
			{
				free(last_cmd);
				end_program(data);
				exit(data->exit_code);
			}
			free(last_cmd);
		}
	}
	free(command);
}
