/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 07:01:39 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/11 05:15:04 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	a(int *i, int *j)
// {
// 	(*i)++;
// 	(*j)++;
// }
/*TODO utiliser indic*/
// int	match_core(char *str, char *pattern, char *indic, int f[4])
// {
// 	// ft_dprintf(2, "STR : %s ,INDIC : %s\n", str, indic);
// 	while (str[f[0]])
// 	{
// 		if ((pattern[f[1]] == '*' && indic[f[1]] == 'O')|| pattern[f[1]] == str[f[0]])
// 		{
// 			ft_dprintf(2, "pattern : %c, indic : %c ,str : %c\n", pattern[f[1]],indic[f[1]], str[f[0]]);
// 			if (pattern[f[1]] == '*' && indic[f[1]] == 'O')
// 			{
// 				f[2] = f[1]++;
// 				f[3] = f[0];
// 			}
// 			else
// 				a(&f[1], &f[0]);
// 		}
// 		else if (f[2] != -1)
// 		{
// 			f[1] = f[2] + 1;
// 			f[0] = ++f[3];
// 		}
// 		else
// 			return (0);
// 	}
// 	while (pattern[f[1]] == '*')
// 		f[1]++;
// 	// ft_dprintf(2, "-----%c------\n", pattern[f[1]]);
// 	return (pattern[f[1]] == '\0');
// }

// int	ft_ismatch(char *str, char *pattern, char *indic)
// {
// 	int	f[4];

// 	if (!ft_strcmp(str, ".") || !ft_strcmp(str, ".."))
// 		return (0);
// 	f[0]= 0;
// 	f[1] = 0;
// 	f[2] = -1;
// 	f[3] = -1;
// 	return (match_core(str, pattern, indic, f));
// }


int	ft_ismatch(char *text, char *pattern, char *indic)
{
	int	n;
	int	m;
	int	i = 0, j = 0, textPointer = -1, pattPointer;

	n = ft_strlen(text);
	m = ft_strlen(pattern);
	if (m == 0) // when pattern is empty
		return (n == 0);
	i = 0, j = 0, textPointer = -1, pattPointer = -1;
	while (i < n)
	{
		if (indic[j] == 'Q')
		{
			j++;
			//pattPointer++;
			continue;
		}
		if (text[i] == pattern[j])
		{ // matching text and pattern characters
			i++;
			j++;
		}
		else if (j < m && pattern[j] == '*' && indic[j] == 'O')
		{ // as * used for one or more character
			textPointer = i;
			pattPointer = j;
			j++;
		}
		else if (pattPointer != -1)
		{
			j = pattPointer + 1;
			i = textPointer + 1;
			textPointer++;
		}
		else
			return (0);
	}
	while (j < m && pattern[j] == '*')
	{
		j++; // j will increase when wildcard is *
	}

	return (j == m);

}

char	*gen_indic(char *str)
{
	int		i;
	char	c;
	char	*res;

	res = malloc(ft_strlen(str) + 1);
	if (!res)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			c = str[i];
			res[i++] = 'Q';
			while (str[i] != c)
				res[i++] = 'N';
			res[i++] = 'Q';
		}
		else
			res[i++] = 'O';
	}
	res[i] = 0;
	return (res);
}

char	**expanded_wildcard(char *str)
{
	char			**tab;
	DIR				*dir;
	struct dirent	*entry;
	char			*indic;

	dir = opendir(".");
	if (!dir)
		return (perror("opendir"), NULL);
	tab = NULL;
	entry = readdir(dir);
	indic = gen_indic(str);
	if (!indic)
		return (closedir(dir), ft_dprintf(2, "test\n"), NULL);
	while (entry)
	{
		if (ft_ismatch(entry->d_name, str, indic))
			tab = join_tab(tab, ft_strdup(entry->d_name));
		entry = readdir(dir);
	}
	if (!tab)
		tab = join_tab(tab, ft_strdup(str));
	free(indic);
	closedir(dir);
	return (tab);
}
