#include "Symbol.h"

int Symbol::mIdCounter = 0;

Symbol::Symbol()
    : mId(++mIdCounter)
{
}

Symbol::SententialFormPointer operator >>(const Symbol &lhs, const Symbol &rhs)
{
    Symbol::SententialForm *result = new Symbol::SententialForm;
    result->push_back(&lhs);
    result->push_back(&rhs);
    return result;
}

Symbol::SententialFormPointer operator >>(Symbol::SententialFormPointer lhs, const Symbol &rhs)
{
    lhs.m->push_back(&rhs);
    return lhs;
}

Symbol::SententialForms *operator |(Symbol::SententialFormPointer lhs, const Symbol::SententialFormPointer rhs)
{
    Symbol::SententialForms *result = new Symbol::SententialForms;
    result->push_back(lhs.m);
    result->push_back(rhs.m);
    return result;
}

Symbol::SententialForms *operator |(Symbol::SententialFormPointer lhs, const Symbol &rhs)
{
    Symbol::SententialForm *rhsForm = new Symbol::SententialForm;
    rhsForm->push_back(&rhs);

    return lhs | rhsForm;
}

//Symbol::SententialForms *operator |(const Symbol &lhs, const Symbol &rhs)
//{
//    Symbol::SententialForm *lhsForm = new Symbol::SententialForm;
//    lhsForm->push_back(&lhs);

//    Symbol::SententialForm *rhsForm = new Symbol::SententialForm;
//    rhsForm->push_back(&rhs);

//    return lhsForm | rhsForm;
//}
