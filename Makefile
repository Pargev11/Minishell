iCC			=	cc
CFLAGS		=	-Wall -Wextra -Werror

OBJ_DIR		=	src/obj
SRC_DIR		=	src
INCLUDES	=	libs/

HEADERS		=	${INCLUDES}/minishell.h

SRC			=	${SRC_DIR}/main.c ${SRC_DIR}/read_write.c
OBJ			=	${SRC:${SRC_DIR}/%.c=${OBJ_DIR}/%.o}

NAME		=	minishell

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c	${HEADERS}
				@mkdir -p ${OBJ_DIR}
				${CC} ${CFLAGS} -I${INCLUDES} -c $< -o $@

all:			${NAME}

${NAME}:		${OBJ} ${LIBFT_A} Makefile
				${CC} ${CFLAGS} ${OBJ} -I${INCLUDES} -o ${NAME} -lreadline

clean:
				rm -rf ${OBJ_DIR}

fclean: 		clean
				rm -f ${NAME}

re: 			fclean all

.PHONY: 		all clean fclean re
