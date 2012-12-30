#include <iostream>
#include <sstream>
#include "FirstExample.h"

int main()
{
    std::istringstream input("42 + 10*(2 + 2 * 3) + 7");
    SimpleLexer::Lexer lexer(input);
    FirstExample::Parser parser(&lexer);
    std::cout << std::boolalpha << parser.parse() << std::endl;
}
