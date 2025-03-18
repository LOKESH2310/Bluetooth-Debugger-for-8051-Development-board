# Compiler, project name, includes, flags
CC = sdcc
PROJECT = exec
INCLUDES = -I./headers
CFLAGS = -mmcs51 --std-sdcc99 --verbose --model-large 
BIN_DIR = bin
SRC_DIR = src

# Linker flags without $(OBJ_FILES) directly
LFLAGS = --code-loc 0x4000 --code-size 0x8000 --xram-loc 0x0000 --xram-size 0x7FFF \
         --model-large --out-fmt-ihx

# Main target to generate .hex file in bin
all: $(BIN_DIR)/$(PROJECT).hex

# Collect all .c files in src and create a list of corresponding .rel files in bin
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%.rel,$(SRC_FILES))

# Compile each .c file in src into corresponding .rel in bin
$(BIN_DIR)/%.rel: $(SRC_DIR)/%.c 
	@echo "[INFO] Compiling $<..."
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

# Link all .rel files in bin to produce .ihx in bin
$(BIN_DIR)/$(PROJECT).ihx: $(OBJ_FILES)
	@echo "[INFO] Linking object files into $(PROJECT).ihx..."
	$(CC) $(OBJ_FILES) $(LFLAGS) -o $(BIN_DIR)/$(PROJECT).ihx

# Convert the .ihx file to .hex in bin
$(BIN_DIR)/$(PROJECT).hex: $(BIN_DIR)/$(PROJECT).ihx
	@echo "[INFO] Converting $(PROJECT).ihx to $(PROJECT).hex..."
	packihx $(BIN_DIR)/$(PROJECT).ihx > $(BIN_DIR)/$(PROJECT).hex
	@echo "[SUCCESS] Hex file generated: $(BIN_DIR)/$(PROJECT).hex"

# Flash the program to the microcontroller using batchisp
flash: $(BIN_DIR)/$(PROJECT).hex
	@echo "[INFO] Flashing the microcontroller with batchisp..."
	batchisp -device at89c51rc2 -hardware RS232 -port COM5 -baudrate 115200 \
	-operation erase f memory flash blankcheck loadbuffer ./$(BIN_DIR)/$(PROJECT).hex program verify start reset 1
	@echo "[SUCCESS] Flashing completed."


# Clean all generated files in bin folder (Windows-compatible)
.PHONY: clean
clean:
	@echo "[INFO] Cleaning up generated files..."
	@if exist $(BIN_DIR) del /S /Q $(BIN_DIR)\*
	@echo "[INFO] Clean-up completed."