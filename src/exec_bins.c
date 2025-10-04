/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:24:31 by vlchinen          #+#    #+#             */
/*   Updated: 2025/10/05 00:10:46 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*dist_path_line(t_minishell *data)
{
	t_list	*current;

	current = *(data->env_list);
	while (current != NULL)
	{
		if (ft_strcmp(lst_content(current)->name, "PATH") == 0)
			return (lst_content(current)->value);
		current = current->next;
	}
	return (NULL);
}

char	*search_for_path(t_minishell *data, char *program)
{
	char	**paths;
	char	**arr;
	char	*fullpath;
	char	*tmpline;

	if (!access(program, X_OK) && ft_strchr(program, '/'))
		return (ft_strdup(program));
	paths = ft_split(dist_path_line(data), ':');
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
	char		**env;

	program_path = search_for_path(data, cmds[0]);
	// printf("program path is [%s]\n", program_path);
	env = NULL;
	if (program_path && *program_path)
	{
		signal(SIGINT, print_nl_handler);
		pid = fork();
		if (!pid)
		{
			env = list_to_env(data);
			execve(program_path, cmds, env);
		}
		waitpid(pid, NULL, 0);
		signal(SIGINT, interrupt_signal);
	}
	free(program_path);
	if (env != NULL)
		free_arr(env, NULL);
	// here we will need to obtain exit code of a program and also terminate it by ctrl^c
}
