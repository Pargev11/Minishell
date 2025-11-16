/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:48:52 by vlchinen          #+#    #+#             */
/*   Updated: 2025/11/16 18:00:59 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd(char **cmds, t_minishell *data)
{
	int		arr_sz;
	char	*dir_to_ch;
	char	*error_str;

	data->exit_code = 0;
	arr_sz = get_arr_sz(cmds);
	dir_to_ch = cmds[1];
	if (arr_sz > 2)
	{
		data->exit_code = 1;
		ft_putendl_fd("cd: too many arguments", 2);
		return ;
	}
	else if (arr_sz == 1)
		dir_to_ch = get_varible("HOME", data);
	if (!chdir(dir_to_ch))
	{
		// printf("dir = %s\n", dir_to_ch);
		free(data->cwd);
		data->cwd = getcwd(NULL, 0);
	}
	else
	{
		error_str = ft_strjoin("cd: ", dir_to_ch);
		data->exit_code = 1;
		perror(error_str);
		free(error_str);
	}
}

void	pwd(char **cmds, t_minishell *data)
{
	char	*cwd;

	(void)cmds;
	cwd = getcwd(NULL, 0);
	printf("%s\n", cwd);
	free(cwd);
	data->exit_code = 0;
}

void	exit_cmd(char **cmds, t_minishell *data)
{
	int	arr_sz;

	arr_sz = get_arr_sz(cmds);
	if (cmds == NULL)
		printf("\n");
	printf("exit\n");
	if (arr_sz >= 2)
	{
		data->exit_code = ft_atoi(cmds[1]);
		if (!(*cmds[1]) || !(ft_isdigit(*cmds[1]) || *cmds[1] == '-' || *cmds[1] == '+') || !ft_strisnum(&cmds[1][1]))
		{
			ft_printfp("bash: exit: %s: numeric argument required\n",
				cmds[1]);
			data->exit_code = 2;
			exit(data->exit_code);
		}
	}
	if (arr_sz > 2)
	{
		ft_printfp("bash: exit: too many arguments\n");
		data->exit_code = 1;
	}
	end_program(data);
	exit(data->exit_code);
}
