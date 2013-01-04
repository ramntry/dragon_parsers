#include "Terminal.h"
#include "Nonterminal.h"

#define UNUSED_VAR(var) for (; &var - &var; )

void simpleTest();

struct CalcGrammar
{
    CalcGrammar()
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
};

int main()
{
    simpleTest();
    CalcGrammar grammar;

    UNUSED_VAR(grammar);
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

    r = f | g;
    s = h >> j >> h | q;
}
