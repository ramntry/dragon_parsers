#pragma once

namespace ParseLib {

enum StandardToken {
    FirstStandardToken = 0
    , ERR = FirstStandardToken
    , EOF
    , StandardTokenCounter
};

class LexerInterface
{
public:
    virtual int makeStep() = 0;
    virtual int getNext() = 0;
    virtual bool nextIs(int token) = 0;
};

} // namespace ParseLib
