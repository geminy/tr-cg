/**
  * @file GuiApplicationPrivate.h
  */

#ifndef MU_GUIAPPLICATIONPRIVATE_H
#define MU_GUIAPPLICATIONPRIVATE_H

#include "../../core/kernel/ApplicationPrivate.h"
#include "GuiApplication.h"

MU_BEGIN_NAMESPACE

/**
 * @class GuiApplicationPrivate
 */
class MU_EXPORT GuiApplicationPrivate : public ApplicationPrivate
{
public:
    GuiApplicationPrivate(int &argc, char **argv);
    ~GuiApplicationPrivate();

public:
    MU_DECLARE_PUBLIC(GuiApplication)
};

MU_END_NAMESPACE

#endif // MU_GUIAPPLICATIONPRIVATE_H
