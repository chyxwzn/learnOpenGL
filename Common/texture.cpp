// #ifndef STB_IMAGE_IMPLEMENTATION
// #define STB_IMAGE_IMPLEMENTATION
// #endif

#include "texture.hpp"
#include <stb_image.h>
#include <iostream>

namespace Common
{

    GLuint Texture::mUnitIdx = 0;

    Texture::Texture(std::string const& path, std::string const& filename)
    {
        int width, height, channels;
        auto index = path.rfind("Source");
        std::string imgPath = path.substr(0, index) + "Resource/" + filename;
        unsigned char * image = stbi_load(imgPath.c_str(), & width, & height, & channels, 0);
        if (!image)
            fprintf(stderr, "%s %s\n", "Failed to Load Texture", imgPath.c_str());

        // Set the Correct Channel Format
        GLenum format;
        switch (channels)
        {
            case 1 : format = GL_ALPHA;
                     break;
            case 3 : format = GL_RGB;
                     break;
            case 4 : format = GL_RGBA;
                     break;
        }

        // Bind Texture and Set Filtering Levels
        glGenTextures(1, &mTexture);
        glBindTexture(GL_TEXTURE_2D, mTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, format,
                width, height, 0, format, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Release Image Pointer and Store the Texture
        stbi_image_free(image);
        glBindTexture(GL_TEXTURE_2D, 0);
        mUnit = mUnitIdx++;
        // std::cout << "mTexture:" << mTexture << ", mUnit:" << mUnit << ", mUnitIdx:" << mUnitIdx << std::endl;
        // std::cout << "texture" + std::to_string(mUnit) << ", max:" << GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS <<  std::endl;
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &mTexture);
    }

    void Texture::activate(GLuint shader)
    {
        std::string uniform = "tex" + std::to_string(mUnit+1);
        glUniform1i(glGetUniformLocation(shader, uniform.c_str()), mUnit);
        glActiveTexture(GL_TEXTURE0 + mUnit);
        glBindTexture(GL_TEXTURE_2D, mTexture);
    }

};
