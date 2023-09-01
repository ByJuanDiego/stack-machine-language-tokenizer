//
// Created by juandiego on 9/1/23.
//

#ifndef LAB01_SCANNER_H
#define LAB01_SCANNER_H

#include "token.h"


class Scanner {

    enum State {
        initial_state,
        get_al_num,
        get_num,
        get_new_line,
        get_label
    };

private:
    std::string input;
    int first, current;
    State state;

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

        if (result[n - 1] == '\n') {
            result = result.substr(0, n - 1);
            current--;
        }

        return result;
    }

    static Token* verifyKeywords(const std::string& lexeme) {
        auto it = hashtable.find(lexeme);
        if (it != hashtable.end()) {
            return new Token(it->second);
        }
        return new Token(ID, lexeme);
    }

public:

    explicit Scanner(const char* buffer): first(0), current(0), state(){
        input = buffer;
    }

    Token* nextToken(){
        char c = nextChar();
        while (c == ' ' || c == '\t') {
            c = nextChar();
            incrementStartLexeme();
        }
        startLexeme();
        state = initial_state;

        while (true) {
            switch (state) {
                case initial_state: {
                    if (isalpha(c)) state = get_al_num;
                    else if (isdigit(c)) state = get_num;
                    else if (c == '\n') state = get_new_line;
                    else if (c == '\0') return new Token(END);
                    else return new Token(ERR);
                    break;
                }
                case get_al_num: {
                    c = nextChar();

                    if (c == ':') state = get_label;
                    else if (isalnum(c) || c == '_') state = get_al_num;
                    else if (c == '\0') return new Token(END);
                    else {
                        rollBack();
                        return verifyKeywords(getLexeme());
                    }
                    break;
                }
                case get_label: {
                    std::string label = getLexeme();
                    return new Token(LABEL, label.substr(0, label.size() - 1));
                }
                case get_num: {
                    c = nextChar();

                    if (isdigit(c)) state = get_num;
                    else {
                        rollBack();
                        return new Token(NUM, getLexeme());
                    }
                    break;
                }
                case get_new_line: {
                    c = nextChar();
                    if (c == '\n') {
                        state = get_new_line;
                    }
                    else {
                        rollBack();
                        return new Token(EOL);
                    }
                }
            }
        }
    }

    ~Scanner() = default;
};


#endif //LAB01_SCANNER_H
