#include <path.hpp>

int main()
{
	namespace pd = pensar_digital::cpplib;
	pd::Path p("c:/out/log.txt");
    try {
        std::cout << "Path to remove: " << p.string() << std::endl;
        if (p.exists()) {
            std::cout << "Path exists." << std::endl;
        }
        else {
            std::cout << "Path does not exist." << std::endl;
        }
        p.remove();
        std::cout << "Path removed successfully." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}