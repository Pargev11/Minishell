/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:44:09 by pamalkha          #+#    #+#             */
/*   Updated: 2025/11/08 16:33:02 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo(char **cmds, t_minishell *data)
{
	char	end_of_line;
	int		i;

	(void)data;
	i = 1;
	end_of_line = '\n';
	if (cmds[1] && !ft_strncmp(cmds[1], "-n", 3))
	{
		end_of_line = '\0';
		i++;
	}
	while (cmds[i])
	{
		printf("%s", cmds[i]);
		if (cmds[i + 1])
			printf(" ");
		i++;
	}
	printf("%c", end_of_line);
	data->exit_code = 0;
}
