/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 03:18:50 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/09 05:13:03 by ybelatar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void expand_args(char **args, t_minishell *minishell)
// {
//     // expand_env(&args, minishell);
//     // rm_empty(&args);
//     //clean_empty_str(args, minishell);
//     //wildcards(args);
//     //split_no_quotes(args);
//     //strip_quotes(args);
// }

void ft_expand(t_node_ast *node, t_minishell *minishell)
{
    // expand_args(node->args, minishell);
    expand_env(node, minishell);
    rm_empty(node);
    /*...*/
}