#include "minishell.h"

typedef	enum
{
	MATCH_STATUS_FATAL_ERROR = 1,
	MATCH_STATUS_SYNTAX_ERROR,
	MATCH_STATUS_DOESNT_MATCH,
	MATCH_STATUS_MATCH,
}	t_match_status;

typedef	t_match_status	(*t_process_f_ptr)(char *str, size_t *i, t_list **tok_stream);

typedef	struct s_process_symbol_data
{
	const char	*str;
	size_t		*i;
	t_list		**tok_stream;
	const char	*symbol;
	t_tok_type	tok_type;
}	t_process_symbol_data;

void	skip_spaces(char *str, int *i)
{
	while (str[*i] && !ft_strchr(" \v\f\r\n\t", str[*i]))
		*i++;
}

void	free_token_node(t_tok_node *node)
{
	free(node->str);
	free(node);
}

t_tok_node	*ft_alloc_tok_node(t_tok_type type, char quote, char *str)
{
	t_tok_node	*tok_node;

	if (type == TOK_WORD && !str)
		return (NULL);
	tok_node = malloc(sizeof(*tok_node));
	if (!tok_node)
		return (NULL);
	tok_node->type = type;
	tok_node->quote = quote;
	if (str && type == TOK_WORD)
		tok_node->str = str;
	else
		tok_node->str = NULL;
	return (tok_node);
}

t_match_status	process_symbol_abstract(t_process_symbol_data *data)
{
	t_list		*node;
	t_tok_node	*content;
	size_t		sym_len;

	sym_len = ft_strlen(data->symbol);
	if (ft_strncmp(&(data->str[*data->i]), data->symbol, sym_len))
		return (MATCH_STATUS_DOESNT_MATCH);
	content = ft_alloc_tok_node(data->tok_type, NULL, NULL);
	if (!content)
		return (ft_lstclear(data->tok_stream, free_token_node), MATCH_STATUS_FATAL_ERROR);
	node = ft_lstnew(content);
	if (!node)
		return (free_token_node(content), ft_lstclear(data->tok_stream, free_token_node), MATCH_STATUS_FATAL_ERROR);
	ft_lstadd_back(data->tok_stream, node);
	*data->i += sym_len;
	return (MATCH_STATUS_MATCH);
}

t_match_status	process_output_append(char *str, size_t *i, t_list **tok_stream)
{
	t_process_symbol_data	data;
	
	data.str = str;
	data.i = i;
	data.tok_stream = tok_stream;
	data.symbol = ">>";
	data.tok_type = TOK_OUTPUT_APPEND;
	return (process_symbol_abstract(&data));
}

t_match_status	process_output(char *str, size_t *i, t_list **tok_stream)
{
	t_process_symbol_data	data;
	
	data.str = str;
	data.i = i;
	data.tok_stream = tok_stream;
	data.symbol = ">";
	data.tok_type = TOK_OUTPUT;
	return (process_symbol_abstract(&data));
}

t_match_status	process_heredoc(char *str, size_t *i, t_list **tok_stream)
{
	t_process_symbol_data	data;
	
	data.str = str;
	data.i = i;
	data.tok_stream = tok_stream;
	data.symbol = "<<";
	data.tok_type = TOK_HEREDOC;
	return (process_symbol_abstract(&data));
}

t_match_status	process_input(char *str, size_t *i, t_list **tok_stream)
{
	t_process_symbol_data	data;
	
	data.str = str;
	data.i = i;
	data.tok_stream = tok_stream;
	data.symbol = "<";
	data.tok_type = TOK_INPUT;
	return (process_symbol_abstract(&data));
}

t_match_status	process_pipe(char *str, size_t *i, t_list **tok_stream)
{
	t_process_symbol_data	data;
	
	data.str = str;
	data.i = i;
	data.tok_stream = tok_stream;
	data.symbol = "|";
	data.tok_type = TOK_PIPE;
	return (process_symbol_abstract(&data));
}

static void	count_word_len(char *str, char *quote, int *i, size_t *word_len)
{
	*word_len = 0;
	while (str[*i] && str[*i] != quote)
	{
		if (!quote && !ft_strchr(" \v\f\r\n\t\'\"", str[*i]))
			break ;
		*word_len++;
		*i++;
	}
}

t_match_status	process_word(char *str, size_t *i, t_list **tok_stream)
{
	t_list		*node;
	t_tok_node	*content;
	size_t		word_len;
	char		quote;

	quote = 0;
	if (ft_strchr("\'\"",str[*i]))
		quote = str[*i++];
	count_word_len(str, &quote, i, &word_len);
	if (!str[*i] && quote)
		return ((void)printf("syntax error: unclosed quote\n"), MATCH_STATUS_SYNTAX_ERROR);
	content = ft_alloc_tok_node(TOK_WORD, quote, ft_substr(str, *i - word_len, word_len));
	if (!content)
		return (ft_lstclear(tok_stream, free_token_node), MATCH_STATUS_FATAL_ERROR);
	node = ft_lstnew(content);
	if (!node)
		return (free_token_node(content), ft_lstclear(tok_stream, free_token_node), MATCH_STATUS_FATAL_ERROR);
	ft_lstadd_back(tok_stream, node);
	if (str[*i] == quote && quote)
		*i++;
	return (MATCH_STATUS_MATCH);
}

static void	init_vars(t_process_f_ptr *f_ptr, t_match_status *match_status, t_list **tok_stream, int *i)
{
	f_ptr[0] = process_output_append;
	f_ptr[1] = process_output;
	f_ptr[2] = process_heredoc;
	f_ptr[3] = process_input;
	f_ptr[4] = NULL;
	*match_status = NULL;
	*tok_stream = NULL;
	*i = 0;
}

t_list	*tokenize(const char *str, t_tok_status *tok_status)
{
	size_t						i;
	size_t						j;
	t_list						*tok_stream;
	t_match_status				match_status;
	t_process_f_ptr				f_arr[5];

	init_vars(f_arr, &match_status, &tok_stream, &i);
	while (str[i] && match_status != MATCH_STATUS_FATAL_ERROR
		&& match_status != MATCH_STATUS_SYNTAX_ERROR)
	{
		skip_spaces(str, &i);
		j = 0;
		while (f_arr[j])
		{
			match_status = f_arr[j](str, &i, &tok_stream);
			if (match_status != MATCH_STATUS_DOESNT_MATCH)
				break ;
			j++;
		}
	}
	if (match_status == MATCH_STATUS_SYNTAX_ERROR || match_status == MATCH_STATUS_FATAL_ERROR)
		*tok_status = match_status;
	else
		*tok_status = TOK_STATUS_OK;
	return (tok_stream);
}
