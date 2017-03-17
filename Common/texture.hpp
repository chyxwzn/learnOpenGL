#pragma once

#include <glad/glad.h>
#include <string>

namespace Common
{
    class Texture
    {
    public:
        Texture(std::string const& path, std::string const& filename);
        void activate(GLuint shader);
        static GLuint mSamplerIdx;
    private:
        GLuint mTexture;
        GLuint mSampler;
    };
};
