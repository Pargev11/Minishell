/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:48:52 by vlchinen          #+#    #+#             */
/*   Updated: 2025/10/26 15:00:16 by pamalkha         ###   ########.fr       */
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
		return ((void)ft_putendl_fd("cd: too many arguments\n", 2), 1);
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
	char	*cwd;

	(void)cmds;
	cwd = getcwd(NULL, 0);
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}

int	exit_cmd(char **cmds, t_minishell *data)
{
	int		arr_sz;
	int		error_code;

	error_code = 0;
	arr_sz = get_arr_sz(cmds);
	printf("exit\n");
	if (arr_sz > 2)
	{
		ft_printfp("bash: exit: too many arguments\n");
		return (1);
	}
	else if (arr_sz == 2)
	{
		error_code = ft_atoi(cmds[1]);
		if (!error_code && !ft_strisnum(cmds[1]))
		{
			ft_printfp("bash: exit: %s: numeric argument required\n",
				cmds[1]);
			error_code = 2;
		}
	}
	end_program(data);
	exit(error_code);
}
