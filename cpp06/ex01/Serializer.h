#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <stdint.h>
#include "Data.h"


class Serializer {
public:
    static uintptr_t serialize(Data* ptr);

    static Data* deserialize(uintptr_t raw);

private:
    Serializer();
    ~Serializer();
    Serializer(const Serializer&);
    Serializer& operator=(const Serializer&);
};

#endif 
