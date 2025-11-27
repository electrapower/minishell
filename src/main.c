/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:45:37 by ltoscani          #+#    #+#             */
/*   Updated: 2025/11/27 15:14:28 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

static char	**copy_envp(char **envp)
{
	int		count;
	int		i;
	char	**new_env;

	count = 0;
	while (envp[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			ft_free_matrix(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[count] = NULL;
	return (new_env);
}

static void	ft_init_shell(t_data *data, char **envp)
{
	data->env = copy_envp(envp);
	data->exit_status = 0;
	data->need_newline = 0;
	data->should_exit = 0;
}

static void	process_input(char *line, t_data *data)
{
	t_list		*tokens;
	t_command	*pipeline;

	if (!line || !*line)
		return ;
	add_history(line);
	tokens = ft_tokenize(line);
	if (!tokens)
		return ;
	pipeline = parse_pipeline(tokens->content);
	if (!pipeline)
	{
		data->exit_status = 1;
		ft_lstclear(&tokens, ft_free_token);
		return ;
	}
	execute_pipeline(pipeline, data);
	free_pipeline(pipeline);
	ft_lstclear(&tokens, ft_free_token);
}

static void	repl_loop(t_data *data)
{
	char	*line;

	while (!data->should_exit)
	{
		g_signal_received = 0;
		line = readline(PROMPT);
		if (!line)
		{
			printf("exit\n");
			data->should_exit = 1;
			break ;
		}
		if (g_signal_received == SIGINT)
		{
			data->exit_status = 130;
			free(line);
			continue ;
		}
		process_input(line, data);
		free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argc;
	(void)argv;
	ft_init_shell(&data, envp);
	ft_init_signal_handlers();
	repl_loop(&data);
	ft_free_matrix(data.env);
	data.env = NULL;
	return (data.exit_status);
}
