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
# include <stdint.h>
# include <stdbool.h>

typedef enum tok_type
{
	TOK_WORD,
	TOK_PIPE,
	TOK_INPUT,
	TOK_HEREDOC,
	TOK_OUTPUT_APPEND,
	TOK_OUTPUT,
	TOK_L_PAREN,
	TOK_R_PAREN,
	TOK_SUBST_OPEN
}	t_tok_type;

typedef	struct s_symbol_map
{
    char        *symbol;
    t_tok_type  type;
}	t_symbol_map;

typedef	enum
{
	MATCH_STATUS_FATAL_ERROR = 1,
	MATCH_STATUS_SYNTAX_ERROR,
	MATCH_STATUS_DOESNT_MATCH,
	MATCH_STATUS_MATCH
}	t_match_status;

typedef	enum
{
	TOK_STATUS_OK = 0,
	TOK_STATUS_FATAL_ERROR,
	TOK_STATUS_SYNTAX_ERROR
}	t_tok_status;

typedef struct s_var_info
{
	char	*name;
	char	*value;
}	t_var_info;

typedef struct s_minishell
{
	char	*cwd;
	t_list	**env_list;
	int		exit_code;
}	t_minishell;

typedef	struct s_word_segment
{
	char	quote;
	bool	is_subshell_subst;
	char	*segment;
}	t_word_segment;


typedef	struct s_tok_node
{
	t_tok_type	type;
	t_list		*word_segments;
}	t_tok_node;


// typedef	struct s_ast
// {
// 	void	*left;
// 	void	*right;
// }

//main
void		init(t_minishell *data);
void		end_program(t_minishell *data);

//signals
void		print_nl_handler(int sig);
void		print_nl_handler_sigquit(int sig);
void		interrupt_signal(int sig);
void		print_nl_handler_and_exit(int sig);

void	analize_command(char *command, t_minishell *data);

//tokenise
t_list	*tokenize(const char *str, t_tok_status *tok_status);

//envirement variables
t_var_info	*var_info(char *variable, t_minishell *data);
t_list		**env_to_list(void);
char		**list_to_env(t_minishell *data);
void		lst_add_sorted(t_list **lst, t_list *new);
t_var_info	*lst_content(t_list	*lst);
void		free_variable(void	*lst);
char		*get_varible(char *name, t_minishell *data);

//utils
int			ft_strcmp(const char *s1, const char *s2);

#endif