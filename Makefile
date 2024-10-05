PROJ_NAME = SpaceShooter 
SRC_DIR = ./src
OBJ_DIR = ./objs
INC_DIR = ./include
CC = gcc
CFLAGS = -I$(INC_DIR)/ 
# -no-pie added to the linker flags so that the generated executable can be
#  launched from the file browser in Ubuntu. Otherwise Ubuntu thinks the
#  executable is a "shared library" file. 
LDFLAGS = -lm -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -no-pie
OFLAGS = -O2
GFLAGS = -g -Wall 
DEPS = $(OBJECTS:.o=.d)

ifeq (,$(wildcard $(OBJ_DIR)))
	_ := $(shell mkdir -p $(OBJ_DIR))
endif

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

.PHONY: all clean dev

all: $(PROJ_NAME)

$(PROJ_NAME): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(OFLAGS)

$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(GFLAGS) -MMD -MP -c $< -o $@

clean:
	rm $(OBJECTS) $(DEPS) $(PROJ_NAME)

-include $(DEPS)

dev: $(PROJ_NAME)
	ctags -R *
