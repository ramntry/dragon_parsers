#include <sstream>
#include "Lexer.h"

using namespace SimpleLexer;

Lexer::Lexer(std::istream &input)
    : mInput(input)
    , mNextToken(retrieveNextToken())
{
}

int Lexer::makeStep()
{
    int result = mNextToken;
    mNextToken = retrieveNextToken();
    return result;
}

int Lexer::retrieveNextToken()
{
    mNextNumberValue = -1;

    char nextChar = 0;
    mInput >> nextChar;
    if (!mInput) {
        return ParseLib::EOF;
    }

    switch (nextChar)
    {
    case '+': return Plus;
    case '-': return Minus;
    case '*': return Prod;
    case '/': return Slash;
    case '(': return LBrace;
    case ')': return RBrace;
    default:
        if (isdigit(nextChar)) {
            mInput.putback(nextChar);
            mInput >> mNextNumberValue;
            return Number;
        }
        return ParseLib::ERR;
    }
}

bool firstTest()
{
    std::istringstream input("13 + 31 #");
    Lexer lexer(input);
    bool result
          = lexer.nextIs(Number)
         && (lexer.nextNumberValue() == 13)
         && (lexer.makeStep() == Number)
         && lexer.nextIs(Plus)
         && (lexer.makeStep() == Plus)
         && lexer.nextIs(Number)
         && (lexer.nextNumberValue() == 31)
         && (lexer.makeStep() == Number)
         && lexer.nextIs(ParseLib::ERR)
         && (lexer.makeStep() == ParseLib::ERR)
         && lexer.nextIs(ParseLib::EOF);
    return result;
}

bool secondTest()
{
    bool result = true;
    std::istringstream input("/-");
    Lexer lexer(input);

    result &= lexer.makeStep() == Slash;
    result &= lexer.nextIs(Minus);

    return result;
}

bool SimpleLexer::lexerTest()
{
    return firstTest() && secondTest();
}

