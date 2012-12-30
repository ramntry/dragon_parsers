#include <iostream>
#include <sstream>

#include "ParserLib.h"

// Exaple for grammar:
//     expr ::= expr '+' term | term
//     term ::= term '*' factor | factor
//     factor ::= `number` | '(' expr ')'

// which has been manualy transformed to grammar:
//     expr ::= term exprTail
//     exprTail ::= '+' term exprTail | `empty`
//     term ::= factor termTail
//     termTail ::= '*' factor termTail | `empty`
//     factor ::= `number` | `empty`


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
    int getNext() { return mNextToken; }
    bool nextIs(int token) { return token == mNextToken; }
    int nextNumberValue() { return mNextNumberValue; }

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

    initTokens(TokenCounter);
    initEngine(lexer);
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

int main()
{
    std::istringstream input("42 + 10*(2 + 2 * 3) + 7");
    Lexer lexer(input);
    Parser parser(&lexer);
    std::cout << std::boolalpha << parser.parse() << std::endl;
}
