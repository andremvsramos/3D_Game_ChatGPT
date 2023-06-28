CC = gcc
CFLAGS = -Wall -Wextra -fPIE -g
LDFLAGS = -L./minilibx-linux -lmlx -lXext -lX11 -lm -lbsd

TARGET = 3d_game

SRCS = main.c
OBJS = $(SRCS:.c=.o)

MINILIBX_DIR = ./minilibx-linux
MINILIBX_MAKEFILE = $(MINILIBX_DIR)/Makefile
MINILIBX_LIB = $(MINILIBX_DIR)/libmlx.a

.PHONY: all clean minilibx

all: minilibx $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

minilibx:
	$(MAKE) -C $(MINILIBX_DIR)

clean:
	rm -f $(OBJS) $(TARGET)
	$(MAKE) -C $(MINILIBX_DIR) clean
