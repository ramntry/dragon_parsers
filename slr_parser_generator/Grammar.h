#pragma once
#include "Nonterminal.h"

class Grammar
{
public:
    Grammar(const Nonterminal &startNonterminal);
    virtual ~Grammar() {}

    void test();
    void printRulesForNonterminal(const Nonterminal *nonterminal) const;

public:
    const Nonterminal *mStart;
};
