# Anleitung Live Demo

Wir wollen eine kleine Webapplikation schreiben in welcher wir so viele Einträge in eine List einfügen können wie wir wollen. Die Einträge werden in einer Datenbank gespeichert.

Die Makefiles sind bereits gegeben, sie werden benötigt, um das Projekt zu kompilieren.
Im /src Ordner befindet sich der gesamte Source Code.

## Modellierung des Listeneintrags

Wir benötigen eine Model-Klasse für die Einträge welche wir erstellen möchten. 
Diese sieht folgendermaßen aus:

```
#ifndef ENTRY_H
#define ENTRY_H

#include <strin>g
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
```

Wir erstellen einen *operator<<=* mit den angegebenen Parametern um die Möglichkeit zu haben das Objekt zu serialisieren (und später an den Client zu schicken). Zudem haben wir noch einen trivialen Getter welcher uns die Beschreibung des Eintrags als String zurückliefert. Der Konstruktor ist ziemlich simpel und nimmt ebenfalls einen *std::string* welcher die Beschreibung enthält.

## Implementierung der Datenbanklogik

Jetzt wo wir mit Einträgen arbeiten können machen wir uns doch gleich mal an die Datenbanklogik.
Die Headerfile sieht folgendermaßen aus:

```
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
```

Wir haben also eine Klasse *ListManager* in deren Konstruktor die Verbindung zur Datenbank aufgebaut wird. Außerdem kann man Einträge hinzufügen sowie bereits vorhandene abrufen.

Die Implementierung dieser Funktionen sieht so aus:

```
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

```

Der Code dürfte eigentlich selbsterklärend sein. In der *getEntries()* Funktion wird über die Ergebnismenge der *SELECT*-Abfrage iteriert, in einen *std::vector* kopiert und anschließend an den Aufrufer zurückgegeben.
In der *addEntry()* Funktion wird einfach ein neuer Eintrag in der Tabelle *entries* angelegt.

## Implementierung des Controllers

Wir müssen die Requests des Clients natürlich auch steuern können bzw. unsere Datenbankklasse auch irgendwie einsetzen können. Dafür schreiben wir uns zwei Controller.

```
#include <tnt/component.h>
#include <tnt/componentfactory.h>
#include <tnt/httprequest.h>
#include <tnt/httpreply.h>

#include "SerializationHelper.h"
#include "ListManager.h"

class AddListEntry
    : public tnt::Component
{
public:
    unsigned operator()(tnt::HttpRequest& request, tnt::HttpReply& reply, tnt::QueryParams& qparam) override
    {
        auto description = qparam.arg<std::string>("description");

        ListManager manager;
        manager.addEntry(description);

        return HTTP_OK;
    }
};

class GetListEntries
    : public tnt::Component
{
public:
    unsigned operator()(tnt::HttpRequest& request, tnt::HttpReply& reply, tnt::QueryParams& qparam) override
    {
        ListManager manager;
        auto entries = manager.getEntries();

        reply.out() << entries;
        return HTTP_OK;
    }
};

static tnt::ComponentFactoryImpl<GetListEntries> getlistentries("ajax/getentries");
static tnt::ComponentFactoryImpl<AddListEntry> addlistentry("ajax/addentry");
```
Der zweite Controller ist dafür zuständig alle bereits vorhandenen Einträge aus der Datenbank zu holen und an den Client als JSON zurückzugeben. Dafür brauchen wir noch diese Helferfunktion:

```
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
```

Dieser *operator<<* nimmt als linken Operanden einen *std::ostream*, welcher auch von *reply.out()* zurückgegeben wird und serialisiert das Objekt auf der rechten Seite, **wenn der dazugehörige *operator<<=* mit den passenden Parametern vorhanden ist. In unserem Fall ist er das, wie man in unserer Klasse *Entry* sehen kann. Zum Schluss wird das serialisierte Objekt in den *ostream* geschrieben was in unserem Fall bedeutet, dass der Client die Antwort bekommen hat.

Der erste Controller ist dafür zuständig neue Einträge die der Client gerne in die Datenbank schreiben würde zu nehmen und eben das zu tun - am Ende wird einfach HTTP 200 OK zurückgegeben.

Zum Schluss brauchen wir nur noch unsere *main()* Funktion in welcher wir die Applikation starten.

```
#include <tnt/tntnet.h>

int main()
{
    tnt::Tntnet app;

    app.mapUrl("/(.*)", "resources").setPathInfo("view/$1");
    app.mapUrl("^/(.*)", "$1");
    app.listen(8000);
    app.run();
}
```

Und das war's dann auch schon. Viel Spaß damit!
