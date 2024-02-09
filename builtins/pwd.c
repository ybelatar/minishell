/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 15:46:09 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/08 09:44:39 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(char **args, t_minishell *minishell)
{
	char	buf[2048];
	int		len;

	(void)args;
	(void)minishell;
	if (!getcwd(buf, 2048))
		return (ft_dprintf(2, "minishell: pwd: %s\n", strerror(errno)), 1);
	len = ft_strlen(buf);
	if (write(1, buf, len) < len)
		return (ft_dprintf(2, "minishell: pwd: write error: %s\n",
				strerror(errno)), 125);
	if (!write(1, "\n", 1))
		return (ft_dprintf(2, "minishell: pwd: write error: %s\n",
				strerror(errno)), 125);
	return (0);
}
