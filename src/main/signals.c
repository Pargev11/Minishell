/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamalkha <pamalkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:48:32 by vlchinen          #+#    #+#             */
/*   Updated: 2025/08/10 18:08:04 by pamalkha         ###   ########.fr       */
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
