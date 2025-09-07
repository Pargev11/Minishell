/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:48:52 by vlchinen          #+#    #+#             */
/*   Updated: 2025/07/20 20:56:42 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd(char **cmds, t_minishell *data)
{
	int		arr_sz;
	char	*dir_to_ch;
	char	*error_str;

	arr_sz = get_arr_sz(cmds);
	dir_to_ch = cmds[1];
	if (arr_sz > 2)
		return ((void)printf("cd: too many arguments\n"), 1);
	else if (arr_sz == 1)
		dir_to_ch = getenv("HOME");
	if (!chdir(dir_to_ch))
	{
		free(data->cwd);
		data->cwd = getcwd(NULL, 0);
	}
	else
	{
		error_str = ft_strjoin("cd: ", cmds[1]);
		return (perror(error_str), free(error_str), 1);
	}
	return (0);
}

int	pwd(char **cmds)
{
	int		arr_sz;
	char	*cwd;

	arr_sz = get_arr_sz(cmds);
	if (arr_sz > 1)
		return ((void)printf("pwd: too many arguments\n"), 1);
	cwd = getcwd(NULL, 0);
	printf("%s\n", cwd);
	free(cwd);
	return (1);
}
int	exit_cmd(char **cmds, t_minishell *data)
{
	int		arr_sz;

	arr_sz = get_arr_sz(cmds);
	if (arr_sz > 1)
		return ((void)printf("exit: too many arguments\n"), 1);
	printf("exit\n");
	end_program(data);
	exit(0);
}
