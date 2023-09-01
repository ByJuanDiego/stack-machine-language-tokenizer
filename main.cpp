#include <iostream>
#include <fstream>
#include <sstream>
#include "scanner.h"


/*
 * Authors:
 *
 * Juan Diego Prochazka
 * Juan Diego Castro
 */

int main(int argc, char** argv) {
    if (argc < 2) {
        return EXIT_FAILURE;
    }

    std::fstream file(argv[1], std::ios::in);
    std::stringstream stream;
    stream << file.rdbuf();

    Scanner scanner(stream.str().c_str());

    Token* tk = scanner.nextToken();
    while (tk->type != END) {
        std::cout << *tk << std::endl;
        delete tk;
        tk = scanner.nextToken();
    }
    std::cout << *tk << std::endl;

    return EXIT_SUCCESS;
}
