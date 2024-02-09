/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 07:01:19 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/09 04:23:56 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int has_wildcard(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == '"')
        {
            i++;
            while (str[i] != '"')
                i++;
        }
        else if (str[i] == '\'')
        {
            i++;
            while (str[i] != '\'')
                i++;
        }
        else if (str[i] == '*')
            return (1);
        i++;
    }
    return (0);
}

char **insert_tab_in_tab(char **args, char **tab, int *i)
{
    char **res;
    int j;
    int k;

    res = malloc(sizeof(char) * (plen(args) + plen(tab)));
    if (!res)
        return (NULL);
    j = 0;
    k = 0;
    while (j < plen(args) + plen(tab))
    {
        if (j == *i)
        {
            free(args[k++]);
            while (*tab)
                res[j++] = *tab;
        }
        else
            res[j++] = args[k++];
    }
    res[j] = 0;
    (*i) += plen(tab);
    return (res);
}

void wildcards(char **args)
{
    int i;

    i = 0;
    while (args[i])
    {
        if (has_wildcard(args[i]))
            args = insert_tab_in_tab(args, expanded_wildcard(args[i]), &i);
        else
            i++;
    }
}