#include <iostream>
#include <cassert>
#include "Data.h"
#include "Serializer.h"

int main() {
    // Create a Data object on the heap so its address is stable.
    Data* original = new Data(42, 3.14159, "hello world");

    // Serialize the pointer to an unsigned integer.
    uintptr_t raw = Serializer::serialize(original);

    std::cout << "Original pointer:   " << original << std::endl;
    std::cout << "Serialized value:   " << raw << std::endl;

    // Deserialize the integer back into a Data* pointer.
    Data* restored = Serializer::deserialize(raw);

    std::cout << "Deserialized pointer: " << restored << std::endl;

    // Verify the round trip preserved the pointer exactly.
    assert(restored == original);
    std::cout << "Success: deserialize(serialize(ptr)) == ptr" << std::endl;

    // Also verify the restored pointer still refers to valid, correct data.
    assert(restored->id == 42);
    assert(restored->name == "hello world");
    std::cout << "Data contents verified through restored pointer:" << std::endl;
    std::cout << "  id    = " << restored->id << std::endl;
    std::cout << "  value = " << restored->value << std::endl;
    std::cout << "  name  = " << restored->name << std::endl;

    delete original;

    // Uncomment to confirm Serializer truly cannot be instantiated:
    // Serializer s; // compile error: constructor is deleted/private

    return 0;
}
