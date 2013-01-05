#include <iostream>
#include <typeinfo>
#include <set>
#include "Grammar.h"

Grammar::Grammar(const Nonterminal &startNonterminal)
    : mStart(&startNonterminal)
{
}

void Grammar::printFormsForNonterminal(const Nonterminal *nonterminal) const
{
    static std::set<int> alreadyPrinted;
    static int absoluteCounter = 0;
    alreadyPrinted.insert(nonterminal->id());
    std::vector<const Nonterminal *> toPrint;

    std::size_t numofForms= nonterminal->sententialForms().size();
    for (std::size_t r = 0; r < numofForms; ++r) {
        std::cout << "Form No (" << absoluteCounter++ << ", " << r << ") for n" << nonterminal->id() << ":";
        Symbol::FormIterator formEnd = nonterminal->formEndAt(r);
        for (Symbol::FormIterator formIt = nonterminal->formBeginAt(r); formIt != formEnd; ++formIt) {
            const Nonterminal *nonterminal = (**formIt).getAsNonterminal();
            if (nonterminal) {
                std::cout << " n";
                if (alreadyPrinted.find(nonterminal->id()) == alreadyPrinted.end()) {
                    toPrint.push_back(nonterminal);
                    alreadyPrinted.insert(nonterminal->id());
                }
            } else {
                std::cout << " t";
            }
            std::cout << (**formIt).id();
        }
        std::cout << "\n";
    }

    std::size_t sizeToPrint = toPrint.size();
    for (std::size_t i = 0; i < sizeToPrint; ++i) {
        printFormsForNonterminal(toPrint[i]);
    }
    alreadyPrinted.clear();
    absoluteCounter = 0;
}

void Grammar::test()
{
    std::cout << "Test grammar with typeid.name " << typeid(*this).name() << "\n";
    printFormsForNonterminal(mStart);
    std::cout << std::endl;
}
