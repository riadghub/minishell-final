/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reeer-aa <reeer-aa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:15:10 by gekido            #+#    #+#             */
/*   Updated: 2025/06/17 10:45:51 by reeer-aa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	should_exit(void)
{
	if (g_signal_status >= 256)
		return (1);
	return (0);
}

int	get_exit_code(void)
{
	return (g_signal_status % 256);
}

void	close_fd(int fd1, int fd2)
{
	if (fd1 != -1)
		close(fd1);
	if (fd2 != -1)
		close(fd2);
}

void	handle_direct_path_error(char *cmd)
{
	struct stat	path_stat;

	if (access(cmd, F_OK) == 0)
	{
		if (stat(cmd, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putendl_fd(": Is a directory", 2);
			g_signal_status = 126;
		}
		else
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putendl_fd(": Permission denied", 2);
			g_signal_status = 126;
		}
	}
	else
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": No such file or directory", 2);
		g_signal_status = 127;
	}
}

int	is_unknown_cmd(t_token *tokens, t_env *env)
{
	char	*path;
	char	*cmd;

	cmd = tokens->value;
	path = find_path(cmd, env->vars);
	if (!path)
	{
		if (ft_strchr(cmd, '/') || ft_strncmp(cmd, "./", 2) == 0)
			handle_direct_path_error(cmd);
		else
		{
			ft_putstr_fd("minishell: command not found: ", 2);
			ft_putendl_fd(cmd, 2);
			g_signal_status = 127;
		}
		return (1);
	}
	free(path);
	return (0);
}
