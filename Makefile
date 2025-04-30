CC=gcc
CFLAGS=-g -Wall -Werror
LIBS=-pthread
SRC_DIR=src
BUILD_DIR=build
TEST_DIR=tests
INCLUDE=-I./$(SRC_DIR)/inc

SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJ=$(BUILD_DIR)/queue.o $(BUILD_DIR)/process_manager.o $(BUILD_DIR)/factory_manager.o

ZIP_NAME=ssoo_p3_100522240_100522110_100522257.zip

all:
	$(CC) $(INCLUDE) -c queue.c -o queue.o
	$(CC) $(INCLUDE) -c process_manager.c -o process_manager.o
	$(CC) $(INCLUDE) -c factory_manager.c -o factory_manager.o

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
	@mkdir -p $(TESTS_DIR)
	zip -j $(ZIP_NAME) $(SRCS) Makefile autores.txt

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR) $(ZIP_NAME)
	@echo "***************"
	@echo "Deleted files!"
	@echo  ""

