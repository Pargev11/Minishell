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
	if (*command)
	{
		add_history(command);
		printf("%s\n", command);
	}
	free(command);
}
