#include <boost/container/vector.hpp>
#include <chrono>
#include <format>
#include <iostream>
#include <locale>
#include <typeindex>
#include <vector>

int main()
{
    int intValue = 42;
    double doubleValue = 3.14;

    std::type_index intTypeIndex(typeid(intValue));
    std::type_index doubleTypeIndex = std::type_index(typeid(doubleValue));

    std::cout << "int type name: " << intTypeIndex.name() << std::endl;
    std::cout << "double type name: " << doubleTypeIndex.name() << std::endl;

    boost::container::vector<int> v{1, 2, 3};
    std::cout << v.at(2) << std::endl;
}

// std::type_identity;
// std::typeid;
//
