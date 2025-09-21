/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:30:58 by pargev            #+#    #+#             */
/*   Updated: 2025/07/25 22:28:47 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <termios.h>
# include <sys/stat.h>
# include <errno.h>

typedef struct s_minishell
{
	char	*cwd;
	int		exit_code;
}	t_minishell;

//signals
void	print_nl_handler(int sig);
void	interrupt_signal(int sig);

void	init(t_minishell *data);
void	end_program(t_minishell *data);

void	analize_command(char *command, t_minishell *data);

//parse
char	**parse_words(char *cmd, t_minishell *data);
char	**subst_vars(char **cmds, t_minishell *data);
char	**split_with_quotes(char const *s, char const *set);

//built-ins
int		cd(char **cmds, t_minishell *data);
int		pwd(char **cmds);
int	exit_cmd(char **cmds, t_minishell *data);

//bins execution
int		exec(char **cmds);
char	*search_for_path(char **envp, char *program);
char	*dist_path_line(char **envp);

//cleanup
void	free_arr(char **arr, char ***cmds);

//utils
int		get_arr_sz(char **arr_2d);
int		is_dir(char const *path);
char	*str_rm_idx(char *str, size_t idx);
char	*strreplace(char *str, char *str_to_replace, char *replacement);

#endif