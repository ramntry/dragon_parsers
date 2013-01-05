#include <assert.h>
#include <iostream>
#include <typeinfo>
#include <set>
#include "Grammar.h"

Grammar::Grammar(const Nonterminal &startNonterminal)
    : mStart(&startNonterminal)
    , mInited(false)
{
}

void Grammar::printFormsForNonterminal(const Nonterminal *nonterminal) const
{
    static std::set<int> alreadyPrinted;
    static int absoluteCounter = 0;
    alreadyPrinted.insert(nonterminal->id());
    std::vector<const Nonterminal *> toPrint;

    std::size_t numofForms= nonterminal->sententialForms().size();
    for (std::size_t i = 0; i < numofForms; ++i) {
        std::cout << "Form No (" << absoluteCounter++ << ", " << i << ") for n" << nonterminal->id() << ":";
        Symbol::FormIterator formEnd = nonterminal->formEndAt(i);
        for (Symbol::FormIterator formIt = nonterminal->formBeginAt(i); formIt != formEnd; ++formIt) {
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

void Grammar::printAllFirstSets()
{
    init();
    IdToSet::const_iterator setEnd = mFirstSets.end();
    for (IdToSet::const_iterator setIt = mFirstSets.begin(); setIt != setEnd; ++setIt) {
        std::cout << "First set for n" << setIt->first << ":";
        IdSet::const_iterator end = setIt->second.end();
        for (IdSet::const_iterator it = setIt->second.begin(); it != end; ++it) {
            std::cout << " t" << *it;
        }
        std::cout << "\n";
    }
}

void Grammar::init()
{
    if (mInited) {
        return;
    }

    makeFirstSets();
    makeFollowSets();

    mInited = true;
}

void Grammar::makeFirstSets()
{
    makeFirstSetFor(mStart);
}

const Grammar::IdSet &Grammar::makeFirstSetFor(const Nonterminal *nonterminal)
{
    IdSet &target = mFirstSets[nonterminal->id()];
    if (!target.empty()) {
        return target;
    }
    std::size_t numofForms = nonterminal->sententialForms().size();
    for (std::size_t i = 0; i < numofForms; ++i) {
        const Symbol *firstSymbol = *nonterminal->formBeginAt(i);
        if (firstSymbol->getAsTerminal()) {
            target.insert(firstSymbol->id());
        } else {
            const IdSet &resultForFirstSymbol = makeFirstSetFor(firstSymbol->getAsNonterminal());
            target.insert(resultForFirstSymbol.begin(), resultForFirstSymbol.end());
        }
    }
    return target;
}

void Grammar::makeFollowSets()
{
}

bool Grammar::inFirstSet(int terminalId, int nonterminalId)
{
    init();
    IdToSet::const_iterator firstSetIt = mFirstSets.find(nonterminalId);
    assert(firstSetIt != mFirstSets.end());
    return firstSetIt->second.find(terminalId) != firstSetIt->second.end();
}

void Grammar::test()
{
    std::cout << "Test grammar with typeid.name " << typeid(*this).name() << "\n";
    printFormsForNonterminal(mStart);

    std::cout << "First sets:\n";
    printAllFirstSets();
    std::cout << std::endl;
}
