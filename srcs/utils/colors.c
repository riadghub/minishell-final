/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reeer-aa <reeer-aa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:28:45 by reeer-aa          #+#    #+#             */
/*   Updated: 2025/06/19 13:28:51 by reeer-aa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/colors.h"

char	*get_username(void)
{
	char	*user;

	user = getenv("USER");
	if (!user)
		user = "user";
	return (user);
}

char	*get_current_dir(void)
{
	char *pwd;
	char *home;
	char *result;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (ft_strdup("~"));
	home = getenv("HOME");
	if (home && ft_strncmp(pwd, home, ft_strlen(home)) == 0)
	{
		result = ft_strjoin("~", pwd + ft_strlen(home));
		free(pwd);
		return (result);
	}
	return (pwd);
}
