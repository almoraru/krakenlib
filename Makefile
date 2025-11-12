# ************************************************************************** #
#                                                                            #
#                        ______                                              #
#                     .-"      "-.                                           #
#                    /            \                                          #
#        _          |              |          _                              #
#       ( \         |,  .-.  .-.  ,|         / )                             #
#        > "=._     | )(__/  \__)( |     _.=" <                              #
#       (_/"=._"=._ |/     /\     \| _.="_.="\_)                             #
#              "=._ (_     ^^     _)"_.="                                    #
#                  "=\__|IIIIII|__/="                                        #
#                 _.="| \IIIIII/ |"=._                                       #
#       _     _.="_.="\          /"=._"=._     _                             #
#      ( \_.="_.="     `--------`     "=._"=._/ )                            #
#       > _.="                            "=._ <                             #
#      (_/                                    \_)                            #
#                                                                            #
#      Filename: Makefile                                                    #
#      🐙 KRAKENLIB - The Unified Pirate Library 🐙                         #
#      By: almoraru <almoraru@student.42.fr>                                #
#      Created: 2025-11-12 22:42:34 by almoraru                             #
#                                                                            #
# ************************************************************************** #

CC = gcc
FLAGS = -Wall -Wextra -Werror -O3 -march=native -msse2 -fPIC -g
AR = ar rcs
RM = rm -rf

# Project
NAME = krakenlib.a
NAME_SO = libkraken.so

# Directories
SRCS_PATH = srcs/
OBJ_PATH = objs/
INC = -I ./includes

# Colors
GREEN = \033[0;32m
BLUE = \033[0;34m
CYAN = \033[0;36m
YELLOW = \033[0;33m
RED = \033[0;31m
MAGENTA = \033[0;35m
NC = \033[0m

# ============================================================================ #
#                               SOURCE FILES                                   #
# ============================================================================ #

# Core Module
CORE_DIR = core/
CORE_SRCS = $(wildcard $(SRCS_PATH)$(CORE_DIR)*.c)

# Printf Module
PRINTF_DIR = printf/
PRINTF_SRCS = $(wildcard $(SRCS_PATH)$(PRINTF_DIR)*.c)

# Get Next Line Module
GNL_DIR = gnl/
GNL_SRCS = $(wildcard $(SRCS_PATH)$(GNL_DIR)*.c)

# Malloc Module
MALLOC_DIR = malloc/
MALLOC_SRCS = $(wildcard $(SRCS_PATH)$(MALLOC_DIR)*.c)

# Combined sources
ALL_SRCS = $(CORE_SRCS) $(PRINTF_SRCS) $(GNL_SRCS) $(MALLOC_SRCS)
ALL_OBJS = $(patsubst $(SRCS_PATH)%.c,$(OBJ_PATH)%.o,$(ALL_SRCS))

# ============================================================================ #
#                                   RULES                                      #
# ============================================================================ #

all: banner $(NAME)
	@echo -e "$(GREEN)⚓ Krakenlib is ready to unleash! 🐙$(NC)"

banner:
	@echo -e "$(CYAN)"
	@echo -e "                       ______"
	@echo -e "                    .-\"      \"-."
	@echo -e "                   /            \\"
	@echo -e "       _          |              |          _"
	@echo -e "      ( \\         |,  .-.  .-.  ,|         / )"
	@echo -e "       > \"=._     | )(__/  \\__)( |     _.=\" <"
	@echo -e "      (_/\"=._\"=._ |/     /\\     \\| _.=\"_.=\"\\_)"
	@echo -e "             \"=._ (_     ^^     _)\"_.=\""
	@echo -e "                 \"=\\__|IIIIII|__/=\""
	@echo -e "                _.=\"| \\IIIIII/ |\"=._"
	@echo -e "      _     _.=\"_.=\"\\          /\"=._\"=._     _"
	@echo -e "     ( \\_.=\"_.=\"     \`--------\`     \"=._\"=._/ )"
	@echo -e "      > _.=\"                            \"=._ <"
	@echo -e "     (_/                                    \\_)"
	@echo -e ""
	@echo -e "          🐙 KRAKENLIB v1.0.0 🏴‍☠️"
	@echo -e "       \"Release the Kraken!\""
	@echo -e "$(NC)"

$(NAME): $(ALL_OBJS)
	@echo -e "$(MAGENTA)📚 Archiving krakenlib...$(NC)"
	@$(AR) $(NAME) $(ALL_OBJS)
	@echo -e "$(GREEN)✅ Static library '$(NAME)' created!$(NC)"

$(NAME_SO): $(ALL_OBJS)
	@echo -e "$(MAGENTA)🔗 Creating shared library...$(NC)"
	@$(CC) -shared -o $(NAME_SO) $(ALL_OBJS) -lm -lpthread
	@echo -e "$(GREEN)✅ Shared library '$(NAME_SO)' created!$(NC)"

$(OBJ_PATH)%.o: $(SRCS_PATH)%.c
	@mkdir -p $(dir $@)
	@echo -e "$(YELLOW)⚡ Compiling: $(notdir $<)$(NC)"
	@$(CC) $(FLAGS) $(INC) -c $< -o $@

shared: $(NAME_SO)

clean:
	@$(RM) $(OBJ_PATH)
	@echo -e "$(RED)🗑️  Object files cleaned.$(NC)"

fclean: clean
	@$(RM) $(NAME) $(NAME_SO) test_runner
	@echo -e "$(RED)🗑️  All build artifacts removed.$(NC)"

re: fclean all

# Testing
test: all
	@echo -e "$(BLUE)🧪 Compiling test suite...$(NC)"
	@$(CC) $(FLAGS) $(INC) tests/test_all.c $(NAME) -lbsd -lm -lpthread -o test_runner
	@echo -e "$(GREEN)▶️  Running tests...$(NC)"
	@./test_runner
	@echo -e "$(GREEN)✅ Tests complete!$(NC)"

.PHONY: all clean fclean re test shared banner
