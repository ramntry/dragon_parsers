#include "ParserLib.h"

using namespace ParseLib;

bool ParserAction::push(std::stack<int> &symbolStack, int symbol1)
{
    symbolStack.push(symbol1);
    return true;
}

bool ParserAction::push(std::stack<int> &symbolStack, int symbol1, int symbol2)
{
    push(symbolStack, symbol2);
    return push(symbolStack, symbol1);
}

bool ParserAction::push(std::stack<int> &symbolStack, int symbol1, int symbol2, int symbol3)
{
    push(symbolStack, symbol3);
    return push(symbolStack, symbol1, symbol2);
}

bool TokenAction::execute(std::stack<int> &symbolStack, LexerInterface &lexer)
{
    bool match = symbolStack.top() == lexer.getNext();
    if (!match) {
        return false;
    }
    symbolStack.pop();
    lexer.makeStep();
    return true;
}

bool NonterminalAction::execute(std::stack<int> &symbolStack, LexerInterface &)
{
    symbolStack.pop();
    return executeAfterPop(symbolStack);
}

ParserAction *ParserTable::at(int symbol, int token)
{
    return mTable.at(symbol).at(token);
}

void ParserTable::fillTable(int numofSymbols, int numofTokens, ParserAction *action)
{
    Table filledTable(numofSymbols, RowForTokens(numofTokens, action));
    mTable.swap(filledTable);
}

void ParserTable::setSpecialCase(int symbol, int token, ParserAction *action)
{
    mTable.at(symbol).at(token) = action;
}

void ParserTable::fillSymbol(int symbol, ParserAction *action)
{
    RowForTokens &rowForTokens = mTable.at(symbol);
    fill(rowForTokens.begin(), rowForTokens.end(), action);
}

ParseEngine::ParseEngine(LexerInterface *lexer, ParserTable *table)
    : mLexer(lexer)
    , mTable(table)
{
}

bool ParseEngine::parse(int startSymbol)
{
    mSymbolStack.push(startSymbol);
    while (!mLexer->nextIs(ERR) && !mSymbolStack.empty()) {
        ParserAction *action = mTable->at(mSymbolStack.top(), mLexer->getNext());
        bool noSyntaxError = action->execute(mSymbolStack, *mLexer);
        if (!noSyntaxError) {
            return false;
        }
    }
    return mLexer->nextIs(EOF) && mSymbolStack.empty();
}

ParserBase::ParserBase()
    : mTable(new ParserTable)
    , mEngine(0)
{
}

void ParserBase::initEngine(LexerInterface *lexer)
{
    mEngine = new ParseEngine(lexer, mTable);
}

void ParserBase::initTokens(int tokenCounter)
{
    for (int currentToken = ParseLib::FirstStandardToken; currentToken < tokenCounter; ++currentToken) {
        mTable->fillSymbol(currentToken, &mToken);
    }
}

bool ParserBase::parse()
{
    return mEngine->parse(startSymbol());
}

ParserBase::~ParserBase()
{
    delete mEngine;
    delete mTable;
}
