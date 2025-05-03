#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <math.h>

#include "util.h"

/* gen_sphere - Fibonacci sphere algorithm for vertex generation
 *  @n   : number of samples
 *  @vbo : ptr to Vertex Buffer Object id; VBO will be generated if value is -1
 *
 *  @return : 0 if everything went well; -1 otherwise
 *
 * NOTE: Caller must have already bound a Vertex Array Object but not the
 *       Vertex Buffer Object.
 */
int32_t
gen_sphere(size_t n, uint32_t *vbo)
{
    float *vertices;    /* RAM buffer for vertex data  */
    float phi;          /* golden angle                */
    float theta;        /* golden angle increment      */
    float r;            /* section radius at a given y */
    float x, y, z;      /* coordinates                 */

    /* generate VBO */
    if (*vbo == -1)
        glGenBuffers(1, vbo);

    /* allocate buffer for vertices */
    vertices = (float *) malloc(3 * n * sizeof(float));
    RET(!vertices, -1, "unable to allocate vertex buffer (%s)", strerror(errno));

    /* generate vertices */
    phi = M_PI * (sqrt(5.0f) - 1.0f);
    for (size_t i = 0; i < n; i++) {
        y = 1 - 2.0f * i / (n - 1.0f);
        r = sqrt(1.0f - y * y);
        theta = phi * i;
        x = r * cos(theta);
        z = r * sin(theta);

        vertices[i * 3 + 0] = x;
        vertices[i * 3 + 1] = y;
        vertices[i * 3 + 2] = z;
    }

    double memcpy_initial = glfwGetTime();
    /* upload vertex data to GPU memory and specify data format */
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * n * sizeof(float),
                 vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* free vertices buffer from RAM */
    free(vertices);

    return 0;
}

