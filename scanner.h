//
// Created by juan diego on 9/1/23.
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

        tokenize_label,
        tokenize_num,
        tokenize_id,
        tokenize_eol,
        tokenize_eof,
        tokenize_err
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

    explicit Scanner(const char* buffer): first(0), current(0), state(initial_state){
        input = buffer;
    }

    // Action - Read - Transition
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
                    else if (c == '\0') state = tokenize_eof;
                    else state = tokenize_err;
                    break;
                }

                // Readers
                case get_al_num: {
                    c = nextChar();
                    if (c == ':') state = tokenize_label;
                    else if (isalnum(c) || c == '_') state = get_al_num;
                    else if (c == '\0') state = tokenize_eof;
                    else {
                        rollBack();
                        state = tokenize_id;
                    }
                    break;
                }
                case get_num: {
                    c = nextChar();
                    if (isdigit(c)) state = get_num;
                    else {
                        rollBack();
                        state = tokenize_num;
                    }
                    break;
                }
                case get_new_line: {
                    c = nextChar();
                    if (c == '\n') state = get_new_line;
                    else {
                        rollBack();
                        state = tokenize_eol;
                    }
                    break;
                }

                // Tokenizers
                case tokenize_label: {
                    std::string label = getLexeme();
                    size_t n = label.size();
                    return new Token(LABEL, label.substr(0, n - 1));
                }

                case tokenize_eof: return new Token(END);

                case tokenize_err: return new Token(ERR);

                case tokenize_eol: return new Token(EOL);

                case tokenize_num: return new Token(NUM, getLexeme());

                case tokenize_id: return verifyKeywords(getLexeme());
            }
        }
    }

    ~Scanner() = default;
};


#endif //LAB01_SCANNER_H
