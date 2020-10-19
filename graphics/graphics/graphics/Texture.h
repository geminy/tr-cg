#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <glad/glad.h>

class Texture
{
public:
    static unsigned int LoadTextureFromFile(const char* path);
};

#endif // !__TEXTURE_H__
