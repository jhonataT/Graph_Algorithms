# Nome do executável
PROGRAM = dijkstra

# Compilador
CXX = g++

# Opções de compilação
CXXFLAGS = -std=c++11

# Diretório de saída
OUTPUT_DIR = output

# Regras para compilação
all: $(PROGRAM)

$(PROGRAM): $(PROGRAM).cpp
	$(CXX) $(CXXFLAGS) -o $(PROGRAM) $(PROGRAM).cpp

# Regras para limpeza
clean:
	rm -f $(PROGRAM)
	rm -rf $(OUTPUT_DIR)

# Regras para redirecionar a saída
output:
	mkdir -p $(OUTPUT_DIR)

# Executar o programa com argumentos
run:
	./$(PROGRAM) $(ARGS)

run_output: output
	./$(PROGRAM) $(ARGS) > $(OUTPUT_DIR)/output.txt

.PHONY: all clean run output run_output
