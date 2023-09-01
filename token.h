//
// Created by juan diego on 8/31/23.
//

#ifndef LAB01_TOKEN_H
#define LAB01_TOKEN_H

#include <string>
#include <vector>
#include <unordered_map>

enum Type {
    ID,
    NUM,
    EOL,
    ERR,
    END,
    LABEL,
    JMP_EQ,
    JMP_GT,
    JMP_GE,
    JMP_LT,
    JMP_LE,
    GOTO,
    SKIP,
    POP,
    DUP,
    SWAP,
    ADD,
    SUB,
    MUL,
    DIV,
    STORE,
    LOAD,
    PUSH
};

std::vector<std::string> token_names = {
        "ID",
        "NUM",
        "EOL",
        "ERR",
        "END",
        "LABEL",
        "JMP_EQ",
        "JMP_GT",
        "JMP_GE",
        "JMP_LT",
        "JMP_LE",
        "GOTO",
        "SKIP",
        "POP",
        "DUP",
        "SWAP",
        "ADD",
        "SUB",
        "MUL",
        "DIV",
        "STORE",
        "LOAD",
        "PUSH"
};

std::unordered_map<std::string, Type> hashtable {
    {"jmpeq", JMP_EQ},
    {"jmpgt", JMP_GT},
    {"jmpge", JMP_GE},
    {"jmplt", JMP_LT},
    {"jmple", JMP_LE},
    { "goto", GOTO},
    {"skip", SKIP},
    {"pop", POP},
    {"dup", DUP},
    {"swap", SWAP},
    {"add", ADD},
    {"sub", SUB},
    {"mul", MUL},
    {"div", DIV},
    {"store", STORE},
    {"load", LOAD},
    {"push", PUSH}
};

class Token {
public:
    Type type;
    std::string lexeme;

    explicit Token(Type _type): type(_type), lexeme(){
    }

    explicit Token(Type _type, char c): Token(_type){
        lexeme = std::to_string(c);
    }

    explicit Token(Type _type, const std::string& source): Token(_type){
        lexeme = source;
    }
};


std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << token_names[token.type];
    if (!token.lexeme.empty()) {
        os << "(" << token.lexeme << ")";
    }
    return os;
}




#endif //LAB01_TOKEN_H
