/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 07:01:19 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/09 08:23:42 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int has_wildcard(char *str)
{
    int i;
    char c;

    i = 0;
    while (str[i])
    {
        if (str[i] == '"' || str[i] == '\'')
        {
            c = str[i];
            i++;
            while (str[i] != c)
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
    int l;

    res = malloc(sizeof(char *) * (plen(args) + plen(tab) + 1));
    if (!res)
        return (NULL);
    j = 0;
    k = 0;
    l = 0;
    while (j < plen(args) + plen(tab) - 1)
    {
        if (j == *i)
        {
            free(args[k++]);
            while (tab[l])
                res[j++] = tab[l++];
        }
        else
            res[j++] = args[k++];
    }
    res[j] = 0;
    (*i) += plen(tab);
    free(args);
    free(tab);
    return (res);
}

void wildcards(t_node_ast *node)
{
    int i;

    i = 0;
    while (node->args[i])
    {
        if (has_wildcard(node->args[i]))
            node->args = insert_tab_in_tab(node->args, expanded_wildcard(node->args[i]), &i);
        else
            i++;
    }
}