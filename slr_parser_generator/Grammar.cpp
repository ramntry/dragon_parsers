#include <iostream>
#include <typeinfo>
#include <set>
#include "Grammar.h"

Grammar::Grammar(const Nonterminal &startNonterminal)
    : mStart(&startNonterminal)
{
}

void Grammar::printRulesForNonterminal(const Nonterminal *nonterminal) const
{
    static std::set<int> alreadyPrinted;
    static int absoluteCounter = 0;
    alreadyPrinted.insert(nonterminal->id());
    std::vector<const Nonterminal *> toPrint;

    std::size_t numofRules = nonterminal->sententialForms().size();
    for (std::size_t r = 0; r < numofRules; ++r) {
        std::cout << "Rule No (" << absoluteCounter++ << ", " << r << ") for n" << nonterminal->id() << ":";
        Symbol::FormIterator ruleIt = nonterminal->sententialFormAt(r).begin();
        Symbol::FormIterator ruleEnd = nonterminal->sententialFormAt(r).end();
        for (; ruleIt != ruleEnd; ++ruleIt) {
            const Nonterminal *nonterminal = (**ruleIt).getAsNonterminal();
            if (nonterminal) {
                std::cout << " n";
                if (alreadyPrinted.find(nonterminal->id()) == alreadyPrinted.end()) {
                    toPrint.push_back(nonterminal);
                    alreadyPrinted.insert(nonterminal->id());
                }
            } else {
                std::cout << " t";
            }
            std::cout << (**ruleIt).id();
        }
        std::cout << "\n";
    }

    std::size_t sizeToPrint = toPrint.size();
    for (std::size_t i = 0; i < sizeToPrint; ++i) {
        printRulesForNonterminal(toPrint[i]);
    }
    alreadyPrinted.clear();
    absoluteCounter = 0;
}

void Grammar::test()
{
    std::cout << "Test grammar with typeid.name " << typeid(*this).name() << "\n";
    printRulesForNonterminal(mStart);
    std::cout << std::endl;
}
