#include "VerySimpleExample.h"

VerySimpleExample::Parser::Parser(SimpleLexer::Lexer *lexer)
{
    mTable->fillTable(SymbolCounter, TokenCounter, &mError);
    mTable->fillSymbol(Start, &mEmpty);
    mTable->setSpecialCase(Start, LBrace, &mStart);
    initTokens(TokenCounter);
    initEngine(lexer);
}
