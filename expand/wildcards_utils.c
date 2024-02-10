/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 07:01:39 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/10 07:20:06 by ybelatar         ###   ########.fr       */
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
// 		if ((pattern[f[1]] == '*' && indic[f[1]] == 'O') || pattern[f[1]] == str[f[0]])
// 		{
// 			ft_dprintf(2, "pattern : %c, indic : %c ,str : %c\n", pattern[f[1]], indic[f[1]], str[f[0]]);
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

int ft_ismatch(char *s, char *p, char *indic)
{
	int i;
	int sIdx = 0, pIdx = 0, lastWildcardIdx = -1, sBacktrackIdx = -1, nextToWildcardIdx = -1;
	
	while (s[sIdx])
	{
		if (indic[pIdx] == 'Q')
		{
			pIdx++;
			ft_dprintf(2, "test pour %s\n", s);
			continue ;
		}
		if (pIdx < ft_strlen(p) && p[pIdx] == s[sIdx])
		{ 
			// chars match
			++sIdx;
			++pIdx;
		}
		else if (pIdx < ft_strlen(p) && (p[pIdx] == '*' && indic[pIdx] == 'O'))
		{ 
			// wildcard, so chars match - store index.
			lastWildcardIdx = pIdx;
			nextToWildcardIdx = ++pIdx;
			sBacktrackIdx = sIdx;
				
			//storing the pidx+1 as from there I want to match the remaining pattern 
		} 
		else if (lastWildcardIdx == -1)
		{ 
			// no match, and no wildcard has been found.
			return (0);
		}
		else
		{ 
			// backtrack - no match, but a previous wildcard was found.
			pIdx = nextToWildcardIdx;
			sIdx = ++sBacktrackIdx;
			//backtrack string from previousbacktrackidx + 1 index to see if then new pidx and sidx have same chars, if that is the case that means wildcard can absorb the chars in b/w and still further we can run the algo, if at later stage it fails we can backtrack
		}
	}
	i = pIdx;
	while (i < ft_strlen(p))
	{
		if(p[i] != '*')
			return (0);
		i++;
	}
	return (1);
}

char *gen_indic(char *str)
{
    int i;
    char c;
    char *res;

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
		return (ft_dprintf(2, "test\n"), NULL);
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
