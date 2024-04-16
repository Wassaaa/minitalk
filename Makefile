# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/12 14:15:32 by aklein            #+#    #+#              #
#    Updated: 2024/04/16 18:14:37 by aklein           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CLIENT_SRCS		=	client.c
SERVER_SRCS		=	server.c

################################################################################
# COMPILATION
################################################################################
CC				=	cc
CC_STRICT		=	-Wall -Wextra -Werror
CC_DEBUG		=	-g #-fsanitize=leak
HEADERS			=	-I $(LIBFT_INCLUDES) -I $(INCLUDES)
CC_FULL			=	$(CC) $(CC_STRICT) $(HEADERS) $(CC_DEBUG)

################################################################################
# LIBFT
################################################################################
LIBFT			=	./libft/libft.a
LIBFT_DIR		=	./libft
LIBFT_INCLUDES	=	./libft/include

################################################################################
# MANDATORY
################################################################################
NAME			=	minitalk
INCLUDES		=	./include
OBJ_DIR			=	./obj
ARCHIVES		=	./archives
M_HEADER		=	$(INCLUDES)/minitalk.h
M_ARCHIVES		=	$(S_ARCHIVE) $(C_ARCHIVE) $(LIBFT)

################################################################################
# CLIENT
################################################################################
CLIENT			=	client
CLIENT_SRC		=	./src/client
C_OBJECTS		=	$(addprefix $(OBJ_DIR)/, $(CLIENT_SRCS:%.c=%.o))
C_MAIN			=	./c_main.c
C_ARCHIVE		=	$(ARCHIVES)/client.a

################################################################################
# SERVER
################################################################################
SERVER			=	server
SERVER_SRC		=	./src/server
S_OBJECTS		=	$(addprefix $(OBJ_DIR)/, $(SERVER_SRCS:%.c=%.o))
S_MAIN			=	./s_main.c
S_ARCHIVE		=	$(ARCHIVES)/server.a

################################################################################
# RULES
################################################################################

all: $(CLIENT) $(SERVER)

$(SERVER): $(LIBFT) $(S_ARCHIVE) $(S_MAIN)
	$(CC_FULL) $(S_MAIN) $(S_ARCHIVE) $(LIBFT) -o $(SERVER)

$(CLIENT): $(LIBFT) $(C_ARCHIVE) $(C_MAIN)
	$(CC_FULL) $(C_MAIN) $(C_ARCHIVE) $(LIBFT) -o $(CLIENT)

$(LIBFT): libft_force
	make -C $(LIBFT_DIR)

libft_force:
	@true

$(S_ARCHIVE): $(S_OBJECTS)
	mkdir -p $(@D)
	ar rcs $(S_ARCHIVE) $(S_OBJECTS)

$(C_ARCHIVE): $(C_OBJECTS)
	mkdir -p $(@D)
	ar rcs $(C_ARCHIVE) $(C_OBJECTS)

vpath %.c $(SERVER_SRC) $(CLIENT_SRC)

$(OBJ_DIR)/%.o:%.c $(M_HEADER)
	mkdir -p $(@D)
	$(CC_FULL) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(ARCHIVES) $(SERVER).dSYM/ $(CLIENT).dSYM/
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(CLIENT) $(SERVER)
	make fclean -C $(LIBFT_DIR)

re: fclean all

################################################################################
# TESTING
################################################################################

################################################################################
# VALGRIND
################################################################################
CC_VG			=	$(CC) $(CC_STRICT) $(HEADERS)

VG				=	valgrind
VG_ARGS			=	maps/map.ber
VG_FLAGS		=	--leak-check=full --show-leak-kinds=all --suppressions=suppressions
VG_LOG			=	valgrind_leaks.log
VG_LOG_FLAGS	=	--log-file=$(VG_LOG) \
					--leak-check=full \
					--show-leak-kinds=all \
					--track-origins=yes \
					--verbose \
					--suppressions=suppressions \
					--gen-suppressions=all

LOG_VG_TARGET	=	$(VG) $(VG_LOG_FLAGS) ./$(NAME) $(VG_ARGS)

VG_TARGET		=	$(VG) $(VG_FLAGS) ./$(NAME) $(VG_ARGS)

vg: vg_build
	$(VG_TARGET)

vglog: vg_build
	$(LOG_VG_TARGET)

vg_build: libmlx $(LIBFT) $(M_ARCHIVE)
	$(CC_VG) $(M_MAIN) $(M_ARCHIVES) $(MLX42) -o $(NAME)

vglog_clean: fclean
	rm -f $(VG_LOG)

################################################################################
# NORM
################################################################################
norm:
	norminette $(SRC_DIR) $(B_DIR) $(INCLUDES) $(LIBFT_DIR) | grep -v "OK!" || true

norm2:
	norminette $(SRC_DIR) $(B_DIR) $(INCLUDES) $(LIBFT_DIR)

################################################################################
# Colors
################################################################################
# Black, Red, Green, Yellow, Blue, Purple, Cyan, White
BK = \033[0;30m
R = \033[0;31m
G = \033[0;32m
Y = \033[0;33m
B = \033[0;34m
P = \033[0;35m
C = \033[0;36m
W = \033[0;37m
# Bold
BKB = \033[1;30m
RB = \033[1;31m
GB = \033[1;32m
YB = \033[1;33m
BB = \033[1;34m
PB = \033[1;35m
CB = \033[1;36m
WB = \033[1;37m
# Reset Color
RC = \033[0m

################################################################################
# PHONY
################################################################################
.PHONY: all re clean fclean libft_force
