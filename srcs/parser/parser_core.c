/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_core.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reeer-aa <reeer-aa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:10:00 by gekido            #+#    #+#             */
/*   Updated: 2025/06/18 13:54:56 by reeer-aa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_pipe_token(t_token *token)
{
	return (token && token->type == TOKEN_PIPE);
}

void	free_ast(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->left)
		free_ast(node->left);
	if (node->right)
		free_ast(node->right);
	if (node->args)
		free_args(node->args);
	if (node->redirects)
		free_redirections(node->redirects);
	free(node);
}

void	skip_to_next_token(t_token **token, int count)
{
	int	i;

	i = 0;
	while (i < count && *token)
	{
		*token = (*token)->next;
		i++;
	}
}

t_ast_node	*parse_command(t_token **token)
{
	int		word_count;
	char	**args;
	t_redir	*redirects;
	t_token	*temp_token;

	word_count = count_word_tokens(*token);
	if (word_count == 0 && !is_redirection((*token)->type))
		return (NULL);
	temp_token = *token;
	args = extract_args(token, word_count);
	if (!args && word_count > 0)
		return (NULL);
	*token = temp_token;
	redirects = parse_redirections(token);
	if (redirects == NULL && has_redirection_tokens(temp_token))
	{
		if (args)
			free_args(args);
		return (NULL);
	}
	return (create_command_node(args, redirects));
}

t_ast_node	*parser(t_token *tokens)
{
	t_ast_node	*left;
	t_ast_node	*right;

	if (!tokens)
		return (NULL);
	left = parse_command(&tokens);
	if (!left)
		return (NULL);
	if (is_pipe_token(tokens))
	{
		skip_to_next_token(&tokens, 1);
		if (!tokens)
		{
			print_syntax_error(NULL);
			free_ast(left);
			return (NULL);
		}
		right = parser(tokens);
		if (!right)
		{
			print_syntax_error(NULL);
			free_ast(left);
			return (NULL);
		}
		return (create_pipe_node(left, right));
	}
	return (left);
}
