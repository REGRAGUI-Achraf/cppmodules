#include <iostream>
#include <cassert>
#include "Data.h"
#include "Serializer.h"

int main() {
    Data* original = new Data(42);

    uintptr_t raw = Serializer::serialize(original);

    std::cout << "Original pointer:   " << original << std::endl;
    std::cout << "Serialized value:   " << raw << std::endl;

    Data* restored = Serializer::deserialize(raw);

    std::cout << "Deserialized pointer: " << restored << std::endl;


    delete original;


    return 0;
}
