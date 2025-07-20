iCC			=	cc
CFLAGS		=	-g -Wall -Wextra -Werror

OBJ_DIR		=	src/obj
SRC_DIR		=	src
INCLUDES	=	libs/

HEADERS		=	${INCLUDES}/minishell.h

SRC			=	${SRC_DIR}/main.c ${SRC_DIR}/read_write.c ${SRC_DIR}/built-ins.c ${SRC_DIR}/parse.c \
				${SRC_DIR}/exec_bins.c ${SRC_DIR}/cleanup_helpers.c
OBJ			=	${SRC:${SRC_DIR}/%.c=${OBJ_DIR}/%.o}

LIBFT		=	$(INCLUDES)/libft/
LIBFT_A		=	$(LIBFT)libft.a

NAME		=	minishell

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c	${HEADERS}
				@mkdir -p ${OBJ_DIR}
				${CC} ${CFLAGS} -I${INCLUDES} -c $< -o $@

all:			${NAME}

${LIBFT_A}:
				@make -C $(LIBFT) all

${NAME}:		${OBJ} ${LIBFT_A} Makefile
				${CC} ${CFLAGS} ${OBJ} -I${INCLUDES} -o ${NAME} -lreadline $(LIBFT_A)

clean:
				rm -rf ${OBJ_DIR}
				@make -C $(LIBFT) fclean

fclean: 		clean
				rm -f ${NAME}

re: 			fclean all

.PHONY: 		all clean fclean re
