#pragma once
#include <algorithm>
#include <vector>
#include <stack>

#include "LexerInterface.h"

namespace ParseLib {

class ParserAction
{
public:
    virtual bool execute(std::stack<int> &symbolStack, LexerInterface &lexer) = 0;

protected:
    bool push(std::stack<int> &symbolStack, int symbol1);
    bool push(std::stack<int> &symbolStack, int symbol1, int symbol2);
    bool push(std::stack<int> &symbolStack, int symbol1, int symbol2, int symbol3);
};

class ErrorAction : public ParserAction
{
public:
    bool execute(std::stack<int> &, LexerInterface &) { return false; }
};

class EmptyAction : public ParserAction
{
public:
    bool execute(std::stack<int> &symbolStack, LexerInterface &) { symbolStack.pop(); return true; }
};

class TokenAction : public ParserAction
{
public:
    bool execute(std::stack<int> &symbolStack, LexerInterface &lexer);
};

class NonterminalAction : public ParserAction
{
public:
    virtual bool executeAfterPop(std::stack<int> &symbolStack) = 0;
    bool execute(std::stack<int> &symbolStack, LexerInterface &lexer);
};

class ParserTable
{
public:
    ParserAction *at(int symbol, int token);
    void fillTable(int numofSymbols, int numofTokens, ParserAction *action);
    void fillSymbol(int symbol, ParserAction *action);
    void setSpecialCase(int symbol, int token, ParserAction *action);

private:
    typedef std::vector<ParserAction *> RowForTokens;
    typedef std::vector<RowForTokens> Table;
    Table mTable;
};

class ParseEngine
{
public:
    ParseEngine(LexerInterface *lexer, ParserTable *table);
    bool parse(int startSymbol);

private:
    std::stack<int> mSymbolStack;
    LexerInterface *mLexer;
    ParserTable *mTable;
};


class ParserBase
{
public:
    ParserBase();
    ~ParserBase();
    bool parse();
    void initTokens(int tokenCounter);

protected:
    virtual int startSymbol() = 0;
    void initEngine(LexerInterface *lexer);

    EmptyAction mEmpty;
    ErrorAction mError;
    TokenAction mToken;

    ParserTable *mTable;
    ParseEngine *mEngine;
};

} // namespace ParseLib

#define NewNonterminalAction(ActionName) \
class ActionName : public ParseLib::NonterminalAction \
{ \
public: \
    bool executeAfterPop(std::stack<int> &symbolStack) \
    { \
        return push(symbolStack,
#define EndNA(variableName) \
); \
    } \
} variableName
