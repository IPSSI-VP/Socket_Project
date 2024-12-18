# =====================================================================================
#
#       Filename:  Makefile
#
#    Description:  Fichier de compilation du projet
#
#        Version:  1.0
#        Created:  18/12/2024
#       Revision:  none
#
#         Author:  Val
#   Organization:  
#
# =====================================================================================

CC=gcc
CFLAGS=-I./include -Wall -Wextra
SRC_DIR=src
BUILD_DIR=build
INCLUDE_DIR=include

SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
TARGET=$(BUILD_DIR)/program

.PHONY: all clean

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/*
