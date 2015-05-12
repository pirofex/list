#ifndef ENTRY_H
#define ENTRY_H

#include <string>
#include <cxxtools/serializationinfo.h>

class Entry
{
public:
    Entry(const std::string& description)
        : description(description)
    {

    }

    inline const std::string& getDescription() const
    {
        return description;
    }

    friend void operator<<=(cxxtools::SerializationInfo& si, const Entry& entry)
    {
        si.addMember("description") <<= entry.description;
    }

private:
    std::string description;
};


#endif // ENTRY_H
