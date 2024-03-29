#pragma once
#include "ParseLib.h"
#include "Lexer.h"

// Example for grammar:
//     expr ::= expr '+' term | term
//     term ::= term '*' factor | factor
//     factor ::= `number` | '(' expr ')'

// which has been manualy transformed to LL(1)-grammar:
//     expr ::= term exprTail
//     exprTail ::= '+' term exprTail | `empty`
//     term ::= factor termTail
//     termTail ::= '*' factor termTail | `empty`
//     factor ::= `number` | '(' expr ')'

namespace FirstExample {

using namespace SimpleLexer;

// Note: the first Nonterminal should be equals the Token `TokenCounter`
// Note: Nonterminal `SymbolCounter` should be the last in the enum
enum Nonterminal {
    Expr = SimpleLexer::TokenCounter
    , ExprTail
    , Term
    , TermTail
    , Factor
    , SymbolCounter
};

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

} // namespace FirstExample
