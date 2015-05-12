#include <tnt/tntnet.h>

int main()
{
    tnt::Tntnet app;

    app.mapUrl("/(.*)", "resources").setPathInfo("view/$1");
    app.mapUrl("^/(.*)", "$1");
    app.listen(8000);
    app.run();
}
