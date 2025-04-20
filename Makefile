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


all: $(OBJ)
	$(CC) $(CFLAGS) $(LIBS) -o $(BUILD_DIR)/factory  $(OBJ)

	@echo "***************************"
	@echo "Compilation successfully!"
	@echo ""

$(BUILD_DIR)/queue.o: $(SRC_DIR)/queue.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(INCLUDE) -c $< -o $@

$(BUILD_DIR)/process_manager.o: $(SRC_DIR)/process_manager.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(INCLUDE) -c $< -o $@

$(BUILD_DIR)/factory_manager.o: $(SRC_DIR)/factory_manager.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(INCLUDE) -c $< -o $@

zip:
	@mkdir -p $(TESTS_DIR)
	zip -j $(ZIP_NAME) $(SRCS) Makefile autores.txt

clean:
	@rm -rf $(BUILD_DIR) $(TESTS_DIR)
	@echo "***************************"
	@echo "Deleted files!"
	@echo  ""

