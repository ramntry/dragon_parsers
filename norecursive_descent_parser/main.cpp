#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <stack>


// ParseLib starts

namespace ParseLib {

enum StandardToken {
    ERR = 0
    , EOF
    , StandardTokenCounter
};

class LexerInterface
{
public:
    virtual int makeStep() = 0;
    virtual int getNext() = 0;
    virtual bool nextIs(int token) = 0;
};

class ParserAction
{
public:
    virtual bool execute(std::stack<int> &symbolStack, LexerInterface &lexer) = 0;

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

class NonterminalAction : public ParserAction
{
public:
    virtual bool executeAfterPop(std::stack<int> &symbolStack) = 0;
    bool execute(std::stack<int> &symbolStack, LexerInterface &lexer);
};

bool NonterminalAction::execute(std::stack<int> &symbolStack, LexerInterface &)
{
    symbolStack.pop();
    return executeAfterPop(symbolStack);
}

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

class ParserBase
{
public:
    ParserBase();
    ~ParserBase();
    bool parse();

protected:
    virtual int startSymbol() = 0;
    void initEngine(LexerInterface *lexer);

    EmptyAction mEmpty;
    ErrorAction mError;
    TokenAction mToken;

    ParserTable *mTable;
    ParseEngine *mEngine;
};

ParserBase::ParserBase()
    : mTable(new ParserTable)
    , mEngine(0)
{
}

void ParserBase::initEngine(LexerInterface *lexer)
{
    mEngine = new ParseEngine(lexer, mTable);
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

// ParseLib ends


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

// Note: the first Nonterminal should be equals the Token `TokenCounter`
// Note: Nonterminal `SymbolCounter` should be the last in the enum
enum Nonterminal {
    Expr = TokenCounter
    , ExprTail
    , Term
    , TermTail
    , Factor
    , SymbolCounter
};

class Lexer : public ParseLib::LexerInterface
{
public:
    Lexer(std::istream &input);
    int makeStep();
    int getNext();
    bool nextIs(int token);
    int nextNumberValue();

private:
    int retrieveNextToken();

    std::istream &mInput;
    int mNextNumberValue;
    int mNextToken;
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

bool Lexer::nextIs(int token)
{
    return token == mNextToken;
}

int Lexer::makeStep()
{
    int result = mNextToken;
    mNextToken = retrieveNextToken();
    return result;
}

int Lexer::getNext()
{
    return mNextToken;
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
    case '*': return Prod;
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
    std::cout << lexer.nextIs(ParseLib::ERR) << '\n';
    std::cout << (lexer.makeStep() == ParseLib::ERR) << '\n';
    std::cout << lexer.nextIs(ParseLib::EOF) << '\n';
}

class Parser : public ParseLib::ParserBase
{
public:
    Parser(Lexer *lexer);

protected:
    int startSymbol() { return Expr; }

private:
    NewNonterminalAction        (ExprAction) Term, ExprTail         EndNA(mExpr);
    NewNonterminalAction    (ExprTailAction) Plus, Term, ExprTail   EndNA(mExprTail);
    NewNonterminalAction        (TermAction) Factor, TermTail       EndNA(mTerm);
    NewNonterminalAction    (TermTailAction) Prod, Factor, TermTail EndNA(mTermTail);
    NewNonterminalAction  (FactorExprAction) LBrace, Expr, RBrace   EndNA(mFactorExpr);
    NewNonterminalAction(FactorNumberAction) Number                 EndNA(mFactorNumber);
};

Parser::Parser(Lexer *lexer)
{
    mTable->fillTable(SymbolCounter, TokenCounter, &mError);

    mTable->fillSymbol(Expr, &mExpr);
    mTable->fillSymbol(ExprTail, &mEmpty);
    mTable->setSpecialCase(ExprTail, Plus, &mExprTail);

    mTable->fillSymbol(Term, &mTerm);
    mTable->fillSymbol(TermTail, &mEmpty);
    mTable->setSpecialCase(TermTail, Prod, &mTermTail);

    mTable->setSpecialCase(Factor, LBrace, &mFactorExpr);
    mTable->setSpecialCase(Factor, Number, &mFactorNumber);

    for (int currentToken = 0; currentToken < TokenCounter; ++currentToken) {
        mTable->fillSymbol(currentToken, &mToken);
    }

    initEngine(lexer);
}

int main()
{
    std::istringstream input("42 + 10*(2 + 2 * 3) + 7");
    Lexer lexer(input);
    Parser parser(&lexer);
    std::cout << std::boolalpha << parser.parse() << std::endl;
}
