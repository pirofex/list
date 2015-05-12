#ifndef LISTMANAGER_H
#define LISTMANAGER_H

#include <iostream>
#include <vector>

#include <tntdb.h>

#include "Entry.h"

class ListManager
{
public:
    ListManager();
    std::vector<Entry> getEntries();
    void addEntry(const std::string&);

private:
    tntdb::Connection conn;
};

#endif // LISTMANAGER_H
