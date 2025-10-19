CC			=	cc
CFLAGS		=	-g -Wall -Wextra -Werror -fsanitize=address

UNAME_S		:=	$(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	IFLAGS	+=	-I/usr/local/opt/readline/include
	LDFLAGS	+=	-L/usr/local/opt/readline/lib
endif

OBJ_DIR		=	src/obj
SRC_DIR		=	src
INCLUDES	=	headers/

HEADERS		=	${INCLUDES}/minishell.h

SRC			=	${SRC_DIR}/main.c ${SRC_DIR}/read_write.c ${SRC_DIR}/built-ins.c ${SRC_DIR}/parse.c \
				${SRC_DIR}/exec_bins.c ${SRC_DIR}/cleanup_helpers.c ${SRC_DIR}/signals.c ${SRC_DIR}/initialization.c \
				${SRC_DIR}/utils.c ${SRC_DIR}/subst_env_vars.c ${SRC_DIR}/subst_env_vars2.c \
				${SRC_DIR}/split_with_quotes.c ${SRC_DIR}/split_with_quotes2.c
OBJ			=	${SRC:${SRC_DIR}/%.c=${OBJ_DIR}/%.o}

LIBFT		=	libs/libft/
LIBFT_A		=	$(LIBFT)libft.a

LDFLAGS		+=	-L${LIBFT}
IFLAGS		+=	-I${INCLUDES}
IFLAGS		+=	-I${LIBFT}

NAME		=	minishell

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c	${HEADERS}
				@mkdir -p ${OBJ_DIR}
				${CC} ${CFLAGS} ${IFLAGS} -c $< -o $@

all:			${NAME}

${LIBFT_A}:
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
