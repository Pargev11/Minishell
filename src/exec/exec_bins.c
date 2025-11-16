/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:24:31 by vlchinen          #+#    #+#             */
/*   Updated: 2025/11/16 18:18:51 by pamalkha         ###   ########.fr       */
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
	{
		ft_printfp("bash: %s: No such file or directory\n", program);
		return (free(program), NULL);
	}
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
	int		error_code;

	env = list_to_env(data);
	execve(program_path, cmds, env);
	error_code = 0;
	free_arr(env, NULL);
	if (!is_dir(program_path))
	{
		ft_printfp("bash: %s: %s\n", program_path, strerror(errno));
		if (errno == ENOTDIR || errno == ENOENT)
			return (127);
	}
	else
		ft_printfp("bash: %s: Is a directory\n", program_path);
	return (126);
}

void	exec(char **cmds, t_minishell *data)
{
	char	*program_path;

	data->exit_code = 0;
	program_path = search_for_path(data, cmds[0]);
	if (program_path && *program_path)
	{
		signal(SIGINT, print_nl_handler);
		data->exit_code = exec_child(program_path, cmds, data);
		signal(SIGINT, interrupt_signal);
	}
	else if (program_path)
	{
		ft_printfp("bash: %s: command not found\n", cmds[0]);
		data->exit_code = 127;
	}
	free(program_path);
}
