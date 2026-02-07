#include "minishell.h"

bool	is_meta_symbol(const char *str)
{
	static char	*symbols[] = {
		"<<",
		">>",
		"<",
		">",
		"|",
		"(",
		")",
		NULL
	};
	size_t	i;

	i = 0;
	while (symbols[i])
	{
		if (!ft_strncmp(symbols[i], str, ft_strlen(symbols[i])))
			return (true);
		i++;
	}
	return (false);
}

bool	is_whitespace(char c)
{
	if (ft_strchr(" \v\f\r\n\t", c))
		return (true);
	return (false);
}

void	skip_spaces(const char *str, size_t *i)
{
	while (str[*i] && ft_strchr(" \v\f\r\n\t", str[*i]))
		(*i)++;
}

void	del_word_segment(void *void_param)
{
	t_word_segment	*word_segment;

	word_segment = (t_word_segment *)void_param;
	free(word_segment->segment);
	free(word_segment);
}

void	free_token_node(void *void_param)
{
	t_tok_node	*node;

	node = (t_tok_node*)void_param;
	ft_lstclear(&(node->word_segments), del_word_segment);
	free(node);
}

t_list	*ft_alloc_tok_node(t_tok_type type, t_list	*word_segments)
{
	t_tok_node	*tok_node;
	t_list		*list_node;

	if (type == TOK_WORD && !word_segments)
		return (NULL);
	tok_node = malloc(sizeof(*tok_node));
	if (!tok_node)
		return (NULL);
	tok_node->type = type;
	if (word_segments && type == TOK_WORD)
		tok_node->word_segments = word_segments;
	else
		tok_node->word_segments = NULL;
	list_node = ft_lstnew(tok_node);
	if (!list_node)
	{
		free_token_node(tok_node);
	}
	return (list_node);
}

t_list	*ft_alloc_word_segment_node(char quote, bool is_subshell_subst, char *segment)
{
	t_word_segment	*word_segment_node;
	t_list			*list_node;

	if (!segment)
		return (NULL);
	word_segment_node = malloc(sizeof(*word_segment_node));
	if (!word_segment_node)
		return (free(segment), NULL);
	word_segment_node->quote = quote;
	word_segment_node->is_subshell_subst = is_subshell_subst;
	word_segment_node->segment = segment;
	list_node = ft_lstnew(word_segment_node);
	if (!list_node)
		return (del_word_segment(word_segment_node), NULL);
	return (list_node);
}

t_match_status	process_symbol_abstract(const char *str, size_t *i, t_list **tok_stream, t_symbol_map *symbol_map)
{
	t_list		*node;
	size_t		sym_len;

	sym_len = ft_strlen(symbol_map->symbol);
	if (ft_strncmp(&(str[*i]), symbol_map->symbol, sym_len))
		return (MATCH_STATUS_DOESNT_MATCH);
	node = ft_alloc_tok_node(symbol_map->type, NULL);
	if (!node)
		return (ft_lstclear(tok_stream, free_token_node), MATCH_STATUS_FATAL_ERROR);
	ft_lstadd_back(tok_stream, node);
	*i += sym_len;
	return (MATCH_STATUS_MATCH);
}

void	skip_quoted(const char *str, size_t *i)
{
	char	quote;

	quote = str[*i];
	(*i)++;
	while(str[*i] && str[*i] != quote)
		(*i)++;
}

void	process_subshell_subst_pt2(const char *str, int *par_depth, size_t *i)
{
	char	quote;

	quote = 0;
	while (str[*i])
	{
		if (!quote && ft_strchr("\'\"", str[*i]))
		{
			quote = str[*i];
			skip_quoted(str, i);
		}
		if (quote && str[*i] == quote)
			quote = 0;
		else if (!quote && (str[*i] == '(' || !ft_strncmp(&str[*i], "$(", 2)))
			(*par_depth)++;
		else if (!quote && (str[*i] == ')'))
		{
			(*par_depth)--;
			if (*par_depth == 0)
				break ;
		}
		(*i)++;
	}
}

t_match_status	process_subshell_subst(const char *str, size_t *i, t_list **word_segments)
{
	int				par_depth;
	size_t			subsh_str_start;
	t_list			*word_segment_node;

	par_depth = 1;
	*i += 2;
	subsh_str_start = *i;
	process_subshell_subst_pt2(str, &par_depth, i);
	if (!str[*i] && par_depth > 0)
		return ((void)printf("there's unclosed subshell command parenthesis\n"), MATCH_STATUS_SYNTAX_ERROR);
	word_segment_node = ft_alloc_word_segment_node(0, true, ft_substr(str, subsh_str_start, (*i)++ - subsh_str_start));
	if (!word_segment_node)
		return (ft_lstclear(word_segments, del_word_segment), MATCH_STATUS_FATAL_ERROR);
	ft_lstadd_back(word_segments, word_segment_node);
	return (MATCH_STATUS_MATCH);
}

bool	count_word_segment_len(const char *str, char *quote, size_t *i, size_t *word_segment_len)
{
	*word_segment_len = 0;
	while (str[*i] && str[*i] != *quote)
	{
		if ((!*quote && (is_whitespace(str[*i]) || ft_strchr("\'\"", str[*i])))
			|| ((*quote == '\"' || !*quote) && !ft_strncmp("$(", &str[*i], 2))
			|| (*quote && str[*i] == *quote))
			break ;
		(*word_segment_len)++;
		(*i)++;
	}
	if (!str[*i] && *quote)
		return ((void)printf("syntax error: unclosed quote\n"), false);
	return (true);
}

t_match_status	process_word_segment(const char *str, size_t *i, t_list **word_segments)
{
	t_list			*word_segment_node;
	size_t			seg_len;
	char			quote;
	t_match_status	match_status;

	quote = 0;
	if (!ft_strncmp("$(", &str[*i], 2))

		match_status = process_subshell_subst(str, i, word_segments);
	else
	{
		if (ft_strchr("\'\"", str[*i]))
			quote = str[(*i)++];
		if (!count_word_segment_len(str, &quote, i, &seg_len))
			return (MATCH_STATUS_SYNTAX_ERROR);
		word_segment_node = ft_alloc_word_segment_node(quote, false, ft_substr(str, *i - seg_len, seg_len));
		if (!word_segment_node)
			return (ft_lstclear(word_segments, del_word_segment), MATCH_STATUS_FATAL_ERROR);
		ft_lstadd_back(word_segments, word_segment_node);
		if (str[*i] == quote && quote)
			(*i)++;
		match_status = MATCH_STATUS_MATCH;
	}
	return (match_status);
}

t_match_status	process_word(const char *str, size_t *i, t_list **tok_stream)
{
	t_list			*tok_node;
	t_list			*word_segments;
	t_match_status	match_status;

	word_segments = NULL;
	while (str[*i] && !is_whitespace(str[*i]) && !is_meta_symbol(&(str[*i])))
	{
		match_status = process_word_segment(str, i, &word_segments);
		if (match_status == MATCH_STATUS_SYNTAX_ERROR
			|| match_status == MATCH_STATUS_FATAL_ERROR)
			return (ft_lstclear(tok_stream, free_token_node), match_status);
	}
	tok_node = ft_alloc_tok_node(TOK_WORD, word_segments);
	if (!tok_node)
		return (ft_lstclear(tok_stream, free_token_node), MATCH_STATUS_FATAL_ERROR);
	ft_lstadd_back(tok_stream, tok_node);
	return (match_status);
}

static void	init_vars(t_symbol_map **symbol_map_to_init, t_match_status *match_status, t_list **tok_stream, size_t *i)
{
	static t_symbol_map sym_map[] = {
		{"$(", TOK_SUBST_OPEN},
		{">>", TOK_OUTPUT_APPEND},
		{"<<", TOK_HEREDOC},
		{">",  TOK_OUTPUT},
		{"<",  TOK_INPUT},
		{"|",  TOK_PIPE},
		{"(",  TOK_L_PAREN},
		{")",  TOK_R_PAREN},
		{NULL, 0}
		};

	*match_status = 0;
	*tok_stream = NULL;
	*i = 0;
	*symbol_map_to_init = sym_map;
}

t_match_status	do_match(const char *str, size_t *i, t_list **tok_stream, t_symbol_map *symbol_map)
{
	size_t			j;
	t_match_status	match_status;

	j = 0;
	while (symbol_map[j].symbol)
	{
		match_status = process_symbol_abstract(str, i, tok_stream, &(symbol_map[j]));
		if (match_status != MATCH_STATUS_DOESNT_MATCH)
			break ;
		j++;
	}
	if (match_status == MATCH_STATUS_DOESNT_MATCH)
		match_status = process_word(str, i, tok_stream);
	if (match_status == MATCH_STATUS_DOESNT_MATCH)
		match_status = MATCH_STATUS_SYNTAX_ERROR;
	return (match_status);
}

t_list	*tokenize(const char *str, t_tok_status *tok_status)
{
	size_t						i;
	t_list						*tok_stream;
	t_match_status				match_status;
	t_symbol_map				*symbol_map;

	init_vars(&symbol_map, &match_status, &tok_stream, &i);
	while (str[i] && match_status != MATCH_STATUS_FATAL_ERROR
		&& match_status != MATCH_STATUS_SYNTAX_ERROR)
	{
		skip_spaces(str, &i);
		match_status = do_match(str, &i, &tok_stream, symbol_map);
	}
	if (match_status == MATCH_STATUS_SYNTAX_ERROR || match_status == MATCH_STATUS_FATAL_ERROR)
		*tok_status = (t_tok_status)match_status;
	else
		*tok_status = TOK_STATUS_OK;
	return (tok_stream);
}

//TODO: May want to add failsafe syntax error case if all of the matchers didn't match
// also need to implement conjunction of adjacent words in quotes like ["abc"def]

// "(" and ")" are plain tokens, "$(" enables special mode of str consumtion with nested depth tracking.
//n.b. if ("(" || "$(") depth++; else if (")") depth--; if (depth == 0) -> we've finished with $( word subsegment
//Important! [(...)] is a COMMAND node (stream of tokens on parsing stage) (supports redirects as any command) and [$(...)] is a word segment (raw string)

//refactor matching loop with something like:
// typedef struct s_symbol_map {
//     char        *symbol;
//     t_tok_type  type;
// } t_symbol_map;
// static const t_symbol_map symbols_map[] = {
//     {"$(", TOK_SUBST_OPEN},
//     {">>", TOK_OUTPUT_APPEND},
//     {"<<", TOK_HEREDOC},
//     {">",  TOK_OUTPUT},
//     {"<",  TOK_INPUT},
//     {"|",  TOK_PIPE},
//     {"(",  TOK_L_PAREN},
//     {")",  TOK_R_PAREN},
//     {NULL, 0}
// };