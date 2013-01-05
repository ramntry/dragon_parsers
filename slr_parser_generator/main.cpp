#include "Terminal.h"
#include "Nonterminal.h"
#include "Grammar.h"

void simpleTest();

struct CalcGrammar : public Grammar
{
    CalcGrammar() : Grammar(expr)
    {
        expr = term
             | expr >> plus >> term
             ;

        term = factor
             | term >> prod >> factor
             ;

        factor = number
               | lbrace >> expr >> rbrace
               ;
    }

    Terminal number, lbrace, rbrace, plus, prod;
    Nonterminal expr, term, factor;
}
grammar;

struct BracesGrammar : public Grammar
{
    BracesGrammar() : Grammar(s)
    {
        s = grammar.lbrace >> s >> grammar.rbrace >> s;
    }

    Nonterminal s;
}
grammar2;

struct SimpleLang : public Grammar
{
    SimpleLang() : Grammar(program)
    {
        program = decls
                ;

        decls = decl
              | decls >> decl
              ;

        decl = type >> id >> grammar.lbrace >> args >> grammar.rbrace >> stmt
             ;

        arg = type >> id
            ;

        args = arg
             | args >> comma >> arg
             ;

        stmt = expr >> semicolon
             | lcurl >> stmts >> rcurl
             | while_ >> grammar.lbrace >> expr >> grammar.rbrace >> stmt
             ;

        stmts = stmt
              | stmts >> stmt
              ;
    }

    Terminal expr, lcurl, rcurl, semicolon, while_, id, type, comma;
    Nonterminal program, stmt, stmts, decl, decls, arg, args;
};

int main()
{
    simpleTest();

    grammar2.test();
    grammar.test();

    SimpleLang langGrammar;
    langGrammar.test();

    return 0;
}


void simpleTest()
{
    Symbol a;
    Symbol b;
    Symbol c;
    Symbol d;

    Nonterminal e;
    Nonterminal f;
    Nonterminal g;
    Nonterminal h;
    Nonterminal i;
    Nonterminal j;
    Nonterminal k;
    Nonterminal l;
    Nonterminal m;
    Nonterminal n;
    Nonterminal o;
    Nonterminal p;
    Nonterminal q;
    Nonterminal r;
    Nonterminal s;
    Nonterminal t;
    Nonterminal u;

    e = a;
    f = a >> b;
    g = a >> b >> c;
    h = a >> b >> c >> d;

    i = a | b;
    j = a | b | c | d;

    k = a >> b | c;
    l = a | b >> c;
    m = a >> b | c >> d;
    n = a | b >> c | d;
    o = a >> b | c | d;
    p = a | b >> c | d >> a;
    q = a | b | c >> d | a | b >> c >> d;

    u = s;
    r = f | g;
    s = h >> j >> h | q;
}
