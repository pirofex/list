#include "ListManager.h"

ListManager::ListManager()
    : conn(tntdb::connectCached("mysql:dbname=eportfolio user=root passwd="))
{
}

std::vector<Entry> ListManager::getEntries()
{
    auto result = conn.select("SELECT description FROM entries");

    std::vector<Entry> entries;

    for (auto it = result.begin(); it != result.end(); ++it)
    {
        auto row = *it;

        std::string description;
        row["description"].get(description);

        entries.emplace_back(description);
    }

    return entries;
}

void ListManager::addEntry(const std::string& description)
{
    auto stmt = conn.prepare("INSERT INTO entries (description) VALUES (:description)");
    stmt.set("description", description).execute();
}
