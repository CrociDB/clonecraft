#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "engine.h"
#include "skybox.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Renderer
{
private:
    Engine* m_Engine;
    SDL_GLContext m_Context;

public:
    void init(Engine* engine);

    void clear(const glm::vec3& color) const;
    void initFrame(Scene* scene);
    void swapBuffer();
    void renderSkybox(Camera* camera, Skybox* skybox);
    void renderMesh(Camera* camera, Mesh* mesh);

    Renderer();
    ~Renderer();
};

#endif
