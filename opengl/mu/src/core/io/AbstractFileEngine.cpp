#include "AbstractFileEngine.h"
#include "FSFileEngine.h"

MU_BEGIN_NAMESPACE

AbstractFileEngine::AbstractFileEngine()
{

}

AbstractFileEngine::~AbstractFileEngine()
{

}

// TODO File System(Engine/Entry/Iterator/MetaData/Watcher) with name
AbstractFileEngine* AbstractFileEngine::Create(const std::string &name)
{
    return new FSFileEngine(name);
}

MU_END_NAMESPACE
