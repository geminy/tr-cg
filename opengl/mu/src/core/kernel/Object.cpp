#include "Object.h"
#include "ObjectPrivate.h"
#include "Application.h"
#include "CoreEvent.h"
#include "../io/Log.h"
#include "../thread/Thread.h"
#include "../thread/ThreadPrivate.h"

MU_BEGIN_NAMESPACE

Object::Object(Object *parent)
    : dPtr(new ObjectPrivate)
{
    dPtr->mPtr = this;
    dFunc()->mThreadData = ThreadData::Current();
    setParent(parent);
}

Object::Object(ObjectPrivate &dd, Object *parent)
    : dPtr(&dd)
{
    dPtr->mPtr = this;
    dFunc()->mThreadData = ThreadData::Current();
    setParent(parent);
}

Object::~Object()
{
    setParent(NULL);
    dFunc()->deleteChildren();

    MU_ASSERT2(dPtr);
    delete dPtr;
    dPtr = NULL;
}

Object* Object::parent() const
{
    return dPtr->mParent;
}

void Object::setParent(Object *parent)
{  
    dFunc()->setParent(parent);
}

const ObjectList& Object::children() const
{
    return dPtr->mChildren;
}

Object* Object::findChild(const std::string &name, bool recur) const
{
    return dFunc()->findChild(name, recur);
}

ObjectList Object::findChildren(const std::string &name, bool recur) const
{
    ObjectList list;
    dFunc()->findChildren(name, &list, recur);
    return list;
}

std::string Object::objectName() const
{
    return dFunc()->mObjectName;
}

void Object::setObjectName(const std::string &name)
{
    if (dFunc()->mObjectName != name) {
        dFunc()->mObjectName = name;
    }
}

void Object::dumpObject() const
{
    mlogd("MU", "** Object this=%p objectName=%s parent=%p children=%d\n",
         this, objectName().c_str(), parent(), children().size());
}

void Object::dumpObjectTree() const
{
    dumpObject();
    dFunc()->dumpRecursively(0);
}

Thread* Object::thread() const
{
    return dFunc()->mThreadData->mThread;
}

void Object::moveToThread(Thread *thread)
{
    if (dFunc()->mParent) {
        mlogw("MU", "Object::moveToThread failed with a parent!");
        return;
    }

    if (dFunc()->mThreadData != ThreadData::Current()) {
        mlogw("MU", "Object::moveToThread failed outside the object's thread!");
        return;
    }

    if (dFunc()->mThreadData->mThread != thread) {
        dFunc()->mThreadData->mThread = thread;
        dFunc()->moveToThread(thread);
    }
}

bool Object::event(Event */*event*/)
{
    return true;
}

//==================================================
//==================================================

ObjectData::~ObjectData()
{

}

//==================================================
//==================================================

ObjectPrivate::ObjectPrivate()
    : mObjectName("")
    , mThreadData(NULL)
    , mPostedEvents(0)
{
    mPtr = NULL;
    mParent = NULL;
}

ObjectPrivate::~ObjectPrivate()
{
    if (mPostedEvents) {
        Application::removePostedEvents(mPtr);
        mPostedEvents = 0;
    }

    mThreadData = NULL;
    mPtr = NULL;
    mParent = NULL;
    mChildren.clear();
}

bool ObjectPrivate::checkParentThread(Object *parent) const
{
    if (parent && parent->dFunc()->mThreadData != mThreadData) {
        mlogw("MU", "ObjectPrivate::checkParentThread parent(%s %p) and child(%s %p) not in the same thread!",
              parent->objectName().c_str(), parent, mObjectName.c_str(), mPtr);
        return false;
    }
    return true;
}

void ObjectPrivate::setParent(Object *parent)
{
    if (!checkParentThread(parent)) {
        return;
    }

    if (mParent != parent) {
        if (mParent) {
            ObjectList::iterator it = mParent->dPtr->mChildren.begin();
            for (; it != mParent->dPtr->mChildren.end(); ++it) {
                if (*it == mPtr) {
                    mParent->dPtr->mChildren.erase(it);
                    break;
                }
            }
            MU_ASSERT2(it != mParent->dPtr->mChildren.end());
        }
        mParent = parent;
        if (parent) {
            parent->dPtr->mChildren.push_back(mPtr);
        }
    }
}

Object* ObjectPrivate::findChild(const std::string &name, bool recur) const
{
    if (mChildren.size() < 1) {
        return NULL;
    }

    ObjectList::const_iterator it = mChildren.begin();
    for (; it != mChildren.end(); ++it) {
        Object *obj = *it;
        MU_ASSERT2(obj);
        if (obj->objectName() == name) {
            return obj;
        }
    }

    if (recur) {
        for (it = mChildren.begin(); it != mChildren.end(); ++it) {
            MU_ASSERT2(*it);
            Object *obj = (*it)->findChild(name, recur);
            if (obj) {
                return obj;
            }
        }
    }

    return NULL;
}

void ObjectPrivate::findChildren(const std::string &name, ObjectList *list, bool recur) const
{
    if (!list || mChildren.size() < 1) {
        return;
    }

    ObjectList::const_iterator it = mChildren.begin();
    for (; it != mChildren.end(); ++it) {
        Object *obj = *it;
        MU_ASSERT2(obj);
        if (obj->objectName() == name) {
            list->push_back(obj);
        }
        if (recur) {
            obj->dFunc()->findChildren(name, list, recur);
        }
    }
}

void ObjectPrivate::deleteChildren()
{
    if (mChildren.size() < 1) {
        return;
    }

    ObjectList::iterator it = mChildren.begin();
    for (; it != mChildren.end(); ++it) {
        Object *obj = *it;
        MU_ASSERT2(obj);
        obj->dPtr->mParent = NULL;
        ObjectList::iterator next = ++it;
        mChildren.erase(--it);
        it = --next;
        delete obj;
    }
    mChildren.clear();
}

void ObjectPrivate::dumpRecursively(int level) const
{
    mlogd("MU", "**** level=%d this=%p objectName=%s parent=%p children=%d\n",
         level, mPtr, mObjectName.c_str(), mParent, mChildren.size());

    if (mChildren.size() >= 1) {
        ObjectList::const_iterator it = mChildren.begin();
        for (; it != mChildren.end(); ++it) {
            MU_ASSERT2(*it);
            (*it)->dFunc()->dumpRecursively(level + 1);
        }
    }
}

void ObjectPrivate::moveToThread(Thread *thread)
{
    ObjectList::iterator it = mChildren.begin();
    for (; it != mChildren.end(); ++it) {
        MU_ASSERT2(*it);
        (*it)->moveToThread(thread);
    }
}


MU_END_NAMESPACE
