#include <assert.h>
#include "Nonterminal.h"

Nonterminal::Nonterminal()
    : mSententialForms(0)
{
}

Nonterminal::~Nonterminal()
{
    if (mSententialForms == 0) {
        return;
    }
    FormsIterator formsEnd = mSententialForms->end();
    for (FormsIterator formsIt = mSententialForms->begin(); formsIt != formsEnd; ++formsIt) {
        delete *formsIt;
    }
    delete mSententialForms;
}

void Nonterminal::operator =(const Symbol &rhs)
{
    (*this) = static_cast<SententialFormPointer>(rhs);
}

void Nonterminal::operator =(const Symbol::SententialForms *rhs)
{
    assert(mSententialForms == 0);
    mSententialForms = rhs;
}

void Nonterminal::operator =(const Nonterminal &rhs)
{
    (*this) = static_cast<SententialFormPointer>(rhs);
}
