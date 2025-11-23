# ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; #
#                                                                              #
#                        ______                                                #
#                     .-"      "-.                                             #
#                    /            \                                            #
#        _          |              |          _                                #
#       ( \         |,  .-.  .-.  ,|         / )                               #
#        > "=._     | )(__/  \__)( |     _.=" <                                #
#       (_/"=._"=._ |/     /\     \| _.="_.="\_)                               #
#              "=._ (_     ^^     _)"_.="                                      #
#                  "=\__|IIIIII|__/="                                          #
#                 _.="| \IIIIII/ |"=._                                         #
#       _     _.="_.="\          /"=._"=._     _                               #
#      ( \_.="_.="     `--------`     "=._"=._/ )                              #
#       > _.="                            "=._ <                               #
#      (_/                                    \_)                              #
#                                                                              #
#      Filename: Makefile                                                      #
#      By: espadara <espadara@pirate.capn.gg>                                  #
#      Created: 2025/11/12 23:58:25 by espadara                                #
#      Updated: 2025/11/23 17:41:19 by espadara                                #
#                                                                              #
# ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; #


# 🐙 KRAKENLIB - The Unified Pirate Library 🐙
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
TEST_DIR = tests/

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
#                                TEST FILES                                    #
# ============================================================================ #

TEST_SEALIB = $(TEST_DIR)test_sealib.c
TEST_ARENA = $(TEST_DIR)test_arena.c
TEST_PRINTF = $(TEST_DIR)test_printf.c
TEST_GNL = $(TEST_DIR)test_gnl.c
TEST_MALLOC = $(TEST_DIR)test_malloc.c
TEST_ALL = $(TEST_DIR)test_all.c
BENCHMARK_SRC = $(TEST_DIR)benchmark.c

# Test executables
TEST_SEALIB_BIN = test_sealib
TEST_ARENA_BIN = test_arena
TEST_PRINTF_BIN = test_printf
TEST_GNL_BIN = test_gnl
TEST_MALLOC_BIN = test_malloc
TEST_ALL_BIN = test_runner
BENCHMARK_BIN = benchmark

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
	@echo -e "          🐙 KRAKENLIB v2.0.0 🏴‍☠️"
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
	@$(RM) $(NAME) $(NAME_SO)
	@$(RM) $(TEST_SEALIB_BIN) $(TEST_ARENA_BIN) $(TEST_PRINTF_BIN) $(TEST_GNL_BIN) $(TEST_MALLOC_BIN) $(TEST_ALL_BIN)
	@echo -e "$(RED)🗑️  All build artifacts removed.$(NC)"

re: fclean all

# ============================================================================ #
#                              TEST TARGETS                                    #
# ============================================================================ #

# Quick test - runs the main test suite
test: all
	@echo -e "$(BLUE)🧪 Compiling quick test suite...$(NC)"
	@$(CC) $(FLAGS) $(INC) $(TEST_ALL) $(NAME) -lbsd -lm -lpthread -o $(TEST_ALL_BIN)
	@echo -e "$(GREEN)▶️  Running quick tests...$(NC)"
	@./$(TEST_ALL_BIN)
	@echo -e "$(GREEN)✅ Quick tests complete!$(NC)"

# Full test suite - compiles and runs ALL tests
test-all: all test-sealib test-arena test-printf test-gnl test-malloc
	@echo -e "$(GREEN)"
	@echo -e "🐙 ============================================== 🐙"
	@echo -e "       🎉 ALL KRAKENLIB TESTS PASSED! 🎉"
	@echo -e "       The Kraken has been fully tested!"
	@echo -e "🐙 ============================================== 🐙"
	@echo -e "$(NC)"

# Individual test targets
test-sealib: all
	@echo -e "$(BLUE)🔧 Testing Core Module (sealib)...$(NC)"
	@$(CC) $(FLAGS) $(INC) $(TEST_SEALIB) $(NAME) -lbsd -lm -o $(TEST_SEALIB_BIN)
	@./$(TEST_SEALIB_BIN)
	@echo -e "$(GREEN)✅ Core module tests passed!$(NC)"
	@echo ""

test-arena: all
	@echo -e "$(BLUE)🏟️  Testing Arena Allocator...$(NC)"
	@$(CC) $(FLAGS) $(INC) $(TEST_ARENA) $(NAME) -lbsd -lm -o $(TEST_ARENA_BIN)
	@./$(TEST_ARENA_BIN)
	@echo -e "$(GREEN)✅ Arena allocator tests passed!$(NC)"
	@echo ""

test-printf: all
	@echo -e "$(BLUE)🖨️  Testing Printf Module...$(NC)"
	@$(CC) $(FLAGS) $(INC) $(TEST_PRINTF) $(NAME) -lbsd -lm -o $(TEST_PRINTF_BIN)
	@./$(TEST_PRINTF_BIN)
	@echo -e "$(GREEN)✅ Printf module tests passed!$(NC)"
	@echo ""

test-gnl: all
	@echo -e "$(BLUE)📖 Testing Get Next Line...$(NC)"
	@$(CC) $(FLAGS) $(INC) $(TEST_GNL) $(NAME) -lbsd -lm -o $(TEST_GNL_BIN)
	@./$(TEST_GNL_BIN)
	@echo -e "$(GREEN)✅ Get Next Line tests passed!$(NC)"
	@echo ""

test-malloc: all
	@echo -e "$(BLUE)💾 Testing Malloc Module...$(NC)"
	@$(CC) $(FLAGS) $(INC) $(TEST_MALLOC) $(NAME) -lbsd -lm -lpthread -o $(TEST_MALLOC_BIN)
	@./$(TEST_MALLOC_BIN)
	@echo -e "$(GREEN)✅ Malloc module tests passed!$(NC)"
	@echo ""


benchmark: all
	@echo -e "$(BLUE)⚡ Building Benchmark Suite...$(NC)"
	@$(CC) $(FLAGS) -O3 $(INC) $(BENCHMARK_SRC) $(NAME) -lbsd -lm -o $(BENCHMARK_BIN)
	@echo -e "$(GREEN)✅ Benchmark built!$(NC)"
	@echo -e "$(YELLOW)Run with: ./$(BENCHMARK_BIN)$(NC)"
	@echo ""

run-benchmark: benchmark
	@echo -e "$(BLUE)⚡ Running Benchmarks...$(NC)"
	@./$(BENCHMARK_BIN)

# Help target
help:
	@echo -e "$(CYAN)🐙 KRAKENLIB MAKEFILE HELP 🐙$(NC)"
	@echo -e ""
	@echo -e "$(YELLOW)Main targets:$(NC)"
	@echo -e "  make              - Build the library"
	@echo -e "  make shared       - Build shared library (.so)"
	@echo -e "  make clean        - Remove object files"
	@echo -e "  make fclean       - Remove all build artifacts"
	@echo -e "  make re           - Rebuild from scratch"
	@echo -e ""
	@echo -e "$(YELLOW)Test targets:$(NC)"
	@echo -e "  make test         - Run quick test suite"
	@echo -e "  make test-all     - Run ALL module tests"
	@echo -e "  make test-sealib  - Test core utilities"
	@echo -e "  make test-arena   - Test arena allocator"
	@echo -e "  make test-printf  - Test printf module"
	@echo -e "  make test-gnl     - Test get_next_line"
	@echo -e "  make test-malloc  - Test malloc module"
	@echo -e ""
	@echo -e "$(CYAN)⚓ Release the Kraken! ⚓$(NC)"

.PHONY: all clean fclean re test test-all test-sealib test-arena test-printf test-gnl test-malloc shared banner help benchmark run-benchmark
