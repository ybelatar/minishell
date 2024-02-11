/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 17:27:07 by pibosc            #+#    #+#             */
/*   Updated: 2024/02/11 21:47:13 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	ft_strncmp(const char *s1, const char *s2, size_t n)
// {
// 	size_t	i;

// 	i = 0;
// 	while ((unsigned char)s1[i]
// 		&& (unsigned char)s1[i] == (unsigned char)s2[i] && i < n)
// 		i++;
// 	if (i < n)
// 		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
// 	return (0);
// }

void	ft_close(int fd)
{
	if (fd > 2)
		close(fd);
}

char	ft_get_last_char(const char *str)
{
	while (*(str) && *(str + 1))
		++str;
	return (*str);
}

static inline void	ft_cwd(t_minishell *minishell, char *cwd, char *pwd,
		char *s)
{
	char	c[2];

	pwd = ft_strchrr(cwd, '/');
	if (!pwd)
	{
		if (g_status)
			minishell->cmd_line = readline(PROMPT_ME_S);
		else
			minishell->cmd_line = readline(PROMPT_ME_NS);
	}
	c[0] = '~';
	c[1] = 0;
	if (get_env("HOME", minishell->env) && !ft_strcmp(pwd,
			ft_strchrr(get_env("HOME", minishell->env), '/')))
		pwd = c;
	if (g_status)
		ft_sprintf(s, "\1\e[1;91m\xE2\x9E\2\x9C  \1\e[1;96m\2%s\1\e[0m\2 ",
			pwd);
	else
		ft_sprintf(s, "\1\e[1;92m\xE2\x9E\2\x9C  \1\e[1;96m\2%s\1\e[0m\2 ",
			pwd);
	minishell->cmd_line = readline(s);
}

static inline void	ft_pwd(t_minishell *minishell, char *pwd,
		char *s)
{
	char	c[2];

	pwd = ft_strchrr(pwd, '/');
	if (!pwd)
	{
		if (g_status)
			minishell->cmd_line = readline(PROMPT_ME_S);
		else
			minishell->cmd_line = readline(PROMPT_ME_NS);
	}
	c[0] = '~';
	c[1] = 0;
	if (get_env("HOME", minishell->env) && !ft_strcmp(pwd,
			ft_strchrr(get_env("HOME", minishell->env), '/')))
		pwd = c;
	if (g_status)
		ft_sprintf(s, "\1\e[1;91m\xE2\x9E\2\x9C  \1\e[1;96m\2%s\1\e[0m\2 ",
			pwd);
	else
		ft_sprintf(s, "\1\e[1;92m\xE2\x9E\2\x9C  \1\e[1;96m\2%s\1\e[0m\2 ",
			pwd);
	minishell->cmd_line = readline(s);
}

/*	Creates a prompt from the last exit code and the current working directory.
	Takes a pointer to a t_minishell struct to set its prompt variable.
	The prompt is allocated so it needs to be freed after its use.*/
void	ft_prompt(t_minishell *minishell)
{
	char	cwd[PATH_MAX];
	char	s[PATH_MAX + 34];
	char	*pwd;

	pwd = 0;
	cwd[0] = 0;
	getcwd(cwd, PATH_MAX);
	if (*cwd)
	{
		ft_cwd(minishell, cwd, pwd, s);
		return ;
	}
	pwd = get_env("PWD", minishell->env);
	if (pwd)
	{
		ft_pwd(minishell, pwd, s);
		return ;
	}
	if (g_status)
		minishell->cmd_line = readline(PROMPT_ME_S);
	else
		minishell->cmd_line = readline(PROMPT_ME_NS);
}
