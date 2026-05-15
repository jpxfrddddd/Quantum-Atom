#include <glad/glad.h>
#include <GL/gl.h>
#include <cmath>
#include <cstdlib>
#include <iostream>

#include "renderer.h"
#include "../physics/orbital.h"
#include "../physics/atom.h"
#include "../camera/camera.h"
#include "../physics/orb.h"

/*float Renderer::getMaxDensity(const Atom& atom, float box_size){
    float max_d = 0.0001f;
    for(int i = 0; i < 2000000; i ++){
        float x = (float)rand() / RAND_MAX * 2.0f * box_size - box_size;
        float y = (float)rand() / RAND_MAX * 2.0f * box_size - box_size;
        float z = (float)rand() / RAND_MAX * 2.0f * box_size - box_size;

        float current_d = 0.0f;
        for(const auto& orb : atom.orbitals){
            float Zeff = atom.getZeff(orb.n, orb.l);
            float p = orb.psi(x, y, z, Zeff);

            current_d += orb.electrons * p*p;
        }
        if(current_d > max_d){
            max_d = current_d;
        }
    }
    return max_d;
}*/



void Renderer::initOpenGL(){
    glEnable(GL_PROGRAM_POINT_SIZE);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OrbitalPoint), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(OrbitalPoint), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

float Renderer::getMaxDensity(const Atom& atom, float box_size){
    float max_d = 35.0f;

    if(atom.Z > 0 && atom.Z < 3){
        max_d = 0.1f;
    }
    if(atom.Z > 2 && atom.Z < 5){
        max_d = 2.0f;
    }
    if(atom.Z > 4 && atom.Z < 8){
        max_d = 10.0f;
    }
    if(atom.Z > 7 && atom.Z < 10){
        max_d = 20.0f;
    }
    if(atom.Z >= 10 && atom.Z <= 15){
        max_d = 30.0f;
    }
    if(atom.Z > 15 && atom.Z <= 20){
        max_d = 32.0f;
    }
    return max_d;
}

float Renderer::getOrbitalDensity(const orbitalGroup& orbital, float box_size){
    float max_d = 1.0f;

    if(orbital.name == "orbital_1s"){
        max_d = 0.001f;
    }

    else if(orbital.name == "orbital_2s"){
        max_d = 0.1f;
    }

    else if(orbital.name == "orbital_2p_x"){
        max_d = 15.0f;
    }

    else if(orbital.name == "orbital_2p_z"){
        max_d = 0.01f;
    }

    else if(orbital.name == "orbital_2p_y"){
        max_d = 15.0f;
    }

    else if(orbital.name == "orbital_3s"
    || orbital.name == "orbital_3p"){
        max_d = 1e6f;
    }

    else if(orbital.name == "orbital_3p_x"
    || orbital.name == "orbital_3p_z"){
        max_d = 1e5f;
    }

    else if(orbital.name == "orbital_3p_y"){
        max_d = 1e4f;
    }

    else if(orbital.name == "orbital_3d"){
        max_d = 0.1f;
    }

    return max_d;
}

float getOrbitalBoxSize(const orbitalGroup& orbital){
    float box_size;

    if(orbital.name == "orbital_1s"
    || orbital.name == "orbital_2p_z"){
        box_size = 8.0f;
    }

    if(orbital.name == "orbital_2p_x"){
        box_size = 12.0f;
    }

    if(orbital.name == "orbital_2s"
    || orbital.name == "orbital_2p"
    || orbital.name == "orbital_3p_z"
    || orbital.name == "orbital_3p_y"){
        box_size = 12.0f;
    }

    if(orbital.name == "orbital_2p_y"){
        box_size = 15.0f;
    }

    if(orbital.name == "orbital_3s"
    || orbital.name == "orbital_3p"
    || orbital.name == "orbital_3p_x"
    || orbital.name == "orbital_3p_z"
    || orbital.name == "orbital_3p_y"){
        box_size = 13.5f;
    }

    return box_size;
}

float Renderer::getBoxSize(const Atom& atom){
    float box_size = 8.0f;

    if(atom.Z > 0 && atom.Z < 3){
        box_size = 8.0f;
    }
    if(atom.Z > 2 && atom.Z < 8){
        box_size = 14.0f;
    }
    if(atom.Z > 7 && atom.Z < 10){
        box_size = 16.0f;
    }
    if(atom.Z >= 10 && atom.Z <= 15){
        box_size = 50.0f;
    }
    if(atom.Z > 15 && atom.Z <= 20){
        box_size = 55.0f;
    }


    

    return box_size;
}

void Renderer::generateCloud(const Atom& atom){
    pointCloud.clear();

    float box_size = getBoxSize(atom);
    float auto_max_density = getMaxDensity(atom, box_size);

    for(int i = 0; i < 2000000; i++){
        float x = (float)rand() / RAND_MAX * 2.0f * box_size - box_size;
        float y = (float)rand() / RAND_MAX * 2.0f * box_size - box_size;
        float z = (float)rand() / RAND_MAX * 2.0f * box_size - box_size;

        float density = 0.0f;

        for(const auto& orb : atom.orbitals){
            float Zeff = atom.getZeff(orb.n, orb.l);
            float psi = orb.psi(x, y, z, Zeff);
            density += orb.electrons * psi*psi; // psi^2: probabilty of presence
        }
  

        if(std::isnan(density) || std::isinf(density)){
            continue; //skip NaN values
        }

        float p = (float)rand() / RAND_MAX * auto_max_density;

        if(p < density){
            float r_dist = sqrtf(x*x + y*y + z*z);
            float intensity = expf(-r_dist/3.0f);
            pointCloud.push_back({x, y, z, intensity, 0.4f * intensity, 1.0f - intensity, 1.0f});
        }
    }
    //std::cout << "Points: " << pointCloud.size() << std::endl;
    //std::cout << auto_max_density << std::endl;
}





void Renderer::generateOrbital(const orbitalGroup& orbitalGenerated){
    orbitalCloud.clear();
    orbitalCloud.reserve(2000000 * 0.80f); //assuming 80% of points will be filled, adjust for memory -> performance

    float box_size = getOrbitalBoxSize(orbitalGenerated);
    float auto_max_density = getOrbitalDensity(orbitalGenerated, box_size);

    //printf("orbital: %s | max density: %.7f\n", orbitalGenerated.name.c_str(), auto_max_density);

    #pragma omp parallel for
    for(int i = 0; i < 2000000; i++){
        float x = (float)rand() / RAND_MAX * 2.0f * box_size - box_size;
        float y = (float)rand() / RAND_MAX * 2.0f * box_size - box_size;
        float z = (float)rand() / RAND_MAX * 2.0f * box_size - box_size;

        float density = 0.0f;
        for(const auto& orb : orbitalGenerated.orbitalVec){
            float Zeff = orbitalGenerated.Z;
            float psi = orb.psi(x, y, z, Zeff);
            density += orb.electrons * psi*psi;
            //float d = orb.electrons *psi*psi;
            //if(i % 10000 == 0) printf("Real density: %f\n", d);
        }

        if(std::isnan(density) || std::isinf(density)){
            continue; // skip NaN values
        }

        float p = (float)rand() / RAND_MAX * auto_max_density;

        if(p < density){
            float r_dist = sqrtf(x*x + y*y + z*z);
            float intensity = expf(-r_dist/3.0f);
            
            orbitalCloud.push_back({x, y, z, intensity, 0.4f * intensity, 1.0f - intensity, 1.0f});
            //orbitalCloud.push_back({x, y, z, intensity, 0.4 * intensity, 1.0 - intensity, 1.0});
        }
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, orbitalCloud.size() * sizeof(OrbitalPoint), orbitalCloud.data(), GL_DYNAMIC_DRAW);
    //std::cout << "orbital: " << orbitalGenerated.name << std::endl;
        
    //std::cout << "points: " << orbitalCloud.size() << std::endl;
}

void Renderer::render(const Atom& atom, const Camera& cam){
    glClear(GL_COLOR_BUFFER_BIT); //GL_DEPTH_BUFFER_BIT

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glUseProgram(shaderProgram);

    float fx, fy, fz;
    cam.getForward(fx, fy, fz);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(cam.yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 view = glm::lookAt(
        glm::vec3(cam.x, cam.y, cam.z),
        glm::vec3(cam.x + fx, cam.y + fy, cam.z + fz),
        glm::vec3(0.0f, 1.0f, 0.0f)
    ); //glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -40.0f));
    createViewMatrix(glm::value_ptr(view), (Camera&)cam);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view)); 
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f/1080.0f, 0.1f, 100.0f);

    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    /*glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"),      1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"),       1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, orbitalCloud.size() * sizeof(OrbitalPoint), orbitalCloud.data(), GL_DYNAMIC_DRAW);

    glDrawArrays(GL_POINTS, 0, (GLsizei)orbitalCloud.size());

    /*float proj[16];
    float fov = 45.0f;
    float aspect = 1920.0f/1080.0f;
    float zNear = 0.1f;
    float zFar = 1000.0f;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    perspective(proj, fov, aspect, zNear, zFar);
    glLoadMatrixf(proj);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-cam.pitch, 1.0f, 0.0f, 0.0f);
    glRotatef(-cam.yaw, 0.0f, 1.0f, 0.0f);
    glTranslatef(-cam.x, -cam.y, -cam.z); 

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glPointSize(1.2f);
    glBegin(GL_POINTS);

    for(const auto& pt : pointCloud){
        glColor4f(pt.r, pt.g, pt.b, pt.a);
        glVertex3f(pt.x, pt.y, pt.z);
    }
    
    glRotatef(time * 20.0f, 0, 1, 0);
    glEnd();
    glDisable(GL_BLEND);*/
    
} 

void Renderer::renderOrbital(const orbitalGroup& generatedOrbital, const Camera& cam){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //black
    glClear(GL_COLOR_BUFFER_BIT);

    //visual options
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    //shader activation 
    glUseProgram(shaderProgram);
    
    //the time variable can be used to animate the orbital, for example by rotating it over time
    float time = (float)glfwGetTime();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, time * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f)); 

    //move back the camera to see the orbital
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -30.0f));
    
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

    //send matrices to shader
    //glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model)); rotate the orbital over time
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, orbitalCloud.size() * sizeof(OrbitalPoint), orbitalCloud.data(), GL_DYNAMIC_DRAW);

    glDrawArrays(GL_POINTS, 0, (GLsizei)orbitalCloud.size());
} 
