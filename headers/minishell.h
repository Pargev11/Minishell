/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pargev <pargev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:30:58 by pargev            #+#    #+#             */
/*   Updated: 2025/12/07 23:47:00 by pargev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <unistd.h>
# include <stdint.h>
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
# include <fcntl.h>

typedef struct s_var_info
{
	char	*name;
	char	*value;
}	t_var_info;

typedef struct s_cmd
{
	char	*cmd;
	int		quotes;
}	t_cmd;

typedef struct s_cmds
{
	char	***cmds;
	int		**quote_mask;
	int		*stdin_fd;
}	t_cmds;

typedef struct s_minishell
{
	char	*cwd;
	t_list	**env_list;
	int		exit_code;
}	t_minishell;

typedef struct s_env_info
{
	char	*env_name;
	char	*env_value_name;
}	t_env_info;

typedef struct s_parsing_data
{
	int		i;
	int		start;
	t_list	**cmds;
	int		is_quotation;
	char	*word;
}	t_parsing_data;

//main
void		init(t_minishell *data);
void		end_program(t_minishell *data);

//signals
void		print_nl_handler(int sig);
void		interrupt_signal(int sig);
void		print_nl_handler_and_exit(int sig);

//parse
void		analize_command(char *command, t_minishell *data);
t_list		**parse_to_list(char *command, t_minishell *data);
t_cmd		*new_cmd(char *cmd, int quotes);
void		free_cmd(void *content);
t_cmd		*get_cmd(t_list	*cmds_list);
int			skip_spaces(char *str, int *i);
t_cmds		parse_words(char *cmd, t_minishell *data);
char		*subst_vars(char *cmd, t_minishell *data, t_list **cmds);
void		allocate_cmds(t_list **cmds_list, t_cmds *cmds);
t_cmd		*get_cmd(t_list	*cmds_list);
int			is_redirection(char *str);
void		remove_redirects(char ***cmds, int i, int **quote_mask);
int			parse_operators(char *command, int i, t_list **cmds,
				int is_quotation);

//built-ins
void		cd(char **cmds, t_minishell *data);
void		pwd(char **cmds, t_minishell *data);
void		exit_cmd(char **cmds, t_minishell *data);
void		echo(char **cmds, t_minishell *data);
void		export(char **cmds, t_minishell *data);
void		env(char **cmds, t_minishell *data);
void		unset(char **cmds, t_minishell *data);

//execution
void		exec(char **cmds, t_minishell *data);
void		execute_command(t_cmds *cmds, t_minishell *data);
void		execute_pipeline(t_cmds *cmds, int commands_count,
				t_minishell *data);
int			run_builtin(char **cmds, t_minishell *data);
int			handle_redirects(t_cmds *cmds, int i, int *fd_index);
int			handle_heredoc(char	*delimiter, int quotes, t_minishell *data);

//utils
int			ft_strcmp(const char *s1, const char *s2);
int			check_name(char *name);
int			get_arr_sz(char **arr_2d);
int			is_dir(char const *path);
void		free_arr(char **arr, char ***cmds);
void		free_mask(int **mask);

//envirement variables
t_var_info	*var_info(char *variable, t_minishell *data);
t_list		**env_to_list(void);
char		**list_to_env(t_minishell *data);
void		lst_add_sorted(t_list **lst, t_list *new);
t_var_info	*lst_content(t_list	*lst);
void		free_variable(void	*lst);
char		*get_varible(char *name, t_minishell *data);

#endif