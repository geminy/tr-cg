#include "ApplicationPrivate.h"
#include "EventDispatcher_Linux.h"
#include "EventLoop.h"
#include "../io/FileInfo.h"
#include "../io/Log.h"
#include "../io/Process.h"
#include "../thread/Thread.h"
#include "../thread/ThreadPrivate.h"
#include "../thread/ThreadPool.h"
#include "../tool/Locale.h"
#include "../../gui/text/Font.h"
#include "../../gui/kernel/Palette.h"
#include "../../gui/kernel/Cursor.h"

MU_BEGIN_NAMESPACE

Application::Application(int &argc, char **argv)
    : Object(*new ApplicationPrivate(argc, argv))
{
    setObjectName("Application");

    dFunc()->init();

    dFunc()->mEventDispatcher->startUp();
}

Application::Application(ApplicationPrivate &dd)
    : Object(dd, NULL)
{

}


Application::~Application()
{
    ThreadPool *pool = ThreadPool::GlobalInstance();
    if (pool) {
        pool->waitForDone();
    }

    dFunc()->mThreadData->mEventDispatcher = NULL;
    if (ApplicationPrivate::mEventDispatcher) {
        ApplicationPrivate::mEventDispatcher->closeDown();
    }
    ApplicationPrivate::mEventDispatcher = NULL;

}

StringList Application::arguments()
{
    if (!Application::Instance()) {
        return StringList();
    }

    return Application::Instance()->dFunc()->arguments();
}

std::string Application::appName()
{
    if (!Application::Instance()) {
        return "";
    }

    return Application::Instance()->dFunc()->appName();
}

std::string Application::appDirPath()
{
    if (!Application::Instance()) {
        return "";
    }

    return Application::Instance()->dFunc()->appDirPath();
}

std::string Application::appFilePath()
{
    if (!Application::Instance()) {
        return "";
    }

    return Application::Instance()->dFunc()->appFilePath();
}

int Application::appPid()
{
    if (!Application::Instance()) {
        return -1;
    }

    return Application::Instance()->dFunc()->appPid();
}

int Application::exec()
{
    if (!ApplicationPrivate::CheckInstance("Application::processEvents")) {
        return -1;
    }

    ApplicationPrivate::CheckMainThread();

    if (!Application::Instance()->dFunc()->mThreadData->mEventLoops.empty()) {
        mlogw("MU", "Application::exec the event loop is running!");
        return -1;
    }

    EventLoop loop;
    return loop.exec();
}

void Application::processEvents(EventLoop::EventLoopFlag flag, int maxTime)
{
    if (!ApplicationPrivate::CheckInstance("Application::processEvents")) {
        return;
    }

    ThreadData *data = ThreadData::Current();
    if (data->mEventDispatcher) {
        data->mEventDispatcher->processEvents(flag, maxTime);
    }
    else {
        mlogw("MU", "Application::processEvents failed with NULL event dispatcher!");
    }
}

void Application::exit(int returnCode)
{
    if (!ApplicationPrivate::CheckInstance("Application::exit")) {
        return;
    }
    ThreadData *data = Application::Instance()->dFunc()->mThreadData;
    for (int i =0; i < data->mEventLoops.size(); ++i) {
        EventLoop *loop = data->mEventLoops.at(i);
        MU_ASSERT2(loop);
        loop->exit(returnCode);
    }
}

bool Application::sendEvent(Object *receiver, Event *event)
{
    return Application::Instance()->notify(receiver, event);
}

void Application::postEvent(Object *receiver, Event *event, int priority)
{
    if (!receiver) {
        delete event;
        return;
    }

    ThreadData *data = Application::Instance()->dFunc()->mThreadData;
    AutoMutex _mutex(data->mPostEventList.mMutex);
    PostEvent pe;
    pe.mReceiver = receiver;
    pe.mEvent = event;
    pe.mPriority = priority;
    data->mPostEventList.push_back(pe);
}

void Application::sendPostedEvents(Object *receiver, int type)
{
    ThreadData *data = receiver ? receiver->dFunc()->mThreadData : ThreadData::Current();
    AutoMutex _mutex(data->mPostEventList.mMutex);

    if (!receiver) {
        return;
    }

    int n = data->mPostEventList.size();

    for (int i = 0; i < n; ++i) {
        PostEvent pe = data->mPostEventList.at(i);
        --pe.mReceiver->dFunc()->mPostedEvents;
        pe.mEvent->mPosted = false;
        sendEvent(receiver, pe.mEvent);
        delete pe.mEvent;
        pe.mEvent = 0;
    }
    data->mPostEventList.clear();
}

void Application::removePostedEvents(Object *receiver, int type)
{
    ThreadData *data = receiver ? receiver->dFunc()->mThreadData : ThreadData::Current();
    AutoMutex _mutex(data->mPostEventList.mMutex);

    if (receiver && !receiver->dFunc()->mPostedEvents) {
        return;
    }

    std::vector<Event*> events;
    int n = data->mPostEventList.size();

    for (int i = 0; i < n; ++i) {
        PostEvent pe = data->mPostEventList.at(i);

        if ((!receiver || pe.mReceiver == receiver)
                && (pe.mEvent && (type == 0 || pe.mEvent->type() == type))) {
            --pe.mReceiver->dFunc()->mPostedEvents;
            pe.mEvent->mPosted = false;
            events.push_back(pe.mEvent);
            pe.mEvent = 0;
        }
    }

    for (int i = 0; i < events.size(); ++i) {
        delete events[i];
    }
}

AbstractEventDispatcher* Application::eventDispatcher()
{
    return Application::Instance()->dFunc()->mEventDispatcher;
}

void Application::setEventDispatcher(AbstractEventDispatcher *eventDispatcher)
{
    Application::Instance()->dFunc()->mEventDispatcher = eventDispatcher;
}

bool Application::event(Event *event)
{

}

bool Application::notify(Object *receiver, Event *event)
{
    return ApplicationPrivate::notify(receiver, event);
}

//==================================================
//==================================================

AbstractEventDispatcher* ApplicationPrivate::mEventDispatcher = NULL;
bool ApplicationPrivate::mLocaleInitialized = false;
PlatformIntegration* ApplicationPrivate::mPlatformIntegration = NULL;

bool ApplicationPrivate::CheckInstance(const char *func)
{
    bool ret = !!Application::Instance();
    if (!ret) {
        mlogw("MU", "ApplicationPrivate::CheckInstance please instantiate the Application object firstly before %s!\n", func);
    }
    return ret;
}

void ApplicationPrivate::CheckReceiverThread(Object *receiver)
{
    Thread *object = receiver->thread();
    Thread *current = Thread::CurrentThread();
    MU_ASSERT(object == current, "ApplicationPrivate::CheckReceiverThread cannot send event to object owned by a different thread!");
}

void ApplicationPrivate::CheckMainThread()
{
    Thread *current = Thread::CurrentThread();
    Thread *main = Application::Instance()->thread();
    MU_ASSERT(current == main, "ApplicationPrivate::CheckMainThread cannot send event outside the main thread!");
}

void ApplicationPrivate::InitLocale()
{
    if (mLocaleInitialized) {
        return;
    }
    Locale::Initialize();
    mLocaleInitialized = true;
}

bool ApplicationPrivate::notify(Object *receiver, Event *event)
{
    if (!CheckInstance("ApplicationPrivate::notify")) {
        return true;
    }

    if (!receiver) {
        mlogw("MU", "ApplicationPrivate::notify unexpected NULL receiver!");
        return true;
    }

    if (!event) {
        mlogw("MU", "ApplicationPrivate::notify unexpected NULL event!");
        return true;
    }

    CheckReceiverThread(receiver);
    CheckMainThread();

    return receiver->event(event);
}

void ApplicationPrivate::removePostedEvent(Event *event)
{
    if (!event || !event->mPosted) {
        return;
    }

    ThreadData *data = ThreadData::Current();
    AutoMutex _mutex(data->mPostEventList.mMutex);

    for (int i = 0; i < data->mPostEventList.size(); ++i) {
        PostEvent  pe = data->mPostEventList.at(i);
        if (pe.mEvent == event) {
            --pe.mReceiver->dFunc()->mPostedEvents;
            pe.mEvent->mPosted = false;
            delete pe.mEvent;
            pe.mEvent = 0;
            return;
        }
    }
}

ApplicationPrivate::ApplicationPrivate(int &argc, char **argv)
    : mArgc(argc)
    , mArgv(argv)
    , mThreadDataClean(false)
{
    if (Process::CurrentPid() != Thread::CurrentTid()) {
        mlogw("MU", "ApplicationPrivate::ApplicationPrivate application not created in the main thread!");
    }
}

ApplicationPrivate::~ApplicationPrivate()
{
    cleanupThreadData();

    mArgc = 0;
    mArgv = NULL;
}

void ApplicationPrivate::processCommandLineArguments()
{

}

StringList ApplicationPrivate::arguments() const
{
    StringList list;
    for (int i = 1; i < mArgc; ++i) {
        list.push_back(mArgv[i]);
    }
    return list;
}

std::string ApplicationPrivate::appName() const
{
    return FileInfo(mArgv[0]).baseName();
}

std::string ApplicationPrivate::appDirPath() const
{
    return FileInfo(mArgv[0]).dirName();
}

std::string ApplicationPrivate::appFilePath() const
{
    return FileInfo(mArgv[0]).filePath();
}

int ApplicationPrivate::appPid() const
{
    return Process::CurrentPid();
}

void ApplicationPrivate::createEventDispatcher()
{
    if (!mEventDispatcher) {
        mEventDispatcher = new EventDispatcherLinux(mPtr);
    }
}

void ApplicationPrivate::createPlatformIntegration()
{

}

void ApplicationPrivate::init()
{
    InitLocale();

//    if (!mEventDispatcher) {
//        mEventDispatcher = mThreadData->mEventDispatcher;
//    }
    if (!mEventDispatcher) {
        createEventDispatcher();
    }
    MU_ASSERT2(mEventDispatcher);

    processCommandLineArguments();

    createPlatformIntegration();

    Palette::Initialize();
    Font::Initialize();
    Cursor::Initialize();
}

void ApplicationPrivate::cleanupThreadData()
{
    if (mThreadData && !mThreadDataClean) {
        AutoMutex _mutex(mThreadData->mPostEventList.mMutex);
        int peSize = mThreadData->mPostEventList.size();
        for (int i = 0; i < peSize; ++i) {
            PostEvent pe = mThreadData->mPostEventList.at(i);
            MU_ASSERT2(pe.mReceiver);
            MU_ASSERT2(pe.mEvent);
            --pe.mReceiver->dFunc()->mPostedEvents;
            pe.mEvent->mPosted = false;
            delete pe.mEvent;
        }
        mThreadData->mPostEventList.clear();
        mThreadDataClean = true;
    }
}

MU_END_NAMESPACE
