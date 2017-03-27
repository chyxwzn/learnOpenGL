#pragma once

#include <glad/glad.h>
#include <string>

namespace Common
{
    class Texture
    {
    public:
        Texture(std::string const& path, std::string const& filename);
        ~Texture();
        void activate(GLuint shader, std::string const& sampler);
        static GLuint mUnitIdx;
    private:
        GLuint mTexture;
        GLuint mUnit;
    };
};
