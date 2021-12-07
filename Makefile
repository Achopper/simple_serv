NAME = supaDupaServ

FLAGS = -Wall -Werror -Wextra --std=c++98 -Wconversion -fsanitize=address -g
CC	= clang++
RM	= rm -f

SOURCES_DIR = src/
HEADERS_DIR = inc/

SOURCES_LIST = 	main.cpp\
				Core.cpp\
				Client.cpp\
				Config.cpp\
				Server.cpp\
				Location.cpp\
				Response.cpp\
				DefaultPage.cpp\
				Request.cpp\
				Autoindex.cpp

HEADERS_LIST = 	Core.hpp\
				Config.hpp\
				Location.hpp\
				Server.hpp\
				Response.hpp\
				serv_includes.hpp\
				DefaultPage.hpp\
				Request.hpp\
				Autoindex.hpp

SOURCES = $(addprefix $(SOURCES_DIR), $(SOURCES_LIST))
HEADERS = $(addprefix $(HEADERS_DIR), $(HEADERS_LIST))
OBJECTS_DIR = obj/
OBJECTS_LIST = $(patsubst %.cpp, %.o, $(SOURCES_LIST))
OBJECTS	= $(addprefix $(OBJECTS_DIR), $(OBJECTS_LIST))
DEP = $(addprefix $(OBJECTS_DIR), $(SOURCES_LIST:.cpp=.d))
CPPFLAGS = -MMD -I$(HEADERS_DIR)

all: $(NAME)

$(NAME): $(OBJECTS_DIR) $(OBJECTS)
	$(CC) $(FLAGS)  -o $(NAME) $(OBJECTS)

$(OBJECTS_DIR):
	mkdir -p $(OBJECTS_DIR)

$(OBJECTS_DIR)%.o : $(SOURCES_DIR)%.cpp $(HEADERS)
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