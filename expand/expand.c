/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 03:18:50 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/07 06:19:16 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void expand_args(char **args, t_minishell *minishell)
{
    expand_env(args, minishell);
    rm_empty(args);
    //clean_empty_str(args, minishell);
    wildcards(args, minishell);
    split_no_quotes(args, minishell);
    strip_quotes(args, minishell);
}