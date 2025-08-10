/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:44:09 by pamalkha          #+#    #+#             */
/*   Updated: 2025/08/10 18:07:46 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_spaces(char *str)
{
	char	*res;

	if (!str || !*str)
		return (str);
	while (*str == ' ')
		str++;
	res = str;
	str = &str[ft_strlen(str) - 1];
	while (*str == ' ')
		str--;
	*++str = 0;
	return (res);
}

int	echo(char *command, char **cmds, t_minishell *data)
{
	char	end_of_line;

	end_of_line = '\n';
	if (cmds[1] && !ft_strncmp(cmds[1], "-n", 3))
	{
		command = ft_strnstr(command, "-n", sizeof(command));
		if (command)
			command += 3;
		end_of_line = '\0';
	}
	command = remove_spaces(command);
	printf("%s%c", command, end_of_line);
	return (1);
}
