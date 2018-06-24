#include "../src/tinyini.hpp"

int main()
{
    tinyini::Section ini1 = tinyini::Parser::sectionFromFile("./test.ini");
    try
    {
        std::cout << ini1("AUTHOR")["name"] << std::endl;
        std::cout << ini1("DESCRIPTION")["project"] << std::endl;
        std::cout << ini1("AUTHOR")["nam"] << std::endl;
        ini1("AUTHOR")["age"] = "18";
        ini1.writeToFile("test2.ini");
    }
    catch (tinyini::KeyError &error)
    {
        std::cout << error.what() << std::endl;
    }

    try
    {
        tinyini::Section ini2 = tinyini::Parser::sectionFromFile("./test3.ini");
    }
    catch (tinyini::SyntaxError &error)
    {
        std::cout << error.what() << std::endl;
    }

    try
    {
        tinyini::Section ini3 = tinyini::Parser::sectionFromFile("./test4.ini");
        std::cout << ini3("ROOT")("CHILD")["info"] << std::endl;
    }
    catch (tinyini::KeyError &error)
    {
        std::cout << error.what() << std::endl;
    }

    return 0;
}