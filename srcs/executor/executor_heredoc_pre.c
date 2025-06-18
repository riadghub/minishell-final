/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc_pre.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reeer-aa <reeer-aa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 10:16:04 by reeer-aa          #+#    #+#             */
/*   Updated: 2025/06/18 10:16:16 by reeer-aa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*generate_temp_filename(void)
{
	static int	counter = 0;
	char		*filename;
	char		*num_str;

	num_str = ft_itoa(counter++);
	if (!num_str)
		return (NULL);
	filename = ft_strjoin("/tmp/minishell_heredoc_", num_str);
	free(num_str);
	return (filename);
}

static int	process_single_heredoc(t_redir *redir)
{
	char	*line;
	char	*temp_file;
	int		fd;

	temp_file = generate_temp_filename();
	if (!temp_file)
		return (1);
	fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
	{
		free(temp_file);
		return (1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, redir->file) == 0)
		{
			if (line)
				free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	free(redir->file);
	redir->file = temp_file;
	redir->type = TOKEN_REDIR_IN;
	return (0);
}

int	preprocess_heredocs_in_node(t_ast_node *node)
{
	t_redir	*redir;

	if (!node)
		return (0);
	if (node->type == NODE_COMMAND && node->redirects)
	{
		redir = node->redirects;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC)
			{
				if (process_single_heredoc(redir) != 0)
					return (1);
			}
			redir = redir->next;
		}
	}
	if (node->left && preprocess_heredocs_in_node(node->left) != 0)
		return (1);
	if (node->right && preprocess_heredocs_in_node(node->right) != 0)
		return (1);
	return (0);
}

void	cleanup_temp_files(t_ast_node *node)
{
	t_redir	*redir;

	if (!node)
		return ;
	if (node->type == NODE_COMMAND && node->redirects)
	{
		redir = node->redirects;
		while (redir)
		{
			if (redir->file && ft_strncmp(redir->file,
					"/tmp/minishell_heredoc_", 23) == 0)
				unlink(redir->file);
			redir = redir->next;
		}
	}
	if (node->left)
		cleanup_temp_files(node->left);
	if (node->right)
		cleanup_temp_files(node->right);
}
