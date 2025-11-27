/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:38:51 by ltoscani          #+#    #+#             */
/*   Updated: 2025/11/27 15:57:05 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define PROMPT "minishell> "

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <dirent.h>
# include <termios.h>
# include <term.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>

# include "libft.h"
# include "lexer.h"
# include "parser.h"

extern volatile sig_atomic_t	g_signal_received;

typedef struct s_data
{
	char	**env;
	int		exit_status;
	int		need_newline;
	int		should_exit;
}	t_data;

typedef struct s_pipe_state
{
	int		prev_pipe_read;
	int		pipefd[2];
}	t_pipe_state;

typedef struct s_pipe_exec
{
	int		in_fd;
	int		pipefd[2];
	pid_t	last_pid;
}	t_pipe_exec;

/* expansion */
char	*handle_quoted_env(t_data *data, t_token *token, char **quoted_string);
void	handle_env(t_data *data, t_command *command, int *ac, t_token *token);
char	*expand_tilda(t_data *data, const char *env_name);
char	*expand_env_variable(t_data *data, const char *env_name);
char	*ft_getenv(t_data *data, const char *env_name);
char	*expand_env_variable_in_string(t_data *data, const char *str);
char	*expand_single_variable(t_data *data,
			const char *var_start, const char *var_end);
int		append_char(char **res, char c);
int		append_var(t_data *data, char **res, const char **pcur,
			int in_squotes);
char	*expand_with_quotes(t_data *data, const char *str,
			int in_single_quotes);

/* builtins */
int		ft_pwd(void);
int		ft_echo(char **args);
int		ft_exit(char **args, t_data *data);
int		ft_cd(char **args, t_data *data);
int		ft_export(char **args, t_data *data);
int		ft_unset(char **args, t_data *data);
int		ft_env(char **args, t_data *data);

/* signals */
void	ft_init_signal_handlers(void);
void	ft_handle_sigint(int signo);
void	ft_handle_sigquit(int signo);
void	handle_sigint_child(int signo);
void	handle_sigquit_child(int signo);

/* free */
void	free_pipeline(t_command *head);
void	ft_free_token(void *token_ptr);
void	ft_free_matrix(char **matrix);
void	free_command(t_command *cmd);
void	free_split(char **split);
void	free_env_copy(char **env);

/* executor */
int		execute_command(t_command *cmd, t_data *data);
int		execute_pipeline(t_command *pipeline, t_data *data);
char	*ft_getenv_from_envp(char **env, const char *name);
char	*build_full_path(char *dir, char *cmd);
void	command_not_found_error(char *cmd);
int		get_exit_status(int status);
int		add_env_variable(t_data *data, char *var, char *value);
int		update_env_variable(t_data *data, char *var, char *value);
char	*get_home_path(t_data *data);
char	*get_oldpwd_path(t_data *data);
int		is_builtin(char *cmd);
int		execute_builtin(t_command *cmd, t_data *data);
void	execute_external(t_command *cmd, t_data *data);
void	expand_command_args(t_command *cmd, t_data *data);
char	*remove_outer_quotes(char *str);
void	shift_arguments_left(t_command *cmd);
int		expand_command_name(t_command *cmd, t_data *data);
char	*find_command_path(char *cmd, char **env);
int		exit_too_many_args(t_data *data);
int		exit_no_args(t_data *data);
int		exit_non_numeric(char *arg, t_data *data);
int		exit_with_code(char *arg, t_data *data);
char	*remove_quotes_for_exit(const char *str);
void	handle_execve_error(char *cmd);
int	run_command_after_redirs(t_command *cmd, t_data *data,
			int in_backup, int out_backup);
int	launch_child(t_command *cmd, t_data *data,
				int in_backup, int out_backup);
char	*expand_non_quoted_arg(t_command *cmd, t_data *data, int i);
void	process_arg(t_command *cmd, t_data *data, int i);

/* redirections */
void	restore_fds(int stdin_fd, int stdout_fd);
int		setup_pipe_redirections(t_command *cmd, t_command *pl,
			t_pipe_state *st);
void	cleanup_pipe_redirections(t_command *cmd, t_pipe_state *st);
void	setup_child_signals(void);
char	*expand_heredoc_line(char *line, t_data *data);
int		setup_redirections_with_data(t_command *cmd, t_data *data);
int		handle_heredoc(char *delimiter, int quoted, t_data *data);
int		handle_input_redirection(char *filename);
int		handle_output_redirection(char *filename, int append_mode);

#endif
