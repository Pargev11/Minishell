/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:44:09 by pamalkha          #+#    #+#             */
/*   Updated: 2025/10/18 14:55:11 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo(char **cmds, t_minishell *data)
{
	char	end_of_line;
	char	*command_to_print;
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
		command_to_print = ft_strtrim(cmds[i], " ");
		printf("%s", command_to_print);
		free(command_to_print);
		if (cmds[i + 1])
			printf(" ");
		i++;
	}
	printf("%c", end_of_line);
	return (0);
}
