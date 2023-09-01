#include <iostream>
#include <fstream>
#include <sstream>
#include "scanner.h"


int main(int argc, char** argv) {
    if (argc < 2) {
        return EXIT_FAILURE;
    }

    std::fstream file(argv[1], std::ios::in);
    std::stringstream stream;
    stream << file.rdbuf();

    Scanner scanner(stream.str().c_str());

    Token* prev = nullptr;
    Token* tk = scanner.nextToken(prev);
    while (tk->type != END) {
        std::cout << *tk << std::endl;
        prev = tk;
        tk = scanner.nextToken(tk);
        delete prev;
    }
    std::cout << *tk << std::endl;

    return EXIT_SUCCESS;
}
