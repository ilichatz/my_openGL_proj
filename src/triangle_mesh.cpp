#include "triangle_mesh.h"

TriangleMesh::TriangleMesh() {
    

    // coordinates below are given in NDC format:
    // Normalized Device Coordinates
    std::vector<float> positions = {
        -1.0f, -1.0f, 0.0f, 
         1.f,  -1.0f, 0.0f, 
        -1.0f,  1.0f, 0.0f,
         1.0f,  0.6f, 0.0f,
    };

    std::vector<int> colorIndices = {
        0, 1, 2, 3
    };
    std::vector<int> elements = {
        0, 1, 2, 2, 1, 3
    };
    element_count = 6;
    
    // In openGL a "vertex" is the collection of attributes 
    // needed to draw a point (6 above before splitting them to 3+3)
  

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    VBOs.resize(2);


    //position
    glGenBuffers(1, &VBOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), 
        positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, (void*)0);
    glEnableVertexAttribArray(0);

    //color indices
    glGenBuffers(1, &VBOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, colorIndices.size() * sizeof(int), 
        colorIndices.data(), GL_STATIC_DRAW);
    glVertexAttribIPointer(1, 1, GL_INT, 4, (void*)0);
    glEnableVertexAttribArray(1);

    //element buffer
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(int), 
        elements.data(), GL_STATIC_DRAW);
    



}

void TriangleMesh::draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, element_count , GL_UNSIGNED_INT, 0);
}

TriangleMesh::~TriangleMesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(VBOs.size(), VBOs.data());
    glDeleteBuffers(1, &EBO);
}