/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_write.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:31:00 by pargev            #+#    #+#             */
/*   Updated: 2025/07/13 18:46:19 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	analize_command(char *command)
{
	char	**cmds;
	char	*cwd;

	if (*command)
	{
		add_history(command);
		cmds = parse_words(command);
		if (!ft_strncmp(cmds[0], "cd", 3))
		{
			if 
		}
		//actually "pwd" (as well as "cd") should take one and only one arg, but... will fix later
		else if (!ft_strncmp(cmds[0], "pwd", 3))
		{
			cwd = getcwd(NULL, 0);
			printf("%s\n", cwd);
			free(cwd);
		}
		else
			printf("%s\n", command);
	}
	free(command);
}
