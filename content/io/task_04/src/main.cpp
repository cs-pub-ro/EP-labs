#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <math.h>           /* sin         */
#include <fcntl.h>          /* open        */
#include <unistd.h>         /* read, close */
#include <stdlib.h>         /* malloc      */
#include <sys/stat.h>       /* fstat       */

#include "sphere.h"
#include "util.h"

#define SCR_WIDTH  800
#define SCR_HEIGHT 800

#define VTX_SHADER_PATH "shaders/vertex-shpere.glsl"
#define FRG_SHADER_PATH "shaders/fragment-sphere.glsl"

/******************************************************************************
 * Global variables
 ******************************************************************************/

static size_t   n   = 1024;   /* number of sphere surface samples */
static uint32_t vbo = -1;     /* vertex buffer object             */

/******************************************************************************
 * Event callbacks & helpers
 ******************************************************************************/

/* updated viewport on window resize */
static void
callback_fb_resize(GLFWwindow *window, int32_t width, int32_t height)
{
    glViewport(0, 0, width, height);
}

/* keypress handlers */
static void
process_input(GLFWwindow *window)
{
    static bool lb_isdown = false;
    static bool rb_isdown = false;

    /* terminate program */
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    /* double number of points */
    switch (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET)) {
        case GLFW_PRESS:
            /* keep track of key press state */
            if (rb_isdown)
                break;
            rb_isdown = true;

            /* don't let it overflow */
            if (__builtin_clzg(n)) {
                n *= 2;
                gen_sphere(n, &vbo);
            }

            DEBUG("n = %lu", n);
            break;
        case GLFW_RELEASE:
            rb_isdown = false;
            break;
    }

    /* halve number of points */
    switch (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET)) {
        case GLFW_PRESS:
            /* keep track of key press state */
            if (lb_isdown)
                break;
            lb_isdown = true;

            /* don't let it become 0 */
            if (n != 1) {
                n /= 2;
                gen_sphere(n, &vbo);
            }

            DEBUG("n = %lu", n);
            break;
        case GLFW_RELEASE:
            lb_isdown = false;
            break;
    }
}

/* compile_shader - reads shader code from file, compiles it and links it
 *  @file : GLSL file name
 *  @type : shader type
 *  @prog : shader program
 *
 *  @return : 0 if everything went well; !0 otherwise
 *
 * NOTE: Invoke this function in the proper order since the compiled shader is
 *       also linked to the shader program. The output of the current shader
 *       must match the input of the following.
 */
static uint32_t
compile_shader(char *file, GLenum type, uint32_t prog)
{
    uint32_t    shader;     /* shader object ID     */
    char        *source;    /* shader source code   */
    int32_t     fd;         /* GLSL file descriptor */
    struct stat statbuf;    /* file stats           */
    char        log[512];   /* shader compiler log  */
    int32_t     ans;        /* answer               */
    uint32_t    ret = 1;    /* return value         */

    /* open user provided GLSL source file */
    fd = open(file, O_RDONLY);
    RET(fd == -1, 0, "unable to open file (%s)", strerror(errno));

    /* determine file size */
    ans = fstat(fd, &statbuf);
    GOTO(ans == -1, out_close, "unable to stat file (%s)", strerror(errno));

    /* allocate memory for GLSL file contents */
    source = (char *) malloc(statbuf.st_size + 1);
    GOTO(!source, out_close, "unable to allocate buffer (%s)", strerror(errno));

    /* read file contents */
    ans = read(fd, source, statbuf.st_size);
    GOTO(ans == -1, out_free, "unable to read GLSL file (%s)", strerror(errno));
    source[ans] = '\0';

    /* create shader */
    shader = glCreateShader(type);
    GOTO(!shader, out_free, "unable to create shader of type %#x", type);

    /* attach shader source, compile it, and check for errors */
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &ans);
    GOTO(!ans, out_compile_log, "shader \"%s\" compilation failed:", file);

    /* attach compiled shader to program and link it */
    glAttachShader(prog, shader);
    glLinkProgram(prog);

    glGetProgramiv(prog, GL_LINK_STATUS, &ans);
    GOTO(!ans, out_link_log, "shader \"%s\" linking failed:", file);

    /* success */
    ret = 0;
    goto out_delete;

    /* error cleanup path */
out_link_log:
    glGetProgramInfoLog(prog, sizeof(log), NULL, log);
    printf("%s", log);
    goto out_delete;

out_compile_log:
    glGetShaderInfoLog(shader, sizeof(log), NULL, log);
    printf("%s", log);

    /* normal cleanup path */
out_delete:
    glDeleteShader(shader);
out_free:
    free(source);
out_close:
    close(fd);

    return ret;
}

/******************************************************************************
 * Program entry point
 ******************************************************************************/

int32_t
main(int32_t argc, char *argv[])
{
    GLFWwindow    *window;      /* window object                    */
    const GLubyte *renderer;    /* name of the rendering device     */
    uint32_t      prog;         /* shader program                   */
    uint32_t      vao;          /* vertex array object              */
    int32_t       ret = -1;     /* return code on termination       */
    int32_t       ans;          /* answer                           */

    INFO("Press [ to halve number or vertices and ] to double them");

    /* initialize GLFW with OpenGL 4.6 core required */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* create window object and make it current */
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "balls", NULL, NULL);
    GOTO(!window, out_terminate, "failed to create GLFW window");
    glfwMakeContextCurrent(window);

    /* initialize GLAD (depends on GLFW window context existing) */
    ans = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    GOTO(!ans, out_terminate, "failed to initialize GLAD");

    /* display name of rendering device (confirm that it's running on GPU) */
    renderer = glGetString(GL_RENDERER);
    DEBUG("rendered by: %s", renderer);

    /* register event callbacks */
    glfwSetFramebufferSizeCallback(window, callback_fb_resize);

    {
        /* create shader program, compile shaders, link them to program */
        prog = glCreateProgram();
        GOTO(!prog, out_terminate, "failed to create shader program");

        ans = compile_shader((char *) VTX_SHADER_PATH, GL_VERTEX_SHADER, prog);
        GOTO(ans, out_terminate, "vertex shader failed to compile / link");

        ans = compile_shader((char *) FRG_SHADER_PATH, GL_FRAGMENT_SHADER, prog);
        GOTO(ans, out_terminate, "fragment shader failed to compile / link");
    }

    /* generate VAO & VBO with initial vertex data */
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        gen_sphere(n, &vbo);
    }

    /* set vertex display size */
    glPointSize(5);

    /* main render loop */
    while (!glfwWindowShouldClose(window)) {
        /* process input */
        process_input(window);

        /* get rotation angle uniform location in GPU memory */
        int32_t theta_loc = glGetUniformLocation(prog, "theta");
        GOTO(theta_loc == -1, out_dealloc, "failed to get theta location");

        /* clear background */
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* draw points with the data stored in VAO */
        glUseProgram(prog);
        glUniform1f(theta_loc, 0.5f * glfwGetTime());   /* slow down! */
        glDrawArrays(GL_POINTS, 0, n);

        /* swap front and back buffers & poll I/O events */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /* success */
    ret = 0;

out_dealloc:
    glBindVertexArray(0);   /* not part of dealloc, but needs to be done */

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(prog);

out_terminate:
    /* close remaining windows and free resources */
    glfwTerminate();

    return ret;
}

