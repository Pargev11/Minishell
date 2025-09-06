/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:24:31 by vlchinen          #+#    #+#             */
/*   Updated: 2025/07/20 21:00:39 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*dist_path_line(char **envp)
{
	size_t	varlen;
	char	*varname;

	varname = "PATH";
	varlen = ft_strlen(varname);
	while (*envp && envp++)
	{
		if (!ft_strncmp(varname, envp[-1], varlen))
			return (envp[-1]);
	}
	return (NULL);
}

char	*search_for_path(char **envp, char *program)
{
	char	**paths;
	char	**arr;
	char	*fullpath;
	char	*tmpline;

	if (ft_strchr(program, '/'))
		return (ft_strdup(program));
	paths = ft_split(dist_path_line(envp), ':');
	if (!paths)
		return (free(program), NULL);
	arr = paths;
	while (*paths && ++paths)
	{
		fullpath = ft_strjoin(paths[-1], "/");
		tmpline = fullpath;
		fullpath = ft_strjoin(fullpath, program);
		free(tmpline);
		if (!fullpath)
			return (free_arr(arr, NULL), NULL);
		if (!access(fullpath, F_OK) && !is_dir(fullpath))
			return (free_arr(arr, NULL), fullpath);
		else
			free(fullpath);
	}
	return (free_arr(arr, NULL), ft_strdup(""));
}

void	exec_child(char *program_path, char **cmds, char **env)
{
	execve(program_path, cmds, env);
	if (!is_dir(program_path))
	{
		printf("bash: %s: %s\n", program_path, strerror(errno));
		if (errno == ENOTDIR || errno == ENOENT)
			exit (127);
	}
	else
		printf("bash: %s: is a directory\n", program_path);
	exit(126);
}

int	exec(char **cmds)
{
	pid_t		pid;
	char		*program_path;
	extern char	**environ;
	int			status;

	program_path = search_for_path(environ, cmds[0]);
	if (program_path && *program_path)
	{
		signal(SIGINT, print_nl_handler);
		pid = fork();
		if (!pid)
			exec_child(program_path, cmds, environ);
		waitpid(pid, &status, 0);
		signal(SIGINT, interrupt_signal);	
	}
	else if (program_path)
	{
		printf("bash: %s: command not found\n", cmds[0]);
		status = 127 << 8;
	}
	free(program_path);
	return (WEXITSTATUS(status));
}
