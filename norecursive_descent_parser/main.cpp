#include <iostream>
#include <sstream>
#include "FirstExample.h"
#include "VerySimpleExample.h"

int main()
{
    if (!SimpleLexer::lexerTest()) {
        std::cerr << "Lexer Test Fail!" << std::endl;
        return 1;
    }

    std::istringstream input("42 + 10*(2 + 2 * 3) + 7");
    SimpleLexer::Lexer lexer(input);
    FirstExample::Parser parser(&lexer);
    std::cout << std::boolalpha << parser.parse() << std::endl;

    std::istringstream input2("(((()()(()())(()(()())))))");
    SimpleLexer::Lexer lexer2(input2);
    VerySimpleExample::Parser parser2(&lexer2);
    std::cout << std::boolalpha << parser2.parse() << std::endl;
}
