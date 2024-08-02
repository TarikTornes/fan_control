CC := gcc
CFLAGS := -Wall -O2

TARGET := fan_control
OBJS := fan_control.o general_funcs.o

BUILD_DIR := ./build
SRC_DIR := ./src

BIN_PATH = /usr/local/bin
SERVICE_PATH = /etc/systemd/system
CONFIG_PATH = /etc


all: $(BUILD_DIR) o_files service config
	$(CC) -o $(BIN_PATH)/$(TARGET) $(BUILD_DIR)/fan_control.o $(BUILD_DIR)/general_funcs.o

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

o_files : $(patsubst %, $(BUILD_DIR)/%, $(OBJS))

$(BUILD_DIR)/fan_control.o : $(SRC_DIR)/fan_control.c $(SRC_DIR)/general_funcs.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/fan_control.c -o $(BUILD_DIR)/fan_control.o

$(BUILD_DIR)/general_funcs.o : $(SRC_DIR)/general_funcs.c $(SRC_DIR)/general_funcs.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/general_funcs.c -o $(BUILD_DIR)/general_funcs.o

service : ../fan_control.service
	if [ ! -f $(SERVICE_PATH)/fan_control.service ]; then \
		install ../fan_control.service $(SERVICE_PATH); \
	fi

config : ../fan_control.conf
	if [ ! -f $(CONFIG_PATH)/fan_control.conf ]; then \
		install -m 0666../fan_control.conf $(CONFIG_PATH); \
	fi

clean :
	rm -f $(BUILD_DIR)/*.o $(BIN_PATH)/$(TARGET)

undo :
	mv $(SERVICE_PATH)/fan_control.service ..
	mv $(CONFIG_PATH)/fan_control.conf ..


.PHONY : all o_files service config clean undo
