/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 18:55:41 by asalniko          #+#    #+#             */
/*   Updated: 2025/11/27 16:00:00 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/parser.h"
#include "../../include/minishell.h"

int	is_redirection(t_token_type type)
{
	return (type == REDIRECT_IN || type == REDIRECT_OUT
		|| type == APPEND_OUT || type == HEREDOC);
}

int	is_pipe(t_token_type type)
{
	return (type == PIPE);
}

void	skip_spaces(t_token **token)
{
	while (*token && (*token)->type == SPACES)
		*token = (*token)->next;
}

void	arglist_clear(t_arglist **head, int free_strings)
{
	t_arglist	*node;
	t_arglist	*next;

	if (!head || !*head)
		return ;
	node = *head;
	while (node)
	{
		next = node->next;
		if (free_strings && node->arg)
			free(node->arg);
		free(node);
		node = next;
	}
	*head = NULL;
}
