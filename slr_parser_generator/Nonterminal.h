#pragma once
#include "Symbol.h"

class Nonterminal : public Symbol
{
public:
    Nonterminal();
    void operator =(const Symbol &rhs);
    void operator =(SententialFormPointer rhs);
    void operator =(const SententialForms *rhs);

private:
    const SententialForms *mSententialForms;
};
