#include "FirstExample.h"

FirstExample::Parser::Parser(Lexer *lexer)
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
