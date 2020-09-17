#include "Global"

MU_USE_NAMESPACE

int main(int argc, char *argv[])
{
    GuiApplication app(argc, argv);
    MU_ASSERT2(mApp);

    return app.exec();
}
