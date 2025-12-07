CC			=	cc
CFLAGS		=	-fsanitize=address -g -Wall -Wextra -Werror

UNAME_S		:=	$(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	IFLAGS	+=	-I/usr/local/opt/readline/include
	LDFLAGS	+=	-L/usr/local/opt/readline/lib
endif

OBJ_DIR		=	src/obj
INCLUDES	=	headers/

BUILTINS	=	built-ins.c echo.c export.c unset.c env.c
ENV			=	envirement_variables.c variable_list_operations.c variable_list_add.c subst_env_vars.c
EXEC		=	exec_bins.c
MAIN		=	main.c signals.c initialization.c
PARSE		=	parse.c parse_helper.c read_write.c redirections.c
UTILS		=	cleanup_helpers.c utils.c

HEADERS		=	${INCLUDES}/minishell.h

SRC			=	$(addprefix src/builtins/, $(BUILTINS)) \
				$(addprefix src/env/, $(ENV)) \
				$(addprefix src/exec/, $(EXEC)) \
				$(addprefix src/parse/, $(PARSE)) \
				$(addprefix src/main/, $(MAIN)) \
				$(addprefix src/utils/, $(UTILS)) \

OBJ			=	$(addprefix $(OBJ_DIR)/, $(SRC:src/%.c=%.o))

LIBFT		=	libs/libft/
LIBFT_A		=	$(LIBFT)libft.a

LDFLAGS		+=	-L${LIBFT}
IFLAGS		+=	-I${INCLUDES}
IFLAGS		+=	-I${LIBFT}

NAME		=	minishell

$(OBJ_DIR)/%.o: src/%.c ${HEADERS}
				@mkdir -p $(dir $@)
				${CC} ${CFLAGS} ${IFLAGS} -c $< -o $@

all:			${NAME}

${LIBFT_A}:
				@make -s -C $(LIBFT) fclean
				@make -s -C $(LIBFT) bonus

${NAME}:		${OBJ} ${LIBFT_A} Makefile
				${CC} ${CFLAGS} ${OBJ} ${IFLAGS} ${LDFLAGS} -lft -lreadline -o ${NAME}

clean:
				rm -rf ${OBJ_DIR}
				@make -s -C $(LIBFT) fclean

fclean: 		clean
				rm -f ${NAME}

re: 			fclean all

.PHONY: 		all clean fclean re
