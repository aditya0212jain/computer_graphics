#include "geom.h"
#include "render.h"
#include "shader.h"
// #include "stb_image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <fstream>
#include<vector>
#include <string.h>
#include <bits/stdc++.h>
#include "glm/gtx/string_cast.hpp"

using namespace std;

void SpherePoints(float radius,int num_sectors,int num_stacks,float array[],float normals[],float texel[]);

int ElementsListSphere(int num_sectors,int num_stacks,GLuint ele[]);

// int num_elements_global;

int Geom::read_dummy(const char* filename){
    float points_o[] = {
        0,0,0,0,0
    };
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(points_o), points_o, GL_STATIC_DRAW);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // vertex attrib for coordinates
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), NULL);
    // vertex attrib for textures
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    int elements[] = {
        0,0,0
    };
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elements), elements, GL_STATIC_DRAW);
    glBindVertexArray(vao);
    shader = new Shader("../data/vs.glsl", "../data/fs.glsl"); 
    pickingShader = new Shader("../data/vs.glsl","../data/fs.glsl");
    glUniform1i(glGetUniformLocation(shader->id(), "texture1"), 0);
    return vao;
}

int Geom::read(const char *filename,const char* indexfile,const char* vertex_shader, const char* fragment_shader,const char* texture,int id){
    GLuint vbo = 0;
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                  Creating the geometry for first time
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // int num_stacks = 50;
    // int num_sectors = 50;
    // float radius = 1;
    // int num_points = (num_stacks+1)*(num_sectors+1)*5;
    // float points[(num_stacks+1)*(num_sectors+1)*8];// 3 for coordinates, 3 for normals and 2 for texture coordinates
    // float normals[(num_stacks+1)*(num_sectors+1)*3];
    // float texture_coordinates[(num_stacks+1)*(num_sectors+1)*2];
    // GLuint elements[num_stacks*num_sectors*6];
    // SpherePoints(radius,num_sectors,num_stacks,points,normals,texture_coordinates);  
    // int num_elements = ElementsListSphere(num_sectors,num_stacks,elements);
    // num_elements_global = num_elements;

    /////////////////////////////////////////////////////////////////////
    // Writing the data to a file for the first time
    /////////////////////////////////////////////////////////////////////
    // ofstream sphere_file;
    // cout<<"starting writing\n";
    // sphere_file.open("sphere.txt")
    // for(int i=0;i<num_points;i++){
    //     sphere_file << points[i]<<" ";
    //     if((i+1)%5==0){
    //         sphere_file<<"\n";
    //     }
    // }
    // sphere_file.close();
    // ofstream sphere_indices;
    // sphere_indices.open("sphere_indices.txt");
    // for(int i=0;i<num_elements;i++){
    //     sphere_indices << elements[i]<<" ";
    //     if((i+1)%3==0){
    //         sphere_indices<<"\n";
    //     }
    // }
    // sphere_indices.close();
    // cout<<"writing done\n";

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                                  loading from a file
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ifstream points_file,elements_file;
    points_file.open(filename);
    std::string line;
    vector<float> OutputVertices;
    while(std::getline(points_file,line)){
        istringstream ss( line );
        copy(
            istream_iterator <float> ( ss ),
            istream_iterator <float> (),
            back_inserter( OutputVertices )
        );
    }
    float points[OutputVertices.size()];
    for(int i=0;i<OutputVertices.size();i++){
        points[i] = OutputVertices[i];
    }
    elements_file.open(indexfile);
    vector<int> outputIndices;
    while(std::getline(elements_file,line)){
        istringstream ss(line);
        copy(istream_iterator<int> (ss),
            istream_iterator<int>(),
            back_inserter(outputIndices)
            );
    }
    int elements[outputIndices.size()];
    for(int i=0;i<outputIndices.size();i++){
        elements[i] = outputIndices[i];
    }
    num_elements_global = outputIndices.size();

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                                  Handling vertex and element buffers
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /// Generate vertex data buffer
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(points), points, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // vertex attrib for coordinates
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), NULL);
    // vertex attrib for textures
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                                  Handling Textures                                                                       //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    
    GLuint texture1;
    glGenTextures(1,&texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(texture, &width, &height, &nrChannels ,0);
    if (data)
    {
        // for(int i=0;i<6;i++){
        //     glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        // }   
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Texture loading failed\n");
    }
    // glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    
    stbi_image_free(data);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                                  Binding indices and starting shader
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elements), elements, GL_STATIC_DRAW);

    glBindVertexArray(vao);
    shader = new Shader(vertex_shader, fragment_shader); 
    glUniform1i(glGetUniformLocation(shader->id(), "texture1"), 0);
    pickingShader = new Shader("../data/vs_picking.glsl","../data/fs_picking.glsl");


    int r = (vao & 0x000000FF) >>  0;
    int g = (vao & 0x0000FF00) >>  8;
    int b = (vao & 0x00FF0000) >> 16;
    glUniform4f(glGetUniformLocation(pickingShader->id(), "pickingColor"), r/255.0f, g/255.0f, b/255.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D,texture_id);
    return vao;

}

int Geom::read(const char *filename,const char* indexfile)
{
    GLuint vbo = 0;
    setObjectColor(glm::vec3(1.0,0.0,0.0));
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                  Creating the geometry for first time
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // int num_stacks = 50;
    // int num_sectors = 50;
    // float radius = 1;
    // int num_points = (num_stacks+1)*(num_sectors+1)*8;
    // float points[(num_stacks+1)*(num_sectors+1)*8];// 3 for coordinates, 3 for normals and 2 for texture coordinates
    // float normals[(num_stacks+1)*(num_sectors+1)*3];
    // float texture_coordinates[(num_stacks+1)*(num_sectors+1)*2];
    // GLuint elements[num_stacks*num_sectors*6];
    // SpherePoints(radius,num_sectors,num_stacks,points,normals,texture_coordinates);  
    // int num_elements = ElementsListSphere(num_sectors,num_stacks,elements);
    // num_elements_global = num_elements;

    /////////////////////////////////////////////////////////////////////
    // Writing the data to a file for the first time
    /////////////////////////////////////////////////////////////////////
    // ofstream sphere_file;
    // cout<<"starting writing\n";
    // sphere_file.open("sphere.txt");
    // for(int i=0;i<num_points;i++){
    //     sphere_file << points[i]<<" ";
    //     if((i+1)%8==0){
    //         sphere_file<<"\n";
    //     }
    // }
    // sphere_file.close();
    // ofstream sphere_indices;
    // sphere_indices.open("sphere_indices.txt");
    // for(int i=0;i<num_elements;i++){
    //     sphere_indices << elements[i]<<" ";
    //     if((i+1)%3==0){
    //         sphere_indices<<"\n";
    //     }
    // }
    // sphere_indices.close();
    // cout<<"writing done\n";

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                                  loading from a file
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ifstream points_file,elements_file;
    points_file.open("sphere.txt");
    std::string line;
    vector<float> OutputVertices;
    while(std::getline(points_file,line)){
        istringstream ss( line );
        copy(
            istream_iterator <float> ( ss ),
            istream_iterator <float> (),
            back_inserter( OutputVertices )
        );
    }
    float points[OutputVertices.size()];
    for(int i=0;i<OutputVertices.size();i++){
        points[i] = OutputVertices[i];
    }
    elements_file.open("sphere_indices.txt");
    vector<int> outputIndices;
    while(std::getline(elements_file,line)){
        istringstream ss(line);
        copy(istream_iterator<int> (ss),
            istream_iterator<int>(),
            back_inserter(outputIndices)
            );
    }
    int elements[outputIndices.size()];
    for(int i=0;i<outputIndices.size();i++){
        elements[i] = outputIndices[i];
    }
    num_elements_global = outputIndices.size();

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                                  Handling vertex and element buffers
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /// Generate vertex data buffer
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(points), points, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // vertex attrib for coordinates
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), NULL);
    // vertex attrib for textures
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                                  Handling Textures                                                                       //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // GLuint texture;
    // glGenTextures(1, &texture);
    // glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    // // set texture parameters
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);
    // glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // //glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    
    GLuint texture1;
    glGenTextures(1,&texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load("../data/sn.jpeg", &width, &height, &nrChannels ,0);
    if (data)
    {
        // for(int i=0;i<6;i++){
        //     glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        // }   
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Texture loading failed\n");
    }
    // glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    // glBindTexture(GL_TEXTURE_2D,0);
    stbi_image_free(data);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                                  Binding indices and starting shader
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elements), elements, GL_STATIC_DRAW);

    glBindVertexArray(vao);
    shader = new Shader("../data/vs.glsl", "../data/fs.glsl"); 
    pickingShader = new Shader("../data/vs_picking.glsl","../data/fs_picking.glsl");
    // glUniform1i(glGetUniformLocation(pickingShader->id(), "texture1"), 0);
    // cout<<"vao: "<<vao<<endl;
    return vao;
}

int Geom::render(Renderer *renderer, glm::mat4 PV, glm::mat4 modelmatrix) const
{
    if(renderer->getSelectMode()){
        renderer->useShader(pickingShader);
        // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        int r = (vao & 0x000000FF) >>  0;
        int g = (vao & 0x0000FF00) >>  8;
        int b = (vao & 0x00FF0000) >> 16;
        // std::cout<<"r: "<<r<<", g: "<<g<<", b"<<b<<"\n";
        // int r = 1;
        // int g = 1;
        // int b = 1;
        glUniform4f(glGetUniformLocation(pickingShader->id(), "pickingColor"), r/255.0f, g/255.0f, b/255.0f, 1.0f);
        glUniformMatrix4fv(glGetUniformLocation(pickingShader->id(),"projection_view"),1,GL_FALSE,(const GLfloat*)glm::value_ptr(PV));
        glUniformMatrix4fv(glGetUniformLocation(pickingShader->id(),"model"),1,GL_FALSE,(const GLfloat*)glm::value_ptr(modelmatrix));
        // glUniform3fv(glGetUniformLocation(pickingShader->id(),"objectColor"),1,(const GLfloat*)glm::value_ptr(getObjectColor()));
        // glUniform3fv(glGetUniformLocation(pickingShader->id(),"cameraPos"),1,(const GLfloat*)glm::value_ptr(renderer->getCameraPos()));
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D,texture_id);
        glDrawElements(GL_TRIANGLES, num_elements_global, GL_UNSIGNED_INT, 0);
        
        // renderer->setSelectMode(false,0,0);
    }else{
        renderer->useShader(shader);
        // // shader->setXform((const GLfloat*)glm::value_ptr(rendermat));
        // // glUniformMatrix4fv(MVPid, 1, GL_FALSE, mvp);
        glUniformMatrix4fv(glGetUniformLocation(shader->id(),"projection_view"),1,GL_FALSE,(const GLfloat*)glm::value_ptr(PV));
        glUniformMatrix4fv(glGetUniformLocation(shader->id(),"model"),1,GL_FALSE,(const GLfloat*)glm::value_ptr(modelmatrix));
        glUniform3fv(glGetUniformLocation(shader->id(),"objectColor"),1,(const GLfloat*)glm::value_ptr(getObjectColor()));
        glUniform3fv(glGetUniformLocation(shader->id(),"cameraPos"),1,(const GLfloat*)glm::value_ptr(renderer->getCameraPos()));
        // // std::cout<<glm::to_string(renderer->getCameraPos())<<"\n";
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D,texture_id);
        // // std::cout<<"num global: "<<num_elements_global<<"\n";
        glDrawElements(GL_TRIANGLES, num_elements_global, GL_UNSIGNED_INT, 0);
    }
    
    // glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		// Re-clear the screen for real rendering
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    return glGetError();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  Generating vertices for the Sphere
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SpherePoints(float radius,int num_sectors,int num_stacks,float array[],float normals[],float texel[]){

    int index = 0;
    // float invR = 1/radius;
    int index1 = 0;
    int index2 =0;
    for(int i=0;i<=num_stacks;i++){
        float phi = (-1*glm::pi<float>()/2) +  ((float)i*glm::pi<float>())/((float)num_stacks);
        for(int j=0;j<=num_sectors;j++){
            float theta = (2*glm::pi<float>())*((float)j)/num_sectors;
            // x y z
            // x = r * cosphi * costheta
            // y = r * costheta * sintheta
            // z = r * sintheta
            array[index++] = radius * cos(phi) * cos(theta);
            array[index++] = radius * cos(phi) * sin(theta);
            array[index++] = radius * sin(phi);
            array[index++] = cos(phi) * cos(theta);
            array[index++] = cos(phi)* sin(theta);
            array[index++] = sin(phi);
            array[index++] = theta/(2*glm::pi<float>());
            array[index++] = phi/glm::pi<float>();
            // array[index++] = (float)j/num_sectors;
            // array[index++] = (float)i/num_stacks;

            normals[index1++] = cos(phi) * cos(theta);
            normals[index1++] = cos(phi)* sin(theta);
            normals[index1++] = sin(phi);

            texel[index2++] = (float)j/num_sectors;
            texel[index2++] = (float)i/num_stacks;
        }
    }

    return ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  Generating indices for the sphere
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int ElementsListSphere(int num_sectors,int num_stacks,GLuint ele[]){
    
    int index = 0;

    for(int i=0;i<num_stacks;i++){
        int k1 = i*(num_sectors+1);
        int k2 = (i+1)*(num_sectors+1);
        for(int j=0;j<num_sectors;j++){
            if(i!=0){
                ele[index++] = k1;
                ele[index++] = k2;
                ele[index++] = k1+1; 
            }
            if(i!=num_stacks){
                ele[index++] = k1+1;
                ele[index++] = k2;
                ele[index++] = k2+1;
            }
            k1++;
            k2++;
        }
    }

    return index;
}