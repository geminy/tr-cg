#include "GuiApplicationPrivate.h"

MU_BEGIN_NAMESPACE

GuiApplication::GuiApplication(int &argc, char **argv)
    : Application(*new GuiApplicationPrivate(argc, argv))
{

}

GuiApplication::~GuiApplication()
{

}

//
//

GuiApplicationPrivate::GuiApplicationPrivate(int &argc, char **argv)
    : ApplicationPrivate(argc, argv)
{

}

GuiApplicationPrivate::~GuiApplicationPrivate()
{

}

MU_END_NAMESPACE
