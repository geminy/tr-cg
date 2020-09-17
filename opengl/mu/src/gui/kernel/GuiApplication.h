/**
  * @file GuiApplication.h
  */

#ifndef MU_GUIAPPLICATION_H
#define MU_GUIAPPLICATION_H

#include "../../core/kernel/Application.h"

MU_BEGIN_NAMESPACE

class GuiApplication;
class GuiApplicationPrivate;

/**
 * @class GuiApplication
 */
class MU_EXPORT GuiApplication : public Application
{
public:
    GuiApplication(int &argc, char **argv);
    ~GuiApplication();

private:
    MU_DISABLE_COPY(GuiApplication)
    MU_DECLARE_PRIVATE(GuiApplication)
};

MU_END_NAMESPACE

#endif // MU_GUIAPPLICATION_H
