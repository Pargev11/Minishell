/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:48:32 by vlchinen          #+#    #+#             */
/*   Updated: 2025/12/07 19:52:13 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	interrupt_signal(int sig)
{
	(void)sig;
	printf("\n");
	rl_replace_line ("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	print_nl_handler(int sig)
{
	(void)sig;
	printf("\n");
}

void	print_nl_handler_and_exit(int sig)
{
	(void)sig;
	printf("\n");
	exit(130);
}
