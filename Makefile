CC = cc

CFLAGS = -Wall -Wextra -Werror -g3

INCLUDE_PATH = ./includes/

INCLUDES = 	$(INCLUDE_PATH)ft_dprintf.h \
			$(INCLUDE_PATH)minishell.h

INCLUDE = -I$(INCLUDE_PATH)

RM = rm -rf

NAME = minishell

BUILTINS_PATH = ./builtins/

BUILTINS = 	$(BUILTINS_PATH)cd.c	\
			$(BUILTINS_PATH)echo.c	\
			$(BUILTINS_PATH)env_utils.c	\
			$(BUILTINS_PATH)env.c	\
			$(BUILTINS_PATH)exit.c	\
			$(BUILTINS_PATH)export_utils.c	\
			$(BUILTINS_PATH)export.c	\
			$(BUILTINS_PATH)pwd.c	\
			$(BUILTINS_PATH)unset.c

CLEAR_ERROR_PATH	=	./clear_error/

CLEAR_ERROR	=	$(CLEAR_ERROR_PATH)clear.c	\
				$(CLEAR_ERROR_PATH)clear1.c

EXEC_PATH	=	./exec/

EXEC	=	$(EXEC_PATH)exec_builtin.c	\
			$(EXEC_PATH)exec_child.c	\
			$(EXEC_PATH)exec_utils.c	\
			$(EXEC_PATH)exec.c	\
			$(EXEC_PATH)heredoc.c	\
			$(EXEC_PATH)pid.c	\
			$(EXEC_PATH)pipe.c	\
			$(EXEC_PATH)redirs.c

EXPAND_PATH	=	./expand/

EXPAND	=	$(EXPAND_PATH)clean_some.c	\
			$(EXPAND_PATH)expand_env_utils.c	\
			$(EXPAND_PATH)expand_env.c	\
			$(EXPAND_PATH)expand.c	\
			$(EXPAND_PATH)split_no_quotes.c	\
			$(EXPAND_PATH)strip_quotes.c	\
			$(EXPAND_PATH)wildcards_utils.c	\
			$(EXPAND_PATH)wildcards.c

FT_DPRINTF_PATH	=	./ft_dprintf/

FT_DPRINTF	=	$(FT_DPRINTF_PATH)ft_dprintf.c	\
				$(FT_DPRINTF_PATH)dprintf_utils.c	\
				$(FT_DPRINTF_PATH)dprintf_utils2.c

PARSING_PATH = ./parsing/

PARSING	=	$(PARSING_PATH)ast_redir.c	\
			$(PARSING_PATH)ast_utils.c	\
			$(PARSING_PATH)ast_utils2.c	\
			$(PARSING_PATH)ast.c

PRETOKEN_PATH	=	./pretoken/

PRETOKEN	=	$(PRETOKEN_PATH)checking.c	\
				$(PRETOKEN_PATH)pretoken_utils.c	\
				$(PRETOKEN_PATH)pretoken_utils2.c	\
				$(PRETOKEN_PATH)pretoken.c

SIGNAL_PATH	=	./signal/

SIGNAL	=	$(SIGNAL_PATH)signal.c


TOKEN_PATH	=	./token/

TOKEN	=	$(TOKEN_PATH)automate.c	\
			$(TOKEN_PATH)automate2.c	\
			$(TOKEN_PATH)token_utils.c	\
			$(TOKEN_PATH)token.c

UTILS_PATH	=	./utils/

UTILS	=	$(UTILS_PATH)ft_atoi.c	\
			$(UTILS_PATH)ft_calloc.c	\
			$(UTILS_PATH)ft_isalnum.c	\
			$(UTILS_PATH)ft_parse.c	\
			$(UTILS_PATH)ft_strchr.c	\
			$(UTILS_PATH)ft_strcmp.c	\
			$(UTILS_PATH)ft_strdup.c	\
			$(UTILS_PATH)ft_strjoin.c	\
			$(UTILS_PATH)ft_strlcpy.c	\
			$(UTILS_PATH)ft_substr.c	\
			$(UTILS_PATH)get_next_line.c	\
			$(UTILS_PATH)split.c	\
			$(UTILS_PATH)utils.c

SRCS	=	main.c $(BUILTINS) $(CLEAR_ERROR) $(EXEC) $(EXPAND) $(FT_DPRINTF) $(PARSING) $(PRETOKEN) $(SIGNAL) $(TOKEN) $(UTILS)

OBJS	=	${SRCS:.c=.o}

all: $(NAME)

$(NAME): $(OBJS) $(INCLUDES)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lreadline

bonus: all

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $(<:.c=.o)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean 
	make all

.PHONY: all clean fclean re bonus
