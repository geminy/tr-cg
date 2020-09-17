/**
  * @file ObjectPrivate.h
  */

#ifndef MU_OBJECTPRIVATE_H
#define MU_OBJECTPRIVATE_H

#include "Object.h"

MU_BEGIN_NAMESPACE

class ThreadData;

/**
 * @class ObjectPrivate
 */
class MU_EXPORT ObjectPrivate : public ObjectData
{
public:
    ObjectPrivate();
    virtual ~ObjectPrivate();

    bool checkParentThread(Object *parent) const;

    void setParent(Object *parent);

    Object* findChild(const std::string &name, bool recur) const;
    void findChildren(const std::string &name, ObjectList *list, bool recur) const;

    void deleteChildren();

    void dumpRecursively(int level) const;

    void moveToThread(Thread *thread);

public:
    std::string mObjectName;
    ThreadData *mThreadData;
    int mPostedEvents;

public:
    MU_DECLARE_PUBLIC(Object)
};

MU_END_NAMESPACE

#endif // MU_OBJECTPRIVATE_H
