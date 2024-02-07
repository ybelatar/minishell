/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_some.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 05:48:40 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/07 06:19:09 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int get_real_len(char **args)
{
    int i;
    int len;

    i = 0;
    len = 0;
    while (args[i])
    {
        if (*args[i])
            len++;
        i++;
    }
    return (len);
}

void rm_empty(char **args)
{
    char **res;
    int i;
    int j;
    
    i = 0;
    j = 0;
    res = malloc(sizeof(char *) * (get_real_len(args) + 1));
    if (!res)
        return ;
    while (args[i])
    {
        if (*args[i])
            res[j++] = args[i];
        i++;
    }
    clear_tab(args);
    args = res;
}

// void clean_empty_str(char **args)
// {
//     int i;
// }