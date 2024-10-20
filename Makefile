# Variables
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
TARGET = tarea1
SRCS = tarea1.cpp

# Regla por defecto
all: $(TARGET)

# Regla para compilar el ejecutable
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

# Limpiar archivos generados
clean:
	rm -f $(TARGET)
