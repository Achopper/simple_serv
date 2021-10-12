NAME = supaDupaServ

FLAGS = -Wall -Werror -Wextra --std=c++98 -Wconversion
CC	= clang++
RM	= rm -f

SOURCES_DIR = ./src/
HEADERS_DIR = ./inc/
SOURCES_LIST = main.cpp
HEADERS_LIST =

SOURCES = $(addprefix $(SOURCES_DIR), $(SOURCES_LIST))
HEADERS = $(addprefix $(HEADERS_DIR), $(HEADERS_LIST))
OBJECTS_DIR = obj/
OBJECTS_LIST = $(patsubst %.cpp, %.o, $(SOURCES_LIST))
OBJECTS	= $(addprefix $(OBJECTS_DIR), $(OBJECTS_LIST))
DEP = $(addprefix $(OBJECTS_DIR), $(SOURCES_LIST:.cpp=.d))
CPPFLAGS = -MMD $(HEADERS_LIST)

all: $(NAME)

$(NAME): $(OBJECTS_DIR) $(OBJECTS)
	$(CC) $(FLAGS)  -o $(NAME) $(OBJECTS)

$(OBJECTS_DIR):
	mkdir -p $(OBJECTS_DIR)

$(OBJECTS_DIR)%.o : $(SOURCES_DIR)%.cpp $(HEADERS_LIST)
	$(CC) $(FLAGS) $(CPPFLAGS) -c $< -o $@

-include $(DEP)

clean:
	$(RM)r $(OBJECTS_DIR)

fclean: clean
	$(RM) $(NAME)

leaks:
	leaks --atExit -- ./$(NAME)

re: fclean all

.PHONY: all clean fclean re leaks