#pragma once

// System Headers
#include <assimp/importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

// Standard Headers
#include <map>
#include <memory>
#include <vector>

// Define Namespace
namespace Common
{
    // Vertex Format
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    class Model
    {
    public:

        // Implement Default Constructor and Destructor
         Model() { glGenVertexArrays(1, & mVertexArray); }
        ~Model() { glDeleteVertexArrays(1, & mVertexArray); }

        // Implement Custom Constructors
        Model(std::string const & path, std::string const & filename);
        Model(std::vector<Vertex> const & vertices,
             std::vector<GLuint> const & indices,
             std::map<GLuint, std::string> const & textures);

        // Public Member Functions
        void draw(GLuint shader);

    private:

        // Disable Copying and Assignment
        Model(Model const &) = delete;
        Model & operator=(Model const &) = delete;

        // Private Member Functions
        void parse(std::string const & path, aiNode const * node, aiScene const * scene);
        void parse(std::string const & path, aiMesh const * mesh, aiScene const * scene);
        std::map<GLuint, std::string> process(std::string const & path,
                                              aiMaterial * material,
                                              aiTextureType type);

        // Private Member Containers
        std::vector<std::unique_ptr<Model>> mSubMeshes;
        std::vector<GLuint> mIndices;
        std::vector<Vertex> mVertices;
        std::map<GLuint, std::string> mTextures;

        // Private Member Variables
        GLuint mVertexArray;
        GLuint mVertexBuffer;
        GLuint mElementBuffer;

    };
};
