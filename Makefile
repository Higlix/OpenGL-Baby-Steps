

DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m
RESET = [0m
BOLD = [1m

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc

SRC += $(wildcard $(SRC_DIR)/*.cpp)

OBJ += $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CC = g++ -std=c++98

LIB = deps/glad.c -lglfw 
CFLAGS = -Wall -Werror -Wextra
#CFLAGS = -I $(INC_DIR) -I deps
NAME = graphic

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	@mkdir -p $@
	@echo "$(MAGENTA)Object directories created..."

$(NAME): $(OBJ)
	@echo -n "$(GREEN)Compiled { $(CYAN)$(NAME) $(GREEN)} $(WHITE)"
	$(CC) -o $(NAME) $(OBJ) $(LIB)
	@mkdir -p bin
	@mv $(NAME) bin

$(OBJ): $(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@echo -n "$(GREEN)Compiled {$(RED) $@ $(GREEN)} $(WHITE)"
	$(CC) -c $< -o $@ $(CFLAGS) -I $(INC_DIR) -I deps

src: fclean all

run: all
	@./bin/$(NAME)

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf bin
	@echo "$(YELLOW)Object directory $(RED)DELETED"

fclean: clean
	@echo "$(YELLOW)$(NAME) $(RED)DELETED"

re: fclean all

.PHONY: all clean fclean re