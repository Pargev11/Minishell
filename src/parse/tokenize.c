#include "minishell.h"

typedef	enum
{
	MATCH_STATUS_FATAL_ERROR = 1,
	MATCH_STATUS_SYNTAX_ERROR,
	MATCH_STATUS_DOESNT_MATCH,
	MATCH_STATUS_MATCH,
}	t_match_status;

typedef	t_match_status	(*t_process_f_ptr)(const char *str, size_t *i, t_list **tok_stream);

typedef	struct s_process_symbol_data
{
	const char	*str;
	size_t		*i;
	t_list		**tok_stream;
	const char	*symbol;
	t_tok_type	tok_type;
}	t_process_symbol_data;

bool	is_meta_symbol(char *str)
{
	static char	*str[] = {
		"<<",
		">>",
		"<",
		">",
		"|",
		"(",
		")"
	};


}

void	skip_spaces(const char *str, size_t *i)
{
	// printf("skipping spaces, i is [%lu]\n", *i);
	while (str[*i] && ft_strchr(" \v\f\r\n\t", str[*i]))
		(*i)++;
	// printf("skipped spaces, i is [%lu]\n", *i);
}

void	free_token_node(void *void_param)
{
	t_tok_node	*node;

	node = (t_tok_node*)void_param;
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
	{
		tok_node->str = str;
		// printf("we're adding [%s] word to the word node\n", str);
	}
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
	content = ft_alloc_tok_node(data->tok_type, 0, NULL);
	if (!content)
		return (ft_lstclear(data->tok_stream, free_token_node), MATCH_STATUS_FATAL_ERROR);
	node = ft_lstnew(content);
	if (!node)
		return (free_token_node(content), ft_lstclear(data->tok_stream, free_token_node), MATCH_STATUS_FATAL_ERROR);
	ft_lstadd_back(data->tok_stream, node);
	*data->i += sym_len;
	return (MATCH_STATUS_MATCH);
}

t_match_status	process_output_append(const char *str, size_t *i, t_list **tok_stream)
{
	t_process_symbol_data	data;

	data.str = str;
	data.i = i;
	data.tok_stream = tok_stream;
	data.symbol = ">>";
	data.tok_type = TOK_OUTPUT_APPEND;
	return (process_symbol_abstract(&data));
}

t_match_status	process_output(const char *str, size_t *i, t_list **tok_stream)
{
	t_process_symbol_data	data;

	data.str = str;
	data.i = i;
	data.tok_stream = tok_stream;
	data.symbol = ">";
	data.tok_type = TOK_OUTPUT;
	return (process_symbol_abstract(&data));
}

t_match_status	process_heredoc(const char *str, size_t *i, t_list **tok_stream)
{
	t_process_symbol_data	data;
	// printf("just called process_heredoc, i is [%lu]\n", *i);
	data.str = str;
	data.i = i;
	data.tok_stream = tok_stream;
	data.symbol = "<<";
	data.tok_type = TOK_HEREDOC;
	return (process_symbol_abstract(&data));
}

t_match_status	process_input(const char *str, size_t *i, t_list **tok_stream)
{
	t_process_symbol_data	data;
	// printf("just called process_input, i is [%lu]\n", *i);
	data.str = str;
	data.i = i;
	data.tok_stream = tok_stream;
	data.symbol = "<";
	data.tok_type = TOK_INPUT;
	return (process_symbol_abstract(&data));
}

t_match_status	process_pipe(const char *str, size_t *i, t_list **tok_stream)
{
	t_process_symbol_data	data;
	// printf("just called process_pipe, i is [%lu]\n", *i);
	data.str = str;
	data.i = i;
	data.tok_stream = tok_stream;
	data.symbol = "|";
	data.tok_type = TOK_PIPE;
	return (process_symbol_abstract(&data));
}

static void	count_word_len(const char *str, char *quote, size_t *i, size_t *word_len)
{
	*word_len = 0;
	while (str[*i] && str[*i] != *quote)
	{
		if (!*quote && ft_strchr(" \v\f\r\n\t\'\"", str[*i]))
			break ;
		(*word_len)++;
		(*i)++;
	}
}

t_match_status	process_word(const char *str, size_t *i, t_list **tok_stream)
{
	t_list		*node;
	t_tok_node	*content;
	size_t		word_len;
	char		quote;

	// printf("just called process_word\n");
	quote = 0;
	if (ft_strchr("\'\"", str[*i]))
		quote = str[(*i)++];
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
		(*i)++;
	return (MATCH_STATUS_MATCH);
}

static void	init_vars(t_process_f_ptr *f_ptr, t_match_status *match_status, t_list **tok_stream, size_t *i)
{
	f_ptr[0] = process_output_append;
	f_ptr[1] = process_output;
	f_ptr[2] = process_heredoc;
	f_ptr[3] = process_input;
	f_ptr[4] = process_pipe;
	f_ptr[5] = process_word;
	f_ptr[6] = NULL;
	*match_status = 0;
	*tok_stream = NULL;
	*i = 0;
}
//TODO: May want to add failsafe syntax error case if all of the matchers didn't match
// also need to implement conjunction of adjacent words in quotes like ["abc"def]

// "(" and ")" are plain tokens, "$(" enables special mode of str consumtion with nested depth tracking.
//n.b. if ("(" || "$(") depth++; else if (")") depth--; if (depth == 0) -> we've finished with $( word subsegment
//Important! [(...)] is a COMMAND node (stream of tokens on parsing stage) (supports redirects as any command) and [$(...)] is a word segment (raw string)

//refactor mathing loop with something like:
// typedef struct s_symbol_map {
//     char        *symbol;
//     t_tok_type  type;
// } t_symbol_map;
// static const t_symbol_map g_symbols[] = {
//     {"$(", TOK_SUBST_OPEN}, // Our new friend!
//     {">>", TOK_OUTPUT_APPEND},
//     {"<<", TOK_HEREDOC},
//     {">",  TOK_OUTPUT},
//     {"<",  TOK_INPUT},
//     {"|",  TOK_PIPE},
//     {"(",  TOK_L_PAREN},
//     {")",  TOK_R_PAREN},
//     {NULL, 0}
// };

t_list	*tokenize(const char *str, t_tok_status *tok_status)
{
	size_t						i;
	size_t						j;
	t_list						*tok_stream;
	t_match_status				match_status;
	t_process_f_ptr				f_arr[7];

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
		if (match_status == MATCH_STATUS_DOESNT_MATCH)
			match_status == MATCH_STATUS_SYNTAX_ERROR;
		// printf("we'r looping-zaluping\n");
		// printf("i is [%lu]\n", i);
	}
	if (match_status == MATCH_STATUS_SYNTAX_ERROR || match_status == MATCH_STATUS_FATAL_ERROR)
		*tok_status = (t_tok_status)match_status;
	else
		*tok_status = TOK_STATUS_OK;
	return (tok_stream);
}
