/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:44:09 by pamalkha          #+#    #+#             */
/*   Updated: 2025/10/05 19:33:24 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo(char *command, char **cmds, t_minishell *data)
{
	char	end_of_line;

	(void)data;
	end_of_line = '\n';
	if (cmds[1] && !ft_strncmp(cmds[1], "-n", 3))
	{
		command = ft_strnstr(command, "-n", sizeof(command));
		if (command)
			command += 3;
		end_of_line = '\0';
	}
	command = ft_strtrim(command, " ");
	printf("%s%c", command, end_of_line);
	return (1);
}
