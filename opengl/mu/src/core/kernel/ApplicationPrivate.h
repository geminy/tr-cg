/**
  * @file ApplicationPrivate.h
  */

#ifndef MU_APPLICATIONPRIVATE_H
#define MU_APPLICATIONPRIVATE_H

#include "ObjectPrivate.h"
#include "Application.h"

MU_BEGIN_NAMESPACE

class PlatformIntegration;

/**
 * @class ApplicationPrivate
 */
class MU_EXPORT ApplicationPrivate : public ObjectPrivate
{
public:
    ApplicationPrivate(int &argc, char **argv);
    ~ApplicationPrivate();

    static bool CheckInstance(const char *func);
    static void CheckReceiverThread(Object *receiver);
    static void CheckMainThread();
    static void InitLocale();
    static bool notify(Object *receiver, Event *event);
    static void removePostedEvent(Event *event);

    void processCommandLineArguments();

    StringList arguments() const;
    std::string appName() const;
    std::string appDirPath() const;
    std::string appFilePath() const;

    int appPid() const;

    void createEventDispatcher();

    void createPlatformIntegration();

    void init();
    void cleanupThreadData();

public:
    static AbstractEventDispatcher *mEventDispatcher;
    static bool mLocaleInitialized;
    static PlatformIntegration *mPlatformIntegration;

    int &mArgc;
    char **mArgv;
    bool mThreadDataClean;

public:
    MU_DECLARE_PUBLIC(Application)
};

MU_END_NAMESPACE

#endif // MU_APPLICATIONPRIVATE_H
