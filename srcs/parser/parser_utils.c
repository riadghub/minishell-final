/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reeer-aa <reeer-aa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 01:13:00 by gekido            #+#    #+#             */
/*   Updated: 2025/06/17 12:29:46 by reeer-aa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_word_tokens(t_token *token)
{
	int	count;

	count = 0;
	while (token && token->type != TOKEN_PIPE)
	{
		if (token->type == TOKEN_WORD && token->value
			&& ft_strlen(token->value) > 0)
			count++;
		else if (is_redirection(token->type))
		{
			token = token->next;
			if (token && token->type == TOKEN_WORD)
				token = token->next;
			continue ;
		}
		token = token->next;
	}
	return (count);
}

char	**extract_args(t_token **token, int count)
{
	return (extract_args_safe(token, count));
}

t_redir	*append_redirections(t_redir *list, t_redir *new)
{
	t_redir	*current;

	if (!list)
		return (new);
	if (!new)
		return (list);
	current = list;
	while (current->next)
		current = current->next;
	current->next = new;
	return (list);
}

void	print_syntax_error(t_token *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (token && token->value)
		ft_putstr_fd(token->value, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putendl_fd("'", 2);
	g_signal_status = 2;
}

int	check_redirection_syntax(t_token *redir_token)
{
	if (!redir_token->next)
	{
		print_syntax_error(NULL);
		return (1);
	}
	if (redir_token->type == TOKEN_HEREDOC
		&& is_redirection(redir_token->next->type))
	{
		print_syntax_error(redir_token->next);
		return (1);
	}
	if (redir_token->next->type != TOKEN_WORD)
	{
		print_syntax_error(redir_token->next);
		return (1);
	}
	return (0);
}

int	has_redirection_tokens(t_token *token)
{
	while (token && token->type != TOKEN_PIPE)
	{
		if (is_redirection(token->type))
			return (1);
		token = token->next;
	}
	return (0);
}

t_redir	*create_and_append_redirection(t_redir *redirects, t_token *token)
{
	t_redir	*new_redir;

	new_redir = create_redirection(token->type, token->next->value);
	if (!new_redir)
		return (NULL);
	return (append_redirections(redirects, new_redir));
}

t_redir	*parse_redirections(t_token **token)
{
	t_redir	*redirects;

	redirects = NULL;
	while (*token && (*token)->type != TOKEN_PIPE)
	{
		if (is_redirection((*token)->type))
		{
			if (check_redirection_syntax(*token))
				return (free_redirections(redirects), NULL);
			redirects = create_and_append_redirection(redirects, *token);
			if (!redirects)
				return (free_redirections(redirects), NULL);
			*token = (*token)->next->next;
		}
		else
			*token = (*token)->next;
	}
	return (redirects);
}
