/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:24:31 by vlchinen          #+#    #+#             */
/*   Updated: 2025/09/14 17:59:52 by pamalkha         ###   ########.fr       */
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

	if (!access(program, X_OK) && ft_strchr(program, '/'))
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
		if (!access(fullpath, X_OK))
			return (free_arr(arr, NULL), fullpath);
		else
			free(fullpath);
	}
	return (free_arr(arr, NULL), ft_strdup(""));
}

void	exec(char **cmds, t_minishell *data)
{
	pid_t		pid;
	char		*program_path;

	program_path = search_for_path(data->env, cmds[0]);
	printf("program path is [%s]\n", program_path);
	if (program_path && *program_path)
	{
		signal(SIGINT, print_nl_handler);
		pid = fork();
		if (!pid)
			execve(program_path, cmds, data->env);
		waitpid(pid, NULL, 0);
		signal(SIGINT, interrupt_signal);
	}
	free(program_path);
	// here we will need to obtain exit code of a program and also terminate it by ctrl^c
}
