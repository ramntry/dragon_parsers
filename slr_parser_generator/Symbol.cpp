#include "Nonterminal.h"
#include "Terminal.h"

int Symbol::mIdCounter = 0;

Symbol::Symbol()
    : mId(++mIdCounter)
{
}

Symbol::~Symbol()
{
    if (mId == mIdCounter) {
        --mIdCounter;
    }
}

Symbol::operator SententialFormPointer() const
{
    SententialForm *result = new SententialForm;
    result->push_back(this);
    return result;
}

Symbol::SententialFormPointer::operator SententialForms *() const
{
    SententialForms *result = new SententialForms;
    result->push_back(m);
    return result;
}

Symbol::SententialFormPointer operator >>(Symbol::SententialFormPointer lhs, const Symbol &rhs)
{
    lhs.m->push_back(&rhs);
    return lhs;
}

Symbol::SententialForms *operator |(Symbol::SententialForms *lhs, const Symbol::SententialFormPointer rhs)
{
    lhs->push_back(rhs.m);
    return lhs;
}

Symbol::SententialForms *operator |(Symbol::SententialFormPointer lhs, const Symbol::SententialFormPointer rhs)
{
    return static_cast<Symbol::SententialForms *>(lhs) | rhs;
}

const Nonterminal *Symbol::asNonterminal() const
{
    return dynamic_cast<const Nonterminal *>(this);
}

const Terminal *Symbol::asTerminal() const
{
    return dynamic_cast<const Terminal *>(this);
}
