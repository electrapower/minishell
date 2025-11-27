/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 18:11:55 by asalniko          #+#    #+#             */
/*   Updated: 2025/11/27 16:01:26 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/parser.h"
#include "../../include/minishell.h"

static t_command	*alloc_command(void)
{
	t_command	*cmd;

	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->arg_quoted = NULL;
	cmd->redirections = NULL;
	cmd->pipe_output = 0;
	cmd->from_env_var = 0;
	cmd->next = NULL;
	return (cmd);
}

static int	push_arg_and_quote(t_token **token, t_arglist **alist,
				t_quotedlist **qlist)
{
	t_token	*start;
	t_token	*tmp;
	char	*arg;
	int		overall;

	start = *token;
	arg = parse_one_arg(token);
	if (!arg || arg_push_back(alist, arg) != 0)
	{
		if (arg)
			free(arg);
		return (-1);
	}
	overall = start->quoted;
	tmp = start->next;
	while (tmp && tmp != *token && is_arg_token(tmp->type))
	{
		if (tmp->quoted == Q_SQUOTE)
			overall = Q_SQUOTE;
		tmp = tmp->next;
	}
	if (quotedlist_push_back(qlist, overall) != 0)
		return (-1);
	return (0);
}

static int	parse_args_and_redirs(t_token **token, t_command *cmd,
				t_arglist **alist, t_quotedlist **qlist)
{
	t_token	*cur;
	int		h;

	cur = *token;
	while (cur && cur->type != END_OF_FILE && !is_pipe(cur->type))
	{
		h = handle_space_or_redir(&cur, cmd);
		if (h < 0)
			return (-1);
		if (h > 0)
			continue ;
		if (!is_arg_token(cur->type))
			return (syntax_err_token(cur));
		if (push_arg_and_quote(&cur, alist, qlist) != 0)
			return (-1);
	}
	*token = cur;
	return (0);
}

static t_command	*finalize_command(t_command *cmd, t_arglist **alist,
				t_quotedlist **qlist, t_token **cur)
{
	(void)cur;
	if (!*alist && !cmd->redirections)
	{
		printf("syntax error near unexp token `newline`\n");
		free_command(cmd);
		return (NULL);
	}
	cmd->args = arglist_to_argv(*alist);
	if (!cmd->args)
	{
		arglist_clear(alist, 1);
		quotedlist_clear(qlist);
		free_command(cmd);
		return (NULL);
	}
	if (*qlist)
		cmd->arg_quoted = quotedlist_to_array(*qlist);
	arglist_clear(alist, 0);
	quotedlist_clear(qlist);
	return (cmd);
}

t_command	*parse_command(t_token **current)
{
	t_command		*cmd;
	t_token			*tok;
	t_arglist		*alist;
	t_quotedlist	*qlist;

	cmd = alloc_command();
	if (!cmd)
		return (NULL);
	alist = NULL;
	qlist = NULL;
	tok = *current;
	if (parse_leading_redirs(&tok, cmd) != 0)
		return (free_command(cmd), NULL);
	if (parse_args_and_redirs(&tok, cmd, &alist, &qlist) != 0)
	{
		arglist_clear(&alist, 1);
		quotedlist_clear(&qlist);
		return (free_command(cmd), NULL);
	}
	cmd = finalize_command(cmd, &alist, &qlist, &tok);
	if (!cmd)
		return (NULL);
	*current = tok;
	return (cmd);
}
