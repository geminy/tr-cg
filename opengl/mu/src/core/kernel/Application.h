/**
  * @file Application.h
  */

#ifndef MU_APPLICATION_H
#define MU_APPLICATION_H

#include "Object.h"
#include "Singleton.h"
#include "CoreEvent.h"
#include "EventLoop.h"

MU_BEGIN_NAMESPACE

class Application;
class ApplicationPrivate;
class AbstractEventDispatcher;

typedef std::list<std::string> StringList;

/**
 * @class Application
 */
class MU_EXPORT Application : public Object, public WeakSingleton<Application>
{
public:
    Application(int &argc, char **argv);
    ~Application();

    static StringList arguments();
    static std::string appName();
    static std::string appDirPath();
    static std::string appFilePath();
    static int appPid();

    static int exec();
    static void processEvents(EventLoop::EventLoopFlag flag = EventLoop::AllEvents, int maxTime = -1);
    static void exit(int returnCode = 0);

    static bool sendEvent(Object *receiver, Event *event);
    static void postEvent(Object *receiver, Event *event, int priority = Event::EventPrio_Normal);
    static void sendPostedEvents(Object *receiver = NULL, int type = 0);
    static void removePostedEvents(Object *receiver, int type = 0);

    static AbstractEventDispatcher* eventDispatcher();
    static void setEventDispatcher(AbstractEventDispatcher *eventDispatcher);

    bool event(Event *event);

    bool notify(Object *receiver, Event *event);

protected:
    Application(ApplicationPrivate &dd);

private:
    MU_DISABLE_COPY(Application)
    MU_DECLARE_PRIVATE(Application)
};

MU_END_NAMESPACE

#define mApp Application::Instance()

#endif // MU_APPLICATION_H
