#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <stack>

// Note: Token `TokenCounter` should be the last in the enum
enum Token {
    Number = 0
    , Plus
    , Prod
    , LBrace
    , RBrace
    , EOF
    , ERR
    , TokenCounter
};

// Note: Nonterminal `Start` should be the first in the enum and equals the Token `TokenCounter`
// Note: Nonterminal `NonterminalCounter` should be the last in the enum
enum Nonterminal {
    Expr = TokenCounter
    , ExprTail
    , Term
    , TermTail
    , Factor
    , SymbolCounter
};

class Lexer
{
public:
    Lexer(std::istream &input);
    Token makeStep();
    Token getNext();
    bool nextIs(Token token);
    int nextNumberValue();

private:
    Token retrieveNextToken();

    std::istream &mInput;
    int mNextNumberValue;
    Token mNextToken;
};

Lexer::Lexer(std::istream &input)
    : mInput(input)
    , mNextToken(retrieveNextToken())
{
}

int Lexer::nextNumberValue()
{
    return mNextNumberValue;
}

bool Lexer::nextIs(Token token)
{
    return token == mNextToken;
}

Token Lexer::makeStep()
{
    Token result = mNextToken;
    mNextToken = retrieveNextToken();
    return result;
}

Token Lexer::getNext()
{
    return mNextToken;
}

Token Lexer::retrieveNextToken()
{
    mNextNumberValue = -1;

    char nextChar = 0;
    mInput >> nextChar;
    if (!mInput) {
        return EOF;
    }

    switch (nextChar)
    {
    case '+': return Plus;
    case '*': return Prod;
    case '(': return LBrace;
    case ')': return RBrace;
    default:
        if (isdigit(nextChar)) {
            mInput.putback(nextChar);
            mInput >> mNextNumberValue;
            return Number;
        }
        return ERR;
    }
}

void LexerTest()
{
    std::istringstream input("13 + 31 -");
    Lexer lexer(input);
    std::cout << lexer.nextIs(Number) << '\n';
    std::cout << (lexer.nextNumberValue() == 13) << '\n';
    std::cout << (lexer.makeStep() == Number) << '\n';
    std::cout << lexer.nextIs(Plus) << '\n';
    std::cout << (lexer.makeStep() == Plus) << '\n';
    std::cout << lexer.nextIs(Number) << '\n';
    std::cout << (lexer.nextNumberValue() == 31) << '\n';
    std::cout << (lexer.makeStep() == Number) << '\n';
    std::cout << lexer.nextIs(ERR) << '\n';
    std::cout << (lexer.makeStep() == ERR) << '\n';
    std::cout << lexer.nextIs(EOF) << '\n';
}

namespace ParseLib {

class ParserAction
{
public:
    virtual bool execute(std::stack<int> &symbolStack, Lexer &lexer) = 0;

protected:
    bool push(std::stack<int> &symbolStack, int symbol1);
    bool push(std::stack<int> &symbolStack, int symbol1, int symbol2);
    bool push(std::stack<int> &symbolStack, int symbol1, int symbol2, int symbol3);
};

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

class ErrorAction : public ParserAction
{
public:
    bool execute(std::stack<int> &, Lexer &) { return false; }
};

class EmptyAction : public ParserAction
{
public:
    bool execute(std::stack<int> &symbolStack, Lexer &) { symbolStack.pop(); return true; }
};

class TokenAction : public ParserAction
{
public:
    bool execute(std::stack<int> &symbolStack, Lexer &lexer);
};

bool TokenAction::execute(std::stack<int> &symbolStack, Lexer &lexer)
{
    bool match = symbolStack.top() == lexer.getNext();
    if (!match) {
        return false;
    }
    symbolStack.pop();
    lexer.makeStep();
    return true;
}

class ParserTable
{
public:
    ParserAction *at(int symbol, Token token);
    void fillTable(int numofSymbols, int numofTokens, ParserAction *action);
    void fillSymbol(int symbol, ParserAction *action);
    void setSpecialCase(int symbol, Token token, ParserAction *action);

private:
    typedef std::vector<ParserAction *> RowForTokens;
    typedef std::vector<RowForTokens> Table;
    Table mTable;
};

ParserAction *ParserTable::at(int symbol, Token token)
{
    return mTable.at(symbol).at(token);
}

void ParserTable::fillTable(int numofSymbols, int numofTokens, ParserAction *action)
{
    Table filledTable(numofSymbols, RowForTokens(numofTokens, action));
    mTable.swap(filledTable);
}

void ParserTable::setSpecialCase(int symbol, Token token, ParserAction *action)
{
    mTable.at(symbol).at(token) = action;
}

void ParserTable::fillSymbol(int symbol, ParserAction *action)
{
    RowForTokens &rowForTokens = mTable.at(symbol);
    fill(rowForTokens.begin(), rowForTokens.end(), action);
}

class ParseEngine
{
public:
    ParseEngine(Lexer *lexer, ParserTable *table);
    bool parse(int startSymbol);

private:
    std::stack<int> mSymbolStack;
    Lexer *mLexer;
    ParserTable *mTable;
};

ParseEngine::ParseEngine(Lexer *lexer, ParserTable *table)
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

} // ParseLib

class Parser
{
public:
    Parser(Lexer *lexer);
    ~Parser();
    bool parse();

private:
    class ExprAction;
    class ExprTailAction;
    class TermAction;
    class TermTailAction;
    class FactorExprAction;
    class FactorNumberAction;

    ExprAction *mExpr;
    ExprTailAction *mExprTail;
    TermAction *mTerm;
    TermTailAction *mTermTail;
    FactorExprAction *mFactorExpr;
    FactorNumberAction *mFactorNumber;
    ParseLib::EmptyAction *mEmpty;
    ParseLib::ErrorAction *mError;
    ParseLib::TokenAction *mToken;

    ParseLib::ParserTable *mTable;
    ParseLib::ParseEngine *mEngine;
};

class Parser::ExprAction : public ParseLib::ParserAction
{
public:
    bool execute(std::stack<int> &symbolStack, Lexer &)
    {
        symbolStack.pop();
        return push(symbolStack, Term, ExprTail);
    }
};

class Parser::ExprTailAction : public ParseLib::ParserAction
{
public:
    bool execute(std::stack<int> &symbolStack, Lexer &)
    {
        symbolStack.pop();
        return push(symbolStack, Plus, Term, ExprTail);
    }
};

class Parser::TermAction : public ParseLib::ParserAction
{
public:
    bool execute(std::stack<int> &symbolStack, Lexer &)
    {
        symbolStack.pop();
        return push(symbolStack, Factor, TermTail);
    }
};

class Parser::TermTailAction : public ParseLib::ParserAction
{
public:
    bool execute(std::stack<int> &symbolStack, Lexer &)
    {
        symbolStack.pop();
        return push(symbolStack, Prod, Factor, TermTail);
    }
};

class Parser::FactorExprAction : public ParseLib::ParserAction
{
public:
    bool execute(std::stack<int> &symbolStack, Lexer &)
    {
        symbolStack.pop();
        return push(symbolStack, LBrace, Expr, RBrace);
    }
};

class Parser::FactorNumberAction : public ParseLib::ParserAction
{
public:
    bool execute(std::stack<int> &symbolStack, Lexer &)
    {
        symbolStack.pop();
        return push(symbolStack, Number);
    }
};

Parser::Parser(Lexer *lexer)
    : mExpr(new ExprAction)
    , mExprTail(new ExprTailAction)
    , mTerm(new TermAction)
    , mTermTail(new TermTailAction)
    , mFactorExpr(new FactorExprAction)
    , mFactorNumber(new FactorNumberAction)
    , mEmpty(new ParseLib::EmptyAction)
    , mError(new ParseLib::ErrorAction)
    , mToken(new ParseLib::TokenAction)
    , mTable(new ParseLib::ParserTable)
{
    mTable->fillTable(SymbolCounter, TokenCounter, mError);

    mTable->fillSymbol(Expr, mExpr);
    mTable->fillSymbol(ExprTail, mEmpty);
    mTable->setSpecialCase(ExprTail, Plus, mExprTail);

    mTable->fillSymbol(Term, mTerm);
    mTable->fillSymbol(TermTail, mEmpty);
    mTable->setSpecialCase(TermTail, Prod, mTermTail);

    mTable->setSpecialCase(Factor, LBrace, mFactorExpr);
    mTable->setSpecialCase(Factor, Number, mFactorNumber);

    for (int currentToken = 0; currentToken < TokenCounter; ++currentToken) {
        mTable->fillSymbol(currentToken, mToken);
    }

    mEngine = new ParseLib::ParseEngine(lexer, mTable);
}

Parser::~Parser()
{
    delete mEngine;
    delete mTable;

    delete mToken;
    delete mError;
    delete mEmpty;
    delete mFactorNumber;
    delete mFactorExpr;
    delete mTermTail;
    delete mTerm;
    delete mExprTail;
    delete mExpr;
}

bool Parser::parse()
{
    return mEngine->parse(Expr);
}

int main()
{
    std::istringstream input("42 + 10*(2 + 2 * 3) + 7");
    Lexer lexer(input);
    Parser parser(&lexer);
    std::cout << std::boolalpha << parser.parse() << std::endl;
}
