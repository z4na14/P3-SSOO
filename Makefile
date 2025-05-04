CC=gcc
CFLAGS=-g -Wall -Werror -O0
LIBS=-pthread
SRC_DIR=src
BUILD_DIR=build
TEST_DIR=tests
INCLUDE=-I./$(SRC_DIR)/inc

SRCS=$(SRC_DIR)/queue.c $(SRC_DIR)/process_manager.c $(SRC_DIR)/factory_manager.c
OBJ=$(BUILD_DIR)/queue.o $(BUILD_DIR)/process_manager.o $(BUILD_DIR)/factory_manager.o
INC=$(SRC_DIR)/inc/belt_struct.h $(SRC_DIR)/inc/factory_manager.h $(SRC_DIR)/inc/process_manager.h $(SRC_DIR)/inc/queue.h

ZIP_NAME=./tests/ssoo_p3_100522240_100522110_100522257.zip

all:
	$(CC) -I. -O3 -c queue.c -o queue.o
	$(CC) -I. -O3 -c process_manager.c -o process_manager.o
	$(CC) -I. -O3 -c factory_manager.c -o factory_manager.o

	$(CC) $(LIBS) -o factory queue.o process_manager.o factory_manager.o

dev: $(OBJ)
	$(CC) $(CFLAGS) $(LIBS) -o $(BUILD_DIR)/factory  $(OBJ)

	@echo "************************"
	@echo "Compilation successful!"
	@echo ""

$(BUILD_DIR)/queue.o: $(SRC_DIR)/queue.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/process_manager.o: $(SRC_DIR)/process_manager.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/factory_manager.o: $(SRC_DIR)/factory_manager.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@

.PHONY: zip
zip:
	zip -j $(ZIP_NAME) $(SRCS) $(INC) Makefile autores.txt

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR) $(ZIP_NAME)
	@echo "***************"
	@echo "Deleted files!"
	@echo  ""

