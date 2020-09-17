#include "Global"

MU_USE_NAMESPACE

void TestLog()
{
    mtemplog("TestLog");

    mlog("__STDC__=%d\n", __STDC__);
    mlog("__FILE__=%s __LINE__=%d\n", __FILE__, __LINE__);
    mlog("__DATE__=%s __TIME__=%s\n", __DATE__, __TIME__);

    mlogd("test", "__func__=%s\n", __func__);
    mlogw("test", "__FUNCTION__=%s\n", __FUNCTION__);
    mloge("test", "__PRETTY_FUNCTION__=%s\n", __PRETTY_FUNCTION__);
}

void TestObject()
{
    mtemplog("TestObject");

    Object *root = new Object;
    root->setObjectName("root");
    Object *o1 = new Object(root);
    o1->setObjectName("o1");
    Object *o2 = new Object;
    o2->setParent(o1);
    o2->setObjectName("o2");

    mlog("object tree: root-o1-o2\n");

    class TempObject : public Object
    {
    public:
        explicit TempObject(Object *parent = NULL)
            : Object(parent)
        {
            setObjectName("temp");
        }
    };

    Object *temp = new TempObject;

    mlog("---------- temp no parent\n");

    root->dumpObjectTree();
    o1->dumpObjectTree();
    o2->dumpObjectTree();
    temp->dumpObjectTree();

    mlog("---------- temp parent is root\n");

    temp->setParent(root);
    root->dumpObjectTree();
    o1->dumpObjectTree();
    o2->dumpObjectTree();
    temp->dumpObjectTree();

    mlog("---------- temp parent is o1\n");

    temp->setParent(o1);
    root->dumpObjectTree();
    o1->dumpObjectTree();
    o2->dumpObjectTree();
    temp->dumpObjectTree();

    mlog("---------- temp parent is o2\n");

    temp->setParent(o2);
    root->dumpObjectTree();
    o1->dumpObjectTree();
    o2->dumpObjectTree();
    temp->dumpObjectTree();

    mlog("---------- temp parent is NULL\n");

    temp->setParent(NULL);
    root->dumpObjectTree();
    o1->dumpObjectTree();
    o2->dumpObjectTree();
    temp->dumpObjectTree();

    delete temp;

    Object *obj01 = new Object(root);
    Object *obj02 = new Object(root);
    Object *obj11 = new Object(o1);
    Object *obj12 = new Object(o1);
    Object *obj21 = new Object(o2);
    Object *obj22 = new Object(o2);

    mlog("---------- root object tree for children\n");

    root->dumpObjectTree();

    mlog("---------- root findChild recur\n");

    Object *child = root->findChild("Object");
    if (child) {
        child->dumpObject();
    }
    else {
        mlog("root no child Object recur\n");
    }
    child = root->findChild("o2");
    if (child) {
        child->dumpObject();
    }
    else {
        mlog("root no child o2 recur\n");
    }
    child = root->findChild("temp");
    if (child) {
        child->dumpObject();
    }
    else {
        mlog("root no child temp recur\n");
    }

    mlog("---------- root findChild direct\n");

    child = root->findChild("Object", false);
    if (child) {
        child->dumpObject();
    }
    else {
        mlog("root no child Object direct\n");
    }
    child = root->findChild("o2", false);
    if (child) {
        child->dumpObject();
    }
    else {
        mlog("root no child o2 direct\n");
    }
    child = root->findChild("temp", false);
    if (child) {
        child->dumpObject();
    }
    else {
        mlog("root no child temp direct\n");
    }

    mlog("---------- root findChildren recur\n");

    ObjectList children = root->findChildren("Object");
    ObjectList::iterator it = children.begin();
    for (; it != children.end(); ++it) {
        (*it)->dumpObject();
    }

    mlog("---------- root findChildren direct\n");

    children = root->findChildren("Object", false);
    it = children.begin();
    for (; it != children.end(); ++it) {
        (*it)->dumpObject();
    }

    delete obj01;
    delete obj02;
    delete obj11;
    delete obj12;
    delete obj21;
    delete obj22;

    mlog("---------- root object tree before delete\n");

    root->dumpObjectTree();
    delete o1;

    mlog("---------- root object tree after delete\n");

    root->dumpObjectTree();
    delete root;
}

int main(int /*argc*/, char */*argv*/[])
{
    TestLog();

    TestObject();

    return 0;
}
