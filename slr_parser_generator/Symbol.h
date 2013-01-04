#pragma once
#include <vector>

class Symbol
{
public:
    typedef std::vector<const Symbol *> SententialForm;
    typedef std::vector<const SententialForm *> SententialForms;
    struct SententialFormPointer;

    Symbol();

private:
    static int mIdCounter;

    int mId;
};

struct Symbol::SententialFormPointer
{
    SententialForm *const m;
    SententialFormPointer(SententialForm *pointer) : m(pointer) {}
};

Symbol::SententialFormPointer operator >>(const Symbol &lhs, const Symbol &rhs);
Symbol::SententialFormPointer operator >>(Symbol::SententialFormPointer lhs, const Symbol &rhs);

Symbol::SententialForms *operator |(Symbol::SententialFormPointer lhs, const Symbol::SententialFormPointer rhs);
Symbol::SententialForms *operator |(Symbol::SententialFormPointer lhs, const Symbol &rhs);
