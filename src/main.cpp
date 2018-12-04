#include <iostream>
#include "engine.h"
#include "util.h"

float vdata_cube[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

float vdata_floor[] = {
    -0.5f,  0.0f, -0.5f,  0.0f, 0.5f,
     0.5f,  0.0f, -0.5f,  0.33, 0.5f,
     0.5f,  0.0f,  0.5f,  0.33, 0.0f,
     0.5f,  0.0f,  0.5f,  0.33, 0.0f,
    -0.5f,  0.0f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.0f, -0.5f,  0.0f, 0.5f
};

void controlCamera(Input* input, Camera* camera);

int main(int argc, char* argv[])
{
    Engine* engine = Engine::getInstance();
    engine->init(800, 600);

    // Create triangle
    Mesh* cube = new Mesh();
    cube->init(vdata_cube,
        sizeof(vdata_cube) / sizeof(float),
        VERTEX_UV);
    cube->transform.translate(glm::vec3(0.0f, 0.0f, -3.0f));

    Mesh* floor = new Mesh();
    floor->init(vdata_floor,
        sizeof(vdata_floor) / sizeof(float),
        VERTEX_UV);
    floor->transform.translate(glm::vec3(0.0f, -1.0f, -3.0f));
    floor->transform.scale *= 30.0f;

    // Mesh material
    Shader* shader = new Shader();
    shader->load("../../shaders/vertex.vert", "../../shaders/fragment.frag");

    Material* material = new Material();
    material->init(shader);
    material->addTexture("mainTex", Texture("../../assets/texture.png"));
    material->loadTextures();

    cube->setMaterial(material);
    floor->setMaterial(material);

    // Create scene
    Scene* scene = new Scene();
    scene->addMesh(cube);
    scene->addMesh(floor);

    Camera* camera = scene->getCamera();

    engine->setScene(scene);

    float sensitivity = 0.25f;
    float pitch = 0.0f, yaw = 0.0f;

    while (engine->isRunning())
    {
        engine->checkInputEvents();

        cube->transform.rotate(glm::vec3(0.0f, 0.3f, 0.0f));
        //cube->transform.rotation = glm::quat(glm::radians(glm::vec3(0.0f, 90.1f, 0.0f)));
        //std::cout << camera->transform.getEuler().x << ", "
        //          << camera->transform.getEuler().y << ", "
        //          << camera->transform.getEuler().z << std::endl;

        if (engine->getInput()->getKeyDown(SDLK_s))
        {
            camera->transform.translate(-0.05f * camera->getForward());
        }
        if (engine->getInput()->getKeyDown(SDLK_w))
        {
            camera->transform.translate(0.05f * camera->getForward());
        }
        if (engine->getInput()->getKeyDown(SDLK_a)) 
        {
            camera->transform.translate(0.05f * camera->getRight());
        }
        if (engine->getInput()->getKeyDown(SDLK_d))
        {
            camera->transform.translate(-0.05f * camera->getRight());
        }

        if (engine->getInput()->getKeyDown(SDLK_SPACE))
        {
            camera->transform.lookAt(cube->transform);
        }

        if (engine->getInput()->getKeyDown(SDLK_ESCAPE))
        {
            engine->terminate();
        }

        {
            // Camera mouse control
            auto mouseDelta = engine->getInput()->getMouseDelta();

            yaw += mouseDelta.x * sensitivity;
            pitch += -mouseDelta.y * sensitivity;
            pitch = glm::clamp(pitch, -89.0f, 89.0f);
            glm::vec3 cameraForward{
                cos(glm::radians(pitch)) * cos(glm::radians(yaw)),
                sin(glm::radians(pitch)),
                cos(glm::radians(pitch)) * sin(glm::radians(yaw))
            };
            camera->setForward(glm::normalize(cameraForward));
        }


        engine->renderFrame();
    }

    return EXIT_SUCCESS;
}

void controlCamera(Input *input, Camera* camera)
{
    float sensivity = 0.05f;

}
