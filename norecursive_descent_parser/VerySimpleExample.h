#pragma once
#include "Lexer.h"
#include "ParseLib.h"

namespace VerySimpleExample {

using namespace SimpleLexer;

enum Nonterminal {
    Start = TokenCounter
    , SymbolCounter
};

class Parser : public ParseLib::ParserBase
{
public:
    Parser(Lexer *lexer);

protected:
    int startSymbol() { return Start; }

private:
    NewNonterminalAction(StartAction) LBrace, Start, RBrace, Start EndNA(mStart);
};

} // namespace VerySimpleExample

