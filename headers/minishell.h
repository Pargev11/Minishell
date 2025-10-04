/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:30:58 by pargev            #+#    #+#             */
/*   Updated: 2025/10/04 21:26:21 by pargev           ###   ########.fr       */
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

typedef struct s_var_info
{
	char	*name;
	char	*value;
}	t_var_info;

typedef struct s_minishell
{
	char	*cwd;
	char	**env;
	t_list	**env_list;
}	t_minishell;

typedef struct s_env_info
{
	char	*env_name;
	char	*env_value_name;
}	t_env_info;


//signals
void	print_nl_handler(int sig);
void	interrupt_signal(int sig);

void	init(t_minishell *data);
void	end_program(t_minishell *data);

void	analize_command(char *command, t_minishell *data);

char	**parse_words(char *cmd);

//built-ins
int		cd(char **cmds, t_minishell *data);
int		pwd(char **cmds);
int		exit_cmd(char **cmds, t_minishell *data);
int		echo(char *command, char **cmds, t_minishell *data);
int		export(char **cmds, t_minishell *data);

//bins execution
void	exec(char **cmds, t_minishell *data);
char	*search_for_path(char **envp, char *program);
char	*dist_path_line(char **envp);

//cleanup
void	free_arr(char **arr, char ***cmds);

//utils
int		ft_strcmp(const char *s1, const char *s2);
int		check_name(char *name);

//envirement variables
t_list		**env_to_list();
char		**list_to_env(t_list *env_list);
void		lst_add_sorted(t_list **lst, t_list *new);
t_var_info	*lst_content(t_list	*lst);
void		free_variable(void	*lst);

#endif