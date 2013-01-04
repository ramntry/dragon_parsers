#include "Nonterminal.h"

Nonterminal::Nonterminal()
    : mSententialForms(0)
{
}

void Nonterminal::operator =(const Symbol &rhs)
{
    SententialForm *sententialForm = new SententialForm;
    sententialForm->push_back(&rhs);

    (*this) = sententialForm;
}

void Nonterminal::operator =(SententialFormPointer rhs)
{
    SententialForms *sententialForms = new SententialForms;
    sententialForms->push_back(rhs.m);

    (*this) = sententialForms;
}

void Nonterminal::operator =(const Symbol::SententialForms *rhs)
{
    mSententialForms = rhs;
}

