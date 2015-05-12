#ifndef SERIALIZATIONHELPER_H
#define SERIALIZATIONHELPER_H

#include <cxxtools/jsonserializer.h>
#include <cxxtools/serializationinfo.h>

template <typename T>
std::ostream& operator<<(std::ostream& os, const T& response)
{
    cxxtools::JsonSerializer serializer(os);
    serializer.serialize(response).finish();

    return os;
}

#endif // SERIALIZATIONHELPER_H
