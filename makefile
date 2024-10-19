# Nom de l'exécutable
NAME = snake

# Fichiers source
SRCS = main.c \
       snake.c

# Options pour SDL2 et SDL_ttf
SDL2_FLAGS = $(shell sdl2-config --cflags --libs) -lSDL2_ttf

# Règle par défaut : compilation de l'exécutable
all: $(NAME)

# Compilation de l'exécutable
$(NAME): $(SRCS)
	gcc $(SRCS) -o $(NAME) $(SDL2_FLAGS)

# Supprimer l'exécutable
fclean:
	rm -f $(NAME)

# Nettoyer et recompiler
re: fclean all
