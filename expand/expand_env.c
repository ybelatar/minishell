/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 04:55:52 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/07 05:44:18 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *single_quote(char *str, int *i)
{
    int start;

    start = *i;
    (*i)++;
    while (str[*i] != '\'')
        (*i)++;
    (*i)++;
    return (ft_substr(str, start, *i - start));
}

char *normal(char *str, int *i)
{
    int start;

    start = *i;
    while (str[*i] && str[*i] != '"' && str[*i] != '$' && str[*i] != '\'')
        (*i)++;
    return (ft_substr(str, start, *i - start));
}

char *expand_env_one(char *str, t_minishell *minishell)
{
    char *res;
    int i;

    i = 0;
    res = NULL;
    while (str[i])
    {
        if (str[i] == '\'')
            res = ft_strjoin_free2(res, single_quote(str, &i));
        else if (str[i] == '"')
            res = ft_strjoin_free2(res, double_quote(str, &i, minishell));
        else if (str[i] == '$')
            res = ft_strjoin_free2(res, variable_env(str, &i, minishell));
        else
            res = ft_strjoin_free2(res, normal(str, &i));
    }
}

void expand_env(char **args, t_minishell *minishell)
{
    int i;

    i = 0;
    while (args[i])
        args[i] = expand_env_one(args[i++], minishell);
}