/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:24:31 by vlchinen          #+#    #+#             */
/*   Updated: 2025/11/01 22:56:07 by pargev           ###   ########.fr       */
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

	if (ft_strchr(program, '/'))
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
		if (!access(fullpath, F_OK) && !is_dir(fullpath))
			return (free_arr(arr, NULL), fullpath);
		else
			free(fullpath);
	}
	return (free_arr(arr, NULL), ft_strdup(""));
}

int	exec_child(char *program_path, char **cmds, t_minishell *data)
{
	char	**env;

	env = list_to_env(data);
	execve(program_path, cmds, env);
	if (!is_dir(program_path))
	{
		free_arr(env, NULL);
		printf("bash: %s: %s\n", program_path, strerror(errno));
		if (errno == ENOTDIR || errno == ENOENT)
			return (127);
	}
	else
		printf("bash: %s: is a directory\n", program_path);
	free_arr(env, NULL);
	return (126);
}

int	exec(char **cmds, t_minishell *data)
{
	char	*program_path;
	int		status;

	status = 0;
	program_path = search_for_path(data, cmds[0]);
	if (program_path && *program_path)
	{
		signal(SIGINT, print_nl_handler);
		status = exec_child(program_path, cmds, data);
		signal(SIGINT, interrupt_signal);
	}
	else if (program_path)
	{
		printf("bash: %s: command not found\n", cmds[0]);
		status = 127 << 8;
	}
	free(program_path);
	return (status);
}
