/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strip_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 03:51:07 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/09 04:20:21 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_strlen_wquotes(char *str)
{
    int i;
    int len;
    char c;
    
    i = 0;
    len = 0;
    while (str[i])
    {
        if (str[i] == '"' || str[i] == '\'')
        {
            c = str[i];
            i++;
            while (str[i++] != c)
                len++;
            i++;
        }
        else
        {
            i++;
            len++;
        }
    }
    return (len);
}

char *without_quotes(char *str)
{
    char *res;
    int i;
    int j;
    char c;

    res = malloc(ft_strlen_wquotes(str) +  1);
    if (!res)
        return (NULL);
    i = 0;
    j = 0;
    while (str[i])
    {
        if (str[i] == '"' || str[i] == '\'')
        {
            c = str[i];
            i++;
            while (str[i] != c)
                res[j++] = str[i++];
            i++;
        }
        else
            res[j++] = str[i++];
    }
    res[j] = 0;
    return (res);
}

void strip_quotes(char **args)
{
    int i;

    i = 0;
    while (args[i])
    {
        args[i] = without_quotes(args[i]);
        if (args[i])
            return ;
        i++;
    }
}
