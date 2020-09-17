/**
  * @file Object.h
  */

#ifndef MU_OBJECT_H
#define MU_OBJECT_H

#include "../global/Global.h"

#include <cstddef>
#include <list>
#include <string>

MU_BEGIN_NAMESPACE

class Object;
class ObjectData;
class ObjectPrivate;
class Thread;
class Event;

typedef std::list<Object*> ObjectList;

/**
 * @class Object
 * @todo Thread Safety
 */
class MU_EXPORT Object
{
public:
    explicit Object(Object *parent = NULL);
    virtual ~Object();

    Object* parent() const;
    void setParent(Object *parent);

    const ObjectList& children() const;
    Object* findChild(const std::string &name = "", bool recur = true) const;
    ObjectList findChildren(const std::string &name = "", bool recur = true) const;

    std::string objectName() const;
    void setObjectName(const std::string &name);

    void dumpObject() const;
    void dumpObjectTree() const;

    Thread* thread() const;
    void moveToThread(Thread *thread);

    virtual bool event(Event *event);

    MU_DECLARE_PRIVATE(Object)

protected:
    Object(ObjectPrivate &dd, Object *parent = NULL);

protected:
    ObjectData *dPtr;

private:
    MU_DISABLE_COPY(Object)
};

/**
 * @class ObjectData
 */
class MU_EXPORT ObjectData
{
public:
    virtual ~ObjectData() = 0;

public:
    Object *mPtr;
    Object *mParent;
    ObjectList mChildren;
};

MU_END_NAMESPACE

#endif // MU_OBJECT_H
