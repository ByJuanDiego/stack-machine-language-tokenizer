#include <iostream>
#include <fstream>
#include <sstream>
#include "token.h"


class Scanner {
private:
    std::string input;
    int first, current;
    int state;

    char nextChar(){
        char c = input[current];
        if (c != '\0') {
            ++current;
        }
        return c;
    }

    void rollBack(){
        if (input[current] != '\0'){
            --current;
        }
    }

    void startLexeme(){
        first = current - 1;
    }

    void incrementStartLexeme() {
        ++first;
    }

    std::string getLexeme(){
        return input.substr(first, current - first);
    }

    static Token* verifyKeywords(const std::string& lexeme) {
        auto it = hashtable.find(lexeme);
        if (it != hashtable.end()) {
            return new Token(it->second);
        }
        return new Token(ID, lexeme);
    }

public:

    explicit Scanner(const char* buffer): first(0), current(0), state(0){
        input = buffer;
    }

    Token* nextToken(){
        Token* token;
        char c = nextChar();
        state = 0;

        startLexeme();

        while (1) {
            switch (state) {
                case 0: {
                    if (c == ' ') {
                        incrementStartLexeme();
                        state = 0;
                    }
                    else if (isalpha(c)) {
                        state = 1;
                    }
                    else if (isdigit(c)) {
                        state = 4;
                    }
                    else if (c == '\n') {
                        state = 6;
                    }
                    else if (c == '\0') {
                        state = 8;
                    }
                    break;
                }
                case 1: {
                    c = nextChar();
                    if (c == ':') {
                        state = 3;
                    }
                    else if (isalnum(c) || c == '_') {
                        state = 1;
                    }
                    else {
                        state = 2;
                    }
                    break;
                }
                case 2: {
                    rollBack();
                    return verifyKeywords(getLexeme());
                }
                case 3: {

                    break;
                }
                case 4: {

                    break;
                }
                case 5: {

                    break;
                }
                case 6: {

                    break;
                }
                case 7: {

                    break;
                }
                case 8: {
                    return new Token(END);
                }
            }
        }


        for (; (c == ' ') || (c == '\t'); c = nextChar());

        if (isalpha(c)) {
            for (; isalnum(c) || c == '_'; c = nextChar());
            rollBack();
            std::string lexeme = getLexeme();
            auto it = hashtable.find(lexeme);
            if (it != hashtable.end()) {
                return new Token(it->second);
            }
            return new Token(ID, getLexeme());
        }

        if (isdigit(c)) {
            for (; isdigit(c); c = nextChar());
            rollBack();
            return new Token(NUM, getLexeme());
        }

        if (c == '\n') {
            for (; c == '\n'; c = nextChar());
            rollBack();
            return new Token(EOL);
        }

        if (c == '\0') {
            return new Token(END);
        }

        return new Token(ERR);

    }

    ~Scanner() = default;

};

int main(int argc, char** argv) {
    std::fstream file(argv[1], std::ios::in);
    std::stringstream stream;
    stream << file.rdbuf();

    Scanner scanner(stream.str().c_str());

    Token* tk = scanner.nextToken();
    while (tk->type != END) {
        std::cout << "next token " << *tk << std::endl;
        delete tk;
        tk =  scanner.nextToken();
    }

    return 0;
}
