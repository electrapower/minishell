/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command_helper2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 23:56:55 by asalniko          #+#    #+#             */
/*   Updated: 2025/11/27 16:00:31 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/parser.h"
#include "../../include/minishell.h"

char	*join_segments(char *acc, const char *segment)
{
	char	*res;
	size_t	len_acc;
	size_t	len_seg;

	len_acc = 0;
	if (acc)
		len_acc = ft_strlen(acc);
	len_seg = 0;
	if (segment)
		len_seg = ft_strlen(segment);
	res = (char *)malloc(len_acc + len_seg + 1);
	if (!res)
	{
		free(acc);
		return (NULL);
	}
	if (acc)
		ft_memcpy(res, acc, len_acc);
	if (segment)
		ft_memcpy(res + len_acc, segment, len_seg);
	res[len_acc + len_seg] = '\0';
	free(acc);
	return (res);
}

char	*parse_one_arg(t_token **current)
{
	t_token	*cursor;
	char	*arg;
	char	*unquoted;

	cursor = *current;
	arg = NULL;
	while (cursor && is_arg_token(cursor->type))
	{
		if (cursor->type == STRING_LITERAL && cursor->value)
		{
			unquoted = remove_outer_quotes(cursor->value);
			arg = join_segments(arg, unquoted);
			free(unquoted);
		}
		else
			arg = join_segments(arg, cursor->value);
		if (!arg)
			return (NULL);
		cursor = cursor->next;
	}
	*current = cursor;
	return (arg);
}

int	quotedlist_push_back(t_quotedlist **head, int quoted)
{
	t_quotedlist	*new_node;
	t_quotedlist	*cursor;

	new_node = (t_quotedlist *)malloc(sizeof(t_quotedlist));
	if (!new_node)
		return (-1);
	new_node->quoted = quoted;
	new_node->next = NULL;
	if (!*head)
	{
		*head = new_node;
		return (0);
	}
	cursor = *head;
	while (cursor->next)
		cursor = cursor->next;
	cursor->next = new_node;
	return (0);
}

void	quotedlist_clear(t_quotedlist **head)
{
	t_quotedlist	*node;
	t_quotedlist	*next;

	if (!head || !*head)
		return ;
	node = *head;
	while (node)
	{
		next = node->next;
		free(node);
		node = next;
	}
	*head = NULL;
}
