//==============================================================================
/*
    Software License Agreement (BSD License)
    Copyright (c) 2019-2021, AMBF
    (https://github.com/WPI-AIM/ambf)

    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above
    copyright notice, this list of conditions and the following
    disclaimer in the documentation and/or other materials provided
    with the distribution.

    * Neither the name of authors nor the names of its contributors may
    be used to endorse or promote products derived from this software
    without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
    ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.

    \author    <amunawar@wpi.edu>
    \author    Adnan Munawar
*/
//==============================================================================

#include "pbr_plugin.h"
#include <boost/program_options.hpp>
#include <mutex>
#include <GLFW/glfw3.h>

double g_metallic = 0.05;
double g_roughness = 0.05;
double g_ao = 0.5;

cMultiMesh* g_sphereMesh;

using namespace std;

int afPBRPlugin::init(int argc, char **argv, const afWorldPtr a_afWorld)
{
    m_worldPtr = a_afWorld;

    afRigidBodyPtr sphereRB = m_worldPtr->getRigidBody("Sphere");

    if (sphereRB){
        g_sphereMesh = sphereRB->getVisualObject();
        cMesh* sphereMesh0 = g_sphereMesh->getMesh(0);

        string basePath = "/home/adnan/ambf_plugins/pbr_plugin/ADF/high_res";

        cTexture2dPtr tex1 = cTexture2d::create();
        cerr << __LINE__ << " " << tex1->loadFromFile(basePath + "/albedo.png") << endl;
        tex1->setTextureUnit(GL_TEXTURE0);

        cNormalMapPtr tex2 = cNormalMap::create();
        cerr << __LINE__ << " " << tex2->loadFromFile(basePath + "/normal.png") << endl;
        tex2->setTextureUnit(GL_TEXTURE2);

        cTexture2dPtr tex3 = cTexture2d::create();
        cerr << __LINE__ << " " << tex3->loadFromFile(basePath + "/metallic.png") << endl;
        tex3->setTextureUnit(GL_TEXTURE3);

        cTexture2dPtr tex4 = cTexture2d::create();
        cerr << __LINE__ << " " << tex4->loadFromFile(basePath + "/roughness.png") << endl;
        tex4->setTextureUnit(GL_TEXTURE4);

        cTexture2dPtr tex5 = cTexture2d::create();
        cerr << __LINE__ << " " << tex5->loadFromFile(basePath + "/ao.png") << endl;
        tex5->setTextureUnit(GL_TEXTURE5);

        g_sphereMesh->setTexture(tex1);
        sphereMesh0->m_normalMap = tex2;
        g_sphereMesh->setMetallicTexture(tex3);
        g_sphereMesh->setRoughnessTexture(tex4);
        g_sphereMesh->setAmbientOcculusionTexture(tex5);

        g_sphereMesh->setUseTexture(true);

        m_worldPtr->m_shaderProgram->setUniformi("albedoMap", C_TU_TEXTURE);
        m_worldPtr->m_shaderProgram->setUniformi("normalMap", C_TU_NORMALMAP);
        m_worldPtr->m_shaderProgram->setUniformi("metallicMap", C_TU_METALLIC);
        m_worldPtr->m_shaderProgram->setUniformi("roughnessMap", C_TU_ROUGHNESS);
        m_worldPtr->m_shaderProgram->setUniformi("aoMap", C_TU_AO);
    }
    return 1;

}

void afPBRPlugin::keyboardUpdate(GLFWwindow *a_window, int a_key, int a_scancode, int a_action, int a_mods)
{
    if (a_mods == GLFW_MOD_CONTROL){
        if (a_key == GLFW_KEY_UP){
            g_metallic = cClamp(g_metallic + 0.01, 0.0, 1.0);
        }
        else if (a_key == GLFW_KEY_DOWN){
            g_metallic = cClamp(g_metallic - 0.01, 0.0, 1.0);
        }
        else if (a_key == GLFW_KEY_RIGHT){
            g_roughness = cClamp(g_roughness + 0.01, 0.0, 1.0);
        }
        else if (a_key == GLFW_KEY_LEFT){
            g_roughness = cClamp(g_roughness - 0.01, 0.0, 1.0);
        }
        else if (a_key == GLFW_KEY_O){
            g_ao = cClamp(g_ao + 0.01, 0.0, 1.0);
        }
        else if (a_key == GLFW_KEY_L){
            g_ao = cClamp(g_ao - 0.01, 0.0, 1.0);
        }

        cerr << "METALLIC: " << g_metallic << " | ROUGHNESS: " << g_roughness << " | AO: " << g_ao << endl;
    }
}

void afPBRPlugin::mouseBtnsUpdate(GLFWwindow *a_window, int a_button, int a_action, int a_modes)
{

}

void afPBRPlugin::mousePosUpdate(GLFWwindow *a_window, double x_pos, double y_pos) {}

void afPBRPlugin::mouseScrollUpdate(GLFWwindow *a_window, double x_pos, double y_pos)
{

}

void afPBRPlugin::graphicsUpdate()
{
    m_worldPtr->m_shaderProgram->setUniformi("uNumLights", m_worldPtr->getLights().size());
    m_worldPtr->m_shaderProgram->setUniformf("metallic", g_metallic);
    m_worldPtr->m_shaderProgram->setUniformf("roughness", g_roughness);
    m_worldPtr->m_shaderProgram->setUniformf("ao", g_ao);
}

void afPBRPlugin::physicsUpdate(double dt)
{

}

void afPBRPlugin::reset()
{

}

bool afPBRPlugin::close()
{
    return 1;
}
