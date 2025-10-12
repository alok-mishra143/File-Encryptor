#include "Cryption.hpp"
#include <iostream>

int main(int argc, char* argv[] ) {
   if(argc != 2) {
      std::cerr << "Usage: " << argv[0] << " <taskData>" << std::endl;
      return 1;
   }
    executeCryption(argv[1]);
    return 0;
}