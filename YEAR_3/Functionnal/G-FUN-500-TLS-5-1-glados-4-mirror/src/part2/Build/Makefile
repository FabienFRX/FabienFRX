## ----------------------------------------------------------------------------------- ##
##                                                                                     ##
## EPITECH PROJECT - Fri, Oct, 2025                                                    ##
## Title           - G-FUN-500-TLS-5-1-glados-4-mirror                                 ##
## Description     -                                                                   ##
##     Makefile                                                                        ##
##                                                                                     ##
## ----------------------------------------------------------------------------------- ##
##                                                                                     ##
##             ███████╗██████╗ ██╗████████╗███████╗ ██████╗██╗  ██╗                    ##
##             ██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝██║  ██║                    ##
##             █████╗  ██████╔╝██║   ██║   █████╗  ██║     ███████║                    ##
##             ██╔══╝  ██╔═══╝ ██║   ██║   ██╔══╝  ██║     ██╔══██║                    ##
##             ███████╗██║     ██║   ██║   ███████╗╚██████╗██║  ██║                    ##
##             ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚══════╝ ╚═════╝╚═╝  ╚═╝                    ##
##                                                                                     ##
## ----------------------------------------------------------------------------------- ##

GHC = ghc
GHCFLAGS = -Wall -O2 -threaded -rtsopts

SRC_DIR = ..
COMPILER_DIR = $(SRC_DIR)/Compiler
VM_DIR = $(SRC_DIR)/VM
APP_DIR = $(SRC_DIR)/App
TEST_DIR = $(SRC_DIR)/tests
BUILD_DIR = .

COMPILER_SOURCES = $(COMPILER_DIR)/Parser.hs \
                   $(COMPILER_DIR)/AST.hs \
                   $(COMPILER_DIR)/Compiler.hs

VM_SOURCES = $(VM_DIR)/Types.hs \
             $(VM_DIR)/CompilerInterface.hs \
             $(VM_DIR)/VM.hs

TEST_SOURCES = $(TEST_DIR)/ParserTests.hs \
               $(TEST_DIR)/CompilerTests.hs \
               $(TEST_DIR)/VMTests.hs \
               $(TEST_DIR)/IntegrationTests.hs

COMPILER_MAIN = $(APP_DIR)/CompilerMain.hs
VM_MAIN = $(APP_DIR)/VMMain.hs
TEST_MAIN = $(TEST_DIR)/TestMain.hs

COMPILER_BIN = glados-compiler
VM_BIN = glados-vm
TEST_BIN = test-runner

.PHONY: all clean fclean re compiler vm tests run-tests help

all: compiler vm

compiler: $(COMPILER_BIN)

$(COMPILER_BIN): $(COMPILER_SOURCES) $(COMPILER_MAIN)
	@echo "Compiling GLaDOS compiler..."
	$(GHC) $(GHCFLAGS) -i$(COMPILER_DIR):$(VM_DIR):$(APP_DIR) \
		-outputdir $(BUILD_DIR)/obj_compiler \
		-o $(BUILD_DIR)/$(COMPILER_BIN) \
		-package parsec \
		-package containers \
		-package filepath \
		$(COMPILER_MAIN) $(COMPILER_SOURCES) $(VM_DIR)/CompilerInterface.hs $(VM_DIR)/Types.hs
	@echo "[OK] Compiler built successfully: $(COMPILER_BIN)"

vm: $(VM_BIN)

$(VM_BIN): $(VM_SOURCES) $(VM_MAIN)
	@echo "Compiling GLaDOS Virtual Machine..."
	$(GHC) $(GHCFLAGS) -i$(VM_DIR):$(APP_DIR) \
		-outputdir $(BUILD_DIR)/obj_vm \
		-o $(BUILD_DIR)/$(VM_BIN) \
		-package containers \
		$(VM_MAIN) $(VM_SOURCES)
	@echo "[OK] VM built successfully: $(VM_BIN)"

# Compile test suite
tests: $(TEST_BIN)

$(TEST_BIN): $(TEST_SOURCES) $(TEST_MAIN) $(COMPILER_SOURCES) $(VM_SOURCES)
	@echo "Compiling test suite..."
	$(GHC) $(GHCFLAGS) -i$(TEST_DIR):$(COMPILER_DIR):$(VM_DIR):$(APP_DIR) \
		-outputdir $(BUILD_DIR)/obj_tests \
		-o $(BUILD_DIR)/$(TEST_BIN) \
		-package HUnit \
		-package parsec \
		-package containers \
		$(TEST_MAIN) $(TEST_SOURCES) $(COMPILER_SOURCES) $(VM_SOURCES)
	@echo "[OK] Test suite built successfully: $(TEST_BIN)"

# Run the test suite
run-tests: tests
	@echo ""
	@echo "Running test suite..."
	@echo ""
	@./$(TEST_BIN)

clean:
	@echo "Removing object files..."
	rm -rf $(BUILD_DIR)/obj_compiler $(BUILD_DIR)/obj_vm $(BUILD_DIR)/obj_tests
	rm -f $(APP_DIR)/*.hi $(APP_DIR)/*.o
	rm -f $(COMPILER_DIR)/*.hi $(COMPILER_DIR)/*.o
	rm -f $(VM_DIR)/*.hi $(VM_DIR)/*.o
	rm -f $(TEST_DIR)/*.hi $(TEST_DIR)/*.o
	@echo "[OK] Object files removed"

fclean: clean
	@echo "Removing executables..."
	rm -f $(BUILD_DIR)/$(COMPILER_BIN) $(BUILD_DIR)/$(VM_BIN) $(BUILD_DIR)/$(TEST_BIN)
	@echo "[OK] Executables removed"

re: fclean all

test: all
	@echo "\n=== Testing Compiler and VM ==="
	@echo "Compiler: ./$(COMPILER_BIN) --help"
	@./$(COMPILER_BIN) --help
	@echo "\n"
	@echo "VM: ./$(VM_BIN) --help"
	@./$(VM_BIN) --help

help:
	@echo "Makefile for GLaDOS Part 2 - Compiler and VM"
	@echo ""
	@echo "Available targets:"
	@echo "  all         - Build both the compiler and the VM (default)"
	@echo "  compiler    - Build only the compiler"
	@echo "  vm          - Build only the VM"
	@echo "  tests       - Build the unit test suite"
	@echo "  run-tests   - Build and run all unit tests"
	@echo "  clean       - Remove object files"
	@echo "  fclean      - Remove object files and executables"
	@echo "  re          - Rebuild everything from scratch (fclean + all)"
	@echo "  test        - Quick test the compiler and VM executables"
	@echo "  help        - Show this help message"
	@echo ""
	@echo "Usage examples:"
	@echo "  make              # Build everything"
	@echo "  make compiler     # Build only the compiler"
	@echo "  make vm           # Build only the VM"
	@echo "  make run-tests    # Build and run unit tests"
	@echo "  make clean        # Clean temporary files"
	@echo "  make re           # Rebuild from scratch"