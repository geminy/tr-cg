#include "Texture.h"

#include <glad/glad.h>
#include <stb_image.h>

#include <cstdio>

Texture::Texture(const std::string& path, const std::string& type, const std::string& name)
    : mType(type), mName(name)
{
    glGenTextures(1, &mId);

    int width, height, components;

    unsigned char* data = stbi_load(path.c_str(), &width, &height, &components, 0);
    if (data) {
        GLenum format = 0;
        if (components == 1) {
            format = GL_RED;
        } else if (components == 3) {
            format = GL_RGB;
        }
        else if (components == 4) {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, mId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // 指定纹理参数
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        printf("Texture LoadTextureFromFile failed:%s\n", path.c_str());
    }
}

// TODO
Texture::~Texture() {
    //glDeleteTextures(1, &id);
}

unsigned int Texture::getId() const {
    return mId;
}

std::string Texture::getType() const {
    return mType;
}

std::string Texture::getName() const {
    return mName;
}
