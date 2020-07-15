#include "scene.h"

namespace annileen
{

    //void Scene::setCubemap(Cubemap* cubemap)
    //{
    //    m_Cubemap = cubemap;
    //
    //    createSkybox();
    //}

    //Cubemap* Scene::getCubemap() const
    //{
    //    return m_Cubemap;
    //}
    //
    //Skybox * Scene::getSkybox() const
    //{
    //    return m_Skybox;
    //}
    //
    //void Scene::createSkybox()
    //{
    //    m_Skybox = new Skybox(m_Cubemap);
    //    m_Skybox->createMesh();
    //}

    //void Scene::addMesh(Mesh* mesh)
    //{
    //    mesh->genBuffers();
    //    m_Meshes.push_back(mesh);
    //}

    //void Scene::removeMesh(Mesh* mesh)
    //{
    //    m_Meshes.remove(mesh);
    //}

    SceneNode* Scene::getRoot()
    {
        return m_Root;
    }

    SceneNode* Scene::createNode()
    {
        SceneNode* node = new SceneNode();
        m_Nodes.push_back(node);
        node->setParent(m_Root);
        return node;
    }

    void Scene::addLight(Light* light)
    {
        m_Lights.push_back(light);
    }

    //std::list<Mesh*>& Scene::getMeshList()
    //{
    //    return m_Meshes;
    //}

    std::list<Light*>& Scene::getLightList()
    {
        return m_Lights;
    }

    Camera* Scene::getCamera()
    {
        return m_Camera;
    }

    Scene::Scene()
    {
        m_Camera = new Camera(60.0f, 0.1f, 300.0f);
        m_Root = new SceneNode();
    }

    Scene::~Scene()
    {
        /*delete m_Cubemap;
        delete m_Skybox;*/
        delete m_Camera;
        delete m_Root;
    }
}

