#ifndef DATA_H
#define DATA_H

#include <string>

// A non-empty structure with several data members, used to test
// the Serializer class.
struct Data {
    int id;
    double value;
    std::string name;

    Data(int id_, double value_, const std::string& name_)
        : id(id_), value(value_), name(name_) {}
};

#endif // DATA_H
