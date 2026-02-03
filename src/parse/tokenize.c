#include "minishell.h"

typedef	enum
{
	TOK_STATUS_OK = 0,
	TOK_STATUS_FATAL_ERROR,
	TOK_STATUS_SYNTAX_ERROR,
	TOK_STATUS_DOESNT_MATCH,
	TOK_STATUS_MATCH
}	t_tok_status;

typedef	t_tok_status	(*t_process_f_ptr)(char *str, size_t *i, t_list **tok_stream);


void	free_token_node(t_tok_node *node)
{

}


t_tok_status	process_output_append(char *str, size_t *i, t_list **tok_stream)
{
	t_list		*node;
	t_tok_node	*content;

	if (ft_strncmp(&str[*i], ">>", 2))
		return (TOK_STATUS_DOESNT_MATCH);
	content = malloc(sizeof(*content));
	if (!content)
	{
	// 	ft_lstclear();
		return (TOK_STATUS_FATAL_ERROR);
	}
	content->type = TOK_OUTPUT_APPEND;
	content->str = NULL;
	node = ft_lstnew(content);
	if (!node)
	{
		free(content);
		return (TOK_STATUS_FATAL_ERROR);
	}
	ft_lstadd_back(tok_stream, node);
	*i += 2;
	return (TOK_STATUS_MATCH);
}

t_tok_status	process_output(char *str, size_t *i, t_list **tok_stream)
{
	t_list		*node;
	t_tok_node	*content;

	if (str[*i] != '>')
		return (TOK_STATUS_DOESNT_MATCH);
	content = malloc(sizeof(*content));
	if (!content)
	{
	// 	ft_lstclear();
		return (TOK_STATUS_FATAL_ERROR);
	}
	content->type = TOK_OUTPUT;
	content->str = NULL;
	node = ft_lstnew(content);
	if (!node)
	{
		free(content);
		return (TOK_STATUS_FATAL_ERROR);
	}
	ft_lstadd_back(tok_stream, node);
	*i += 1;
	return (TOK_STATUS_MATCH);
}

t_tok_status	process_heredoc(char *str, size_t *i, t_list **tok_stream)
{
	t_list		*node;
	t_tok_node	*content;

	if (ft_strncmp(&str[*i], "<<", 2))
		return (TOK_STATUS_DOESNT_MATCH);
	content = malloc(sizeof(*content));
	if (!content)
	{
	// 	ft_lstclear();
		return (TOK_STATUS_FATAL_ERROR);
	}
	content->type = TOK_HEREDOC;
	content->str = NULL;
	node = ft_lstnew(content);
	if (!node)
	{
		free(content);
		return (TOK_STATUS_FATAL_ERROR);
	}
	ft_lstadd_back(tok_stream, node);
	*i += 2;
	return (TOK_STATUS_MATCH);
}

t_tok_status	process_input(char *str, size_t *i, t_list **tok_stream)
{
	t_list		*node;
	t_tok_node	*content;

	if (str[*i] != '<')
		return (TOK_STATUS_DOESNT_MATCH);
	content = malloc(sizeof(*content));
	if (!content)
	{
	// 	ft_lstclear();
		return (TOK_STATUS_FATAL_ERROR);
	}
	content->type = TOK_INPUT;
	content->str = NULL;
	node = ft_lstnew(content);
	if (!node)
	{
		free(content);
		return (TOK_STATUS_FATAL_ERROR);
	}
	ft_lstadd_back(tok_stream, node);
	*i += 1;
	return (TOK_STATUS_MATCH);
}

t_tok_status	process_pipe(char *str, size_t *i, t_list **tok_stream)
{
	t_list		*node;
	t_tok_node	*content;

	if (str[*i] != '|')
		return (TOK_STATUS_DOESNT_MATCH);
	content = malloc(sizeof(*content));
	if (!content)
	{
	// 	ft_lstclear();
		return (TOK_STATUS_FATAL_ERROR);
	}
	content->type = TOK_PIPE;
	content->str = NULL;
	node = ft_lstnew(content);
	if (!node)
	{
		free(content);
		return (TOK_STATUS_FATAL_ERROR);
	}
	ft_lstadd_back(tok_stream, node);
	*i += 1;
	return (TOK_STATUS_MATCH);
}

t_tok_status	process_word(char *str, size_t *i, t_list **tok_stream)
{
	t_list		*node;
	t_tok_node	*content;
	size_t		word_len;
	char		quote;

	word_len = 0;
	quote = 0;
	if (ft_strchr("\'\"",str[*i]))
	{
		quote = str[*i];
		*i++;
	}
	while (str[*i] && str[*i] != quote)
	{
		if (!quote && !ft_strchr(" \v\f\r\n\t", str[*i]))
			break ;
		word_len++;
		i++;
	}
	if (!str[*i] && quote)
	{
		printf("unclosed quote");
		return (TOK_STATUS_FATAL_ERROR)
	}
	content = malloc(sizeof(*content));
}


// fill_in_f_arr()
// {
// 	int	(*)arr[6]
// }

void	skip_spaces(char *str, int *i)
{
	while (!quote && str[i] && !ft_strchr(" \v\f\r\n\t", str[i]))
		i++;
}

t_list	**tokenize(const char *str, t_tok_status *status)
{
	size_t						i;
	size_t						j;
	t_list						*tok_stream;
	bool						in_word;
	char						quote;
	t_tok_status				status;
	static t_process_f_ptr		f_arr[] = {process_output_append,
											process_output,
											process_heredoc,
											process_input,
											NULL};

	quote = 0;
	in_word = false;
	i = 0;
	while (str[i])
	{
		while (!quote && str[i] && !ft_strchr(" \v\f\r\n\t", str[i]))
			i++;
		j = 0;
		while (f_arr[j])
		{
			status = f_arr[j](str, &i, &tok_stream);
			if (status == TOK_STATUS_FATAL_ERROR)
				return (NULL);
			else if (status == TOK_STATUS_MATCH)
				break ;
			else if (status == TOK_STATUS_DOESNT_MATCH)
			{
				j++;
				continue ;
			}
			else

		}




		if (process_output_append(str, &i, &tok_stream) == 1)
			return (NULL);

		if (!ft_strncmp(&str[i], ">>", 2))
		{
			//add append token
		}
		else if (str[i] == '>')
		{
			// etc
		}
		else if (!ft_strncmp(&str[i], "<<", 2))
		{

		}
		else if (str[i] == '<')
		{

		}
		else
		
		
		
	}
	*status = TOK_STATUS_OK;
}
