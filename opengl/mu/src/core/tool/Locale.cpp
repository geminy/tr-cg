#include "Locale.h"

#include <clocale>

MU_BEGIN_NAMESPACE

void Locale::Initialize()
{
    ::setlocale(LC_ALL, "");
}

MU_END_NAMESPACE
