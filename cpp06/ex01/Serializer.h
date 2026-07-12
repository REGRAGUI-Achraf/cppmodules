#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <stdint.h>
#include "Data.h"

// Serializer is a purely static utility class. It cannot be instantiated,
// copied, moved, or destroyed by the user in any way — it only exists to
// group together the two static conversion functions below.
class Serializer {
public:
    // Convert a Data* pointer into its raw integer representation.
    static uintptr_t serialize(Data* ptr);

    // Convert a raw integer representation back into a Data* pointer.
    static Data* deserialize(uintptr_t raw);

private:
    // Prevent construction, copying and assignment in C++98 style.
    Serializer();
    ~Serializer();
    Serializer(const Serializer&);
    Serializer& operator=(const Serializer&);
};

#endif // SERIALIZER_H
