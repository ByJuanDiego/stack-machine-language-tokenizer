//
// Created by juandiego on 9/1/23.
//

#ifndef LAB01_SCANNER_H
#define LAB01_SCANNER_H

#include "token.h"


class Scanner {
private:
    std::string input;
    int first, current;
    int state;

    char nextChar(){
        char c = input[current];
        if (c != '\0') current++;
        return c;
    }

    void rollBack(){
        if (input[current] != '\0') --current;
    }

    void startLexeme(){
        first = current - 1;
    }

    void incrementStartLexeme() {
        ++first;
    }

    std::string getLexeme(){
        std::string result = input.substr(first, current - first);
        size_t n = result.size();
        return (result[n - 1] == '\n')? result.substr(0, n - 1) : result;
    }

    static Token* verifyKeywords(const std::string& lexeme, Token* prev) {
        auto it = hashtable.find(lexeme);
        if (it != hashtable.end()) {
            return new Token(it->second);
        }
        else if ((prev != nullptr) && (prev->type == GOTO)) {
            return new Token(LABEL, lexeme);
        }
        return new Token(ID, lexeme);
    }

public:

    explicit Scanner(const char* buffer): first(0), current(0), state(){
        input = buffer;
    }

    Token* nextToken(Token* prev){
        char c = nextChar();
        while (c == ' ' || c == '\t') {
            c = nextChar();
            incrementStartLexeme();
        }
        startLexeme();
        state = 0;

        while (true) {
            switch (state) {
                case 0: {
                    if (isalpha(c)) state = 1;
                    else if (isdigit(c)) state = 4;
                    else if (c == '\n') state = 6;
                    else if (c == '\0') return new Token(END);
                    else return new Token(ERR);
                    break;
                }
                case 1: {
                    c = nextChar();

                    if (c == ':') state = 3;
                    else if (isalnum(c) || c == '_') state = 1;
                    else if (c == '\0') return new Token(END);
                    else {
                        rollBack();
                        return verifyKeywords(getLexeme(), prev);
                    }
                    break;
                }
                case 3: {
                    std::string label = getLexeme();
                    return new Token(LABEL, label.substr(0, label.size() - 1));
                }
                case 4: {
                    c = nextChar();

                    if (isdigit(c)) state = 4;
                    else {
                        rollBack();
                        return new Token(NUM, getLexeme());
                    }
                    break;
                }
                case 6: {
                    c = nextChar();
                    if (c == '\n') {
                        state = 6;
                    }

                    else if (c == '\0') return new Token(END);
                    else {
                        rollBack();
                        return new Token(EOL);
                    }
                    break;
                }
            }
        }
    }

    ~Scanner() = default;
};


#endif //LAB01_SCANNER_H
