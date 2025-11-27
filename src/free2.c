/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 19:50:27 by asalniko          #+#    #+#             */
/*   Updated: 2025/11/24 19:50:31 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/minishell.h"

void	ft_free_token(void *token_ptr)
{
	t_token	*token;

	if (!token_ptr)
		return ;
	token = (t_token *)token_ptr;
	free(token->value);
	token->value = NULL;
	free(token);
}

