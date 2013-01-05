#pragma once
#include "Symbol.h"

class Nonterminal : public Symbol
{
public:
    Nonterminal();
    ~Nonterminal();

    void operator =(const Symbol &rhs);
    void operator =(const SententialForms *rhs);

    const SententialForms &sententialForms() const { return *mSententialForms; }
    const SententialForm &sententialFormAt(std::size_t index) const { return *sententialForms().at(index); }
    FormIterator formBeginAt(std::size_t index) const { return sententialFormAt(index).begin(); }
    FormIterator formEndAt(std::size_t index) const { return sententialFormAt(index).end(); }

private:
    const SententialForms *mSententialForms;
};
