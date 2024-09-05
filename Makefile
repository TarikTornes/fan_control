CC := gcc
CFLAGS := -Wall -O2

TARGET := fan_control
OBJS := fan_control.o general_utils.o fan_utils.o logging_utils.o config_utils.o

BUILD_DIR := ./build
SRC_DIR := ./src

BIN_PATH = /usr/local/bin
SERVICE_PATH = /etc/systemd/system
CONFIG_PATH = /etc


all: $(BUILD_DIR) o_files service config
	$(CC) -o $(BIN_PATH)/$(TARGET) $(patsubst %, $(BUILD_DIR)/%, $(OBJS)) -lwiringPi

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

o_files : $(patsubst %, $(BUILD_DIR)/%, $(OBJS))

$(BUILD_DIR)/fan_control.o : $(SRC_DIR)/fan_control.c $(SRC_DIR)/general_utils.h $(SRC_DIR)/fan_utils.h $(SRC_DIR)/logging_utils.h $(SRC_DIR)/config_utils.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/fan_control.c -o $(BUILD_DIR)/fan_control.o -lwiringPi

$(BUILD_DIR)/fan_utils.o : $(SRC_DIR)/fan_utils.c $(SRC_DIR)/fan_utils.h $(SRC_DIR)/logging_utils.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/fan_utils.c -o $(BUILD_DIR)/fan_utils.o -lwiringPi

$(BUILD_DIR)/general_utils.o : $(SRC_DIR)/general_utils.c $(SRC_DIR)/general_utils.h $(SRC_DIR)/logging_utils.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/general_utils.c -o $(BUILD_DIR)/general_utils.o

$(BUILD_DIR)/logging_utils.o : $(SRC_DIR)/logging_utils.c $(SRC_DIR)/logging_utils.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/logging_utils.c -o $(BUILD_DIR)/logging_utils.o

$(BUILD_DIR)/config_utils.o : $(SRC_DIR)/config_utils.c $(SRC_DIR)/config_utils.h $(SRC_DIR)/logging_utils.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/config_utils.c -o $(BUILD_DIR)/config_utils.o





service : fan_control.service
	if [ ! -f $(SERVICE_PATH)/fan_control.service ]; then \
		install fan_control.service $(SERVICE_PATH); \
	fi

config : fan_control.conf
	if [ ! -f $(CONFIG_PATH)/fan_control.conf ]; then \
		install -m 0666 fan_control.conf $(CONFIG_PATH); \
	fi

clean :
	rm -f $(BUILD_DIR)/*.o $(BIN_PATH)/$(TARGET)

undo :
	mv $(SERVICE_PATH)/fan_control.service .
	mv $(CONFIG_PATH)/fan_control.conf .

clear :
	rm $(SERVICE_PATH)/fan_control.service
	rm $(CONFIG_PATH)/fan_control.conf
	rm /var/log/fan_control.log
	rm $(BIN_PATH)/$(TARGET)


.PHONY : all o_files service config clean undo
