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
