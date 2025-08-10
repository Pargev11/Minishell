/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:48:52 by vlchinen          #+#    #+#             */
/*   Updated: 2025/08/10 18:08:16 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd(char **cmds, t_minishell *data)
{
	if (!chdir(cmds[1]))
	{
		free(data->cwd);
		data->cwd = getcwd(NULL, 0);
	}
	else
		printf("cd: %s: No such file or directory\n", cmds[1]);
	return (1);
}

int	pwd(char **cmds)
{
	char	*cwd;

	(void)cmds;
	//if ac > 2, than throw error
	cwd = getcwd(NULL, 0);
	printf("%s\n", cwd);
	free(cwd);
	return (1);
}

int	exit_cmd(char **cmds, t_minishell *data)
{
	(void)cmds;
	printf("exit\n");
	end_program(data);
	exit(0);
}
