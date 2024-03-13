ARDUINO_CLI = arduino-cli

SKETCH_DIR = Trap
SKETCH = $(SKETCH_DIR)/Trap.ino
BAUDRATE = 9600
PORT = /dev/ttyACM0
BUILD_DIR = build
TARGET = $(BUILD_DIR)/$(notdir $(SKETCH)).bin

compile: clean
	@cd $(SKETCH_DIR)
	$(ARDUINO_CLI) compile --output-dir $(BUILD_DIR) $(SKETCH)

upload: compile
	@cd $(SKETCH_DIR)
	$(ARDUINO_CLI) upload --input-dir $(BUILD_DIR) $(SKETCH)

clean:
	rm -rf $(BUILD_DIR)

monitor:
	@cd $(SKETCH_DIR)
	$(ARDUINO_CLI) monitor --port $(PORT) --config baudrate=$(BAUDRATE)

.PHONY: compile upload clean monitor
