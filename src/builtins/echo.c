/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:44:09 by pamalkha          #+#    #+#             */
/*   Updated: 2025/12/08 18:58:50 by pamalkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_n(char *str)
{
	if (str && *str == '-')
		str++;
	else
		return (0);
	while (*str == 'n')
		str++;
	if (*str == 0)
		return (1);
	return (0);
}

void	echo(char **cmds, t_minishell *data)
{
	char	end_of_line;
	int		i;

	(void)data;
	i = 1;
	end_of_line = '\n';
	if (check_n(cmds[1]))
	{
		end_of_line = '\0';
		i++;
	}
	while (cmds[i])
	{
		if (!check_n(cmds[i]))
		{
			printf("%s", cmds[i]);
			if (cmds[i + 1])
				printf(" ");
		}
		i++;
	}
	printf("%c", end_of_line);
}
