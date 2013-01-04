#include "Nonterminal.h"

int main()
{
    Symbol a;
    Symbol b;
    Symbol c;
    Symbol d;

    Nonterminal e;
    Nonterminal f;
    Nonterminal g;

    e = a >> b >> c >> d;
    f = a >> b | c >> d;
    g = a >> c | b;

    return 0;
}
