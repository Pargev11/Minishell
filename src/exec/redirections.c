/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:31:00 by pargev            #+#    #+#             */
/*   Updated: 2025/12/07 23:20:15 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_read(char *delimiter, int quotes, int fd[2], t_minishell *data)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			printf("\nbash: warning: here-document at line delimited by "
				"end-of-file (wanted `%s')\n", delimiter);
			rl_on_new_line();
			break ;
		}
		if (!ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1))
			break ;
		if (!quotes)
			line = subst_vars(line, data, NULL);
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	free(line);
}

int	handle_heredoc(char	*delimiter, int quotes, t_minishell *data)
{
	int		fd[2];
	pid_t	pid;
	int		status;

	signal(SIGINT, SIG_IGN);
	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, print_nl_handler_and_exit);
		heredoc_read(delimiter, quotes, fd, data);
		exit(0);
	}
	waitpid(pid, &status, 0);
	if (WEXITSTATUS(status) != 0)
		return (-1);
	signal(SIGINT, interrupt_signal);
	close(fd[1]);
	return (fd[0]);
}

int	handle_redirects2(char *operator, char *filename, int ft, char *error_str)
{
	if (!ft_strncmp(operator, "<", 2))
		ft = open(filename, O_RDONLY);
	else if (!ft_strncmp(operator, ">", 2))
		ft = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (!ft_strncmp(operator, ">>", 3))
		ft = open(filename, O_WRONLY | O_CREAT
				| O_APPEND, 0644);
	if (ft < 0)
	{
		error_str = ft_strjoin("bash: ", filename);
		error_str = ft_strjoin2(error_str, ": ");
		perror(error_str);
		free(error_str);
		return (1);
	}
	else
	{
		if (!ft_strncmp(operator, "<", 2))
			dup2(ft, STDIN_FILENO);
		else if (!ft_strncmp(operator, ">", 2)
			|| !ft_strncmp(operator, ">>", 3))
			dup2(ft, STDOUT_FILENO);
		close(ft);
	}
	return (0);
}

int	handle_redirects(t_cmds *cmds, int i, int *fd_index)
{
	int		j;
	char	*operator;

	j = 0;
	while (cmds->cmds[i][j])
	{
		operator = cmds->cmds[i][j];
		if (!cmds->quote_mask[i][j] && !ft_strncmp(operator, "<<", 3))
		{
			dup2(cmds->stdin_fd[*fd_index], STDIN_FILENO);
			close(cmds->stdin_fd[*fd_index]);
			*fd_index = *fd_index + 1;
		}
		else if (!cmds->quote_mask[i][j] && (!ft_strncmp(operator, "<", 2)
		|| !ft_strncmp(operator, ">", 2) || !ft_strncmp(operator, ">>", 3)))
		{
			j++;
			if (handle_redirects2(operator, cmds->cmds[i][j], 0, NULL))
				return (1);
		}
		j++;
	}
	remove_redirects(cmds->cmds, i, cmds->quote_mask);
	return (0);
}
