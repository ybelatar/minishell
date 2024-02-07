/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybelatar <ybelatar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 14:54:49 by ybelatar          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/02/07 05:45:33 by ybelatar         ###   ########.fr       */
=======
/*   Updated: 2024/02/07 06:02:45 by wouhliss         ###   ########.fr       */
>>>>>>> 38f93631da9b8bc508f688bc7edaff6f7dcba81a
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "ft_dprintf.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define REDIR_HEREDOC -42
# define BUFFER_SIZE 1024

extern int				g_status;

void					sig_handler(int sig);
void					fork_sig_handler(int sig);
void					heredoc_sig_handler(int sig);

typedef enum e_pretoken_type
{
	WORD,
	WHITESPACE,
	PIPE,
	REDIR,
	OPERATOR,
	OPEN_PAR,
	CLOSED_PAR,
}						t_pretoken_type;

typedef enum e_token_type
{
	OPERAND,
	REDIRECT_IN,
	REDIRECT_OUT,
	HEREDOC,
	REDIRECT_APPEND,
	PIPEPIPE,
	OPEN_P,
	CLOSED_P,
	AND,
	OR
}						t_token_type;

typedef enum e_node_type
{
	T_CMD,
	T_PIPE,
	T_OR,
	T_AND
}						t_node_type;

typedef enum e_redir_type
{
	R_IN,
	R_OUT,
	R_HEREDOC,
	R_APPEND
}						t_redir_type;

typedef struct s_pretoken
{
	char				*content;
	t_pretoken_type		type;
	int					wild;
	struct s_pretoken	*next_pretoken;
}						t_pretoken;

typedef struct s_token
{
	char				*content;
	t_token_type		type;
	struct s_token		*next_token;
}						t_token;

typedef struct s_redir_list
{
	t_redir_type		type;
	char				*file;
	struct s_redir_list	*next_redir;
}						t_redir_list;

typedef struct s_node_ast
{
	t_node_type			type;
	char				**args;
	struct s_redir_list	*redirs;
	struct s_node_ast	*left_child;
	struct s_node_ast	*right_child;
}						t_node_ast;

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next_env;
}						t_env;

typedef struct s_pid_list
{
	int					pid;
	struct s_pid_list	*next;
}						t_pid_list;

typedef struct s_minishell
{
	char				*cmd_line;
	char				*prompt;
	t_env				*env;
	t_pretoken			*pretokens;
	t_token				*tokens;
	t_node_ast			*ast;
	int					exit_status;
	int					in;
	int					out;
	t_pid_list			*pid_list;
}						t_minishell;

typedef struct s_cmd
{
	int					fd_in;
	int					fd_out;
	int					pid;
}						t_cmd;

typedef struct s_heredoc
{
	int					in;
	int					out;
	char				*path;
	char				rand[33];
}						t_heredoc;

typedef void			(*t_fct_ptr)(t_minishell *, t_node_ast *, t_cmd *);

/*Parser*/

t_node_ast				*parser(t_token *tokens);

/*exec*/

void					ft_exec(t_minishell *minishell);
int						ft_open_redirs(t_minishell *minishell, t_cmd *cmd,
							t_redir_list *redirs);
void					ft_read(const char *limiter, int out);
int						ft_read_heredoc(t_heredoc *heredoc,
							const char *limiter);
t_pid_list				*add_pid_list(t_pid_list *list, int pid);
void					clear_pid(t_minishell *minishell);
char					*ft_get_bin(t_minishell *minishell, char *cmd);
char					**ft_get_env(t_minishell *minishell);
void					ft_free_tab(char **tab);
void					ft_exec_cmd(t_minishell *minishell, t_node_ast *ast,
							t_cmd *cmd);
void					ft_exec_pipe(t_minishell *minishell, t_node_ast *ast,
							t_cmd *cmd);
void					ft_exec_or(t_minishell *minishell, t_node_ast *ast,
							t_cmd *cmd);
void					ft_exec_and(t_minishell *minishell, t_node_ast *ast,
							t_cmd *cmd);

/*Token builders*/

t_pretoken				*pretokenization(char *str);
t_token					*tokenization(t_pretoken *pretokens);
t_token					*last_token(t_token *token);
int						new_token(t_token **tokens, char *content,
							t_token_type type);

/*Syntax checker*/

int						check_syntax(t_pretoken *pretokens);
void					syntax_error(t_pretoken *pretoken);

/*Expand functions*/

char					*get_env(char *key, t_env *env);
char					*double_quote(char *str, int *i,
							t_minishell *minishell);
char					*variable_env(char *str, int *i,
							t_minishell *minishell);

/*Redirections*/

int						add_redir(t_node_ast *node, t_token **token);
t_redir_list			*create_redir(t_redir_type type, char *file);
void					add_last_redir(t_redir_list **redirs,
							t_redir_list *new);

/*Automata*/

int						q0(t_pretoken *pretokens, int i);
int						q1(t_pretoken *pretokens, int i);
int						q2(t_pretoken *pretokens, int i);
int						q3(t_pretoken *pretokens, int i);
int						q4(t_pretoken *pretokens, int i);
int						q5(t_pretoken *pretokens, int i);

/*Utils*/

int						is_operator(t_token *token);
int						is_redir(t_token *token);
int						get_precedence(t_token_type type);
t_node_ast				*create_node(t_node_type type);
char					**join_tab(char **tab, char *str);
t_node_type				get_type(t_token_type type);
char					*char_to_string(char c, int *i);
int						is_whitespace(char c);
char					*get_whitespace(char *str, int *i);
char					*get_operator(char *str, int *i);
char					*get_redir(char *str, int *i);
char					*get_quoted(char *str, int *i);
int						is_in_charset(char c, char *set);
char					*get_word(char *str, int *i);
t_pretoken				*skip_spaces(t_pretoken *pretokens);
t_pretoken				*new_pretoken(char *content, t_pretoken_type type);
void					update_env(char *key, char *value,
							t_minishell *minishell);
void					delete_env(char *key, t_env *env,
							t_minishell *minishell);
t_env					*new_env(char *key, char *value, int i);
char					*get_name(char *str, int *ptr_i);
void					display_env(t_env *env);
char					*ft_itoa(int n);
int						check_and(char *str);
int						check_quotes(char *str);
char					**sort_tab(char **tab);
int						plen(char **map);
char					**ft_split(char const *s, char c);
char					*get_next_line(int fd);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
int						ft_putstr_fd(char *s, int fd);
char					*get_name(char *str, int *ptr_i);
void					ft_close(int fd);
void					ft_prompt(t_minishell *minishell);

/*Libft functions*/

char					*ft_strchr(char *s, char c);
int						ft_strchri(char *s, char c);
char					*ft_strchrr(char *s, char c);
char					*ft_strdup(char *s);
char					*ft_strjoin(char *s1, char *s2);
char					*ft_strjoin_free(char *s1, char *s2);
char					*ft_substr(char *s, unsigned int start, size_t len);
char					*ft_substr_free(char *s, unsigned int start,
							size_t len);
int						ft_strlen(char *str);
int						ft_isalnum(int c);
int						ft_strcmp(const char *s1, const char *s2);
void					*ft_calloc(size_t nmemb, size_t size);
size_t					ft_strlcpy(char *dst, char *src, size_t size);
int						ft_isatoi(char *str);
int						ft_atoi(const char *str);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
int						ft_isalpha(int c);
void					*ft_memset(void *s, int c, size_t n);
char					*ft_strjoin_free2(char *s1, char *s2);
int						ft_isdigit(int c);

/*Free and error handling functions*/

void					clear_pretokens(t_pretoken **pretokens);
void					clear_tokens(t_token **tokens);
void					clear_tab(char **tab);
void					clear_redirs(t_redir_list **redirs);
void					clear_env(t_env *env);
void					clear_ast(t_node_ast **ast);
void					move_def(t_pretoken **pretoken, int i);
void					move_def_token(t_token **token, int i);

/*Debugging*/

void					display_tab(char **tab);
void					display_pretokens(t_pretoken *pretoken);
void					ft_print_rand(void);

#endif