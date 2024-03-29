#pragma once
#include <vector>

class Terminal;
class Nonterminal;

class Symbol
{
public:
    typedef std::vector<const Symbol *> SententialForm;
    typedef std::vector<const SententialForm *> SententialForms;
    struct SententialFormPointer;

    typedef SententialForm::const_iterator FormIterator;
    typedef SententialForms::const_iterator FormsIterator;

    Symbol();
    virtual ~Symbol();

    int id() const { return mId; }

    operator SententialFormPointer() const;
    const Nonterminal *asNonterminal() const;
    const Terminal *asTerminal() const;

private:
    static int mIdCounter;

    int mId;
};

struct Symbol::SententialFormPointer
{
    SententialForm *const m;
    SententialFormPointer(SententialForm *pointer) : m(pointer) {}
    operator SententialForms *() const;
};

Symbol::SententialFormPointer operator >>(Symbol::SententialFormPointer lhs, const Symbol &rhs);
Symbol::SententialForms *operator |(Symbol::SententialFormPointer lhs, const Symbol::SententialFormPointer rhs);
Symbol::SententialForms *operator |(Symbol::SententialForms *lhs, const Symbol::SententialFormPointer rhs);
