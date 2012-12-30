#include <iostream>
#include <sstream>
#include <string>

using std::istream;
using std::istringstream;
using std::string;

enum Token {
    Plus = '+'
    , Prod = '*'
    , LBrace = '('
    , RBrace = ')'
    , Id = 'a'
};

bool nextIs(Token token, istream &in)
{
    return in.peek() == token;
}

bool checkAndGet(Token token, istream &in)
{
    return in.get() == token;
}

bool isEmpty(istream &in)
{
    return in.get() && !in;
}

bool expr(istream &in);

bool factor(istream &in)
{
    if (nextIs(LBrace, in)) {
        return in.get() && expr(in) && checkAndGet(RBrace, in);
    }
    return checkAndGet(Id, in);
}

bool termTail(istream &in)
{
    if (nextIs(Prod, in)) {
        return in.get() && factor(in) && termTail(in);
    }
    return true;
}

bool term(istream &in)
{
    return factor(in) && termTail(in);
}

bool exprTail(istream &in)
{
    if (nextIs(Plus, in)) {
        return in.get() && term(in) && exprTail(in);
    }
    return true;
}

bool expr(istream &in)
{
    return term(in) && exprTail(in);
}

bool parse(istream &in)
{
    return expr(in) && isEmpty(in);
}

int main()
{
    istringstream in("a+a*a*(a+a+a)+a*(a)");
    std::cout << std::boolalpha << parse(in) << std::endl;
}
