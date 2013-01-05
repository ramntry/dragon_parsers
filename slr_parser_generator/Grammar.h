#pragma once
#include <set>
#include <map>
#include "Nonterminal.h"

class Grammar
{
public:
    Grammar(const Nonterminal &startNonterminal);
    virtual ~Grammar() {}

    bool inFirstSet(int terminalId, int nonterminalId);

    void printFormsForNonterminal(const Nonterminal *nonterminal);
    void printAllFirstSets();

    void test();

private:
    typedef std::set<int> IdSet;
    typedef std::map<int, IdSet> IdToSet;

    void init();
    void makeFirstSets();
    void makeFollowSets();
    const Grammar::IdSet &makeFirstSetFor(const Nonterminal *nonterminal);

    const Nonterminal *mStart;

    bool mInited;
    IdToSet mFirstSets;
    IdToSet mFollowSets;
};
