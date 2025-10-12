CXX = g++
CXXFLAGS = -std=c++20 -g -Wall -I. -Isrc/app/encryptDecrypt -Isrc/app/fileHanding -Isrc/app/Procesess -pthread

MAIN_TARGET = encrypt_decrypt
CRYPTION_TARGET = cryption
PARALLEL_TARGET = parallel_demo

MAIN_SRC = main.cpp \
           src/app/Procesess/ProcessManagement.cpp \
           src/app/fileHanding/IO.cpp \
           src/app/fileHanding/ReadENV.cpp \
           src/app/encryptDecrypt/Cryption.cpp \
           src/app/encryptDecrypt/ParallelCryption.cpp

CRYPTION_SRC = src/app/encryptDecrypt/CryptionMain.cpp \
               src/app/encryptDecrypt/Cryption.cpp \
               src/app/fileHanding/IO.cpp \
               src/app/fileHanding/ReadENV.cpp

PARALLEL_SRC = src/app/encryptDecrypt/ParallelCryptionDemo.cpp \
               src/app/Procesess/ProcessManagement.cpp \
               src/app/fileHanding/IO.cpp \
               src/app/fileHanding/ReadENV.cpp \
               src/app/encryptDecrypt/Cryption.cpp \
               src/app/encryptDecrypt/ParallelCryption.cpp

MAIN_OBJ = $(MAIN_SRC:.cpp=.o)
CRYPTION_OBJ = $(CRYPTION_SRC:.cpp=.o)
PARALLEL_OBJ = $(PARALLEL_SRC:.cpp=.o)

all: $(MAIN_TARGET) $(CRYPTION_TARGET) $(PARALLEL_TARGET)

$(MAIN_TARGET): $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(CRYPTION_TARGET): $(CRYPTION_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(PARALLEL_TARGET): $(PARALLEL_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /q $(MAIN_OBJ) $(CRYPTION_OBJ) $(PARALLEL_OBJ) $(MAIN_TARGET).exe $(CRYPTION_TARGET).exe $(PARALLEL_TARGET).exe 2>nul || echo "Clean completed"

.PHONY: clean all