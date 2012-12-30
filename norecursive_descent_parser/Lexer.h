#pragma once
#include <iostream>
#include "LexerInterface.h"

namespace SimpleLexer {

// Note: the first Token should be equals `ParseLib::StandardTokenCounter`
// Note: Token `TokenCounter` should be the last in the enum
enum Token {
    Number = ParseLib::StandardTokenCounter
    , Plus
    , Prod
    , LBrace
    , RBrace
    , TokenCounter
};

class Lexer : public ParseLib::LexerInterface
{
public:
    Lexer(std::istream &input);
    int makeStep();
    int getNext() { return mNextToken; }
    bool nextIs(int token) { return token == mNextToken; }
    int nextNumberValue() { return mNextNumberValue; }

private:
    int retrieveNextToken();

    std::istream &mInput;
    int mNextNumberValue;
    int mNextToken;
};

bool lexerTest();

}
