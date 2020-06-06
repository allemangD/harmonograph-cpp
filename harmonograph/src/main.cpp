#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cgl/shader.hpp>
#include <cgl/program.hpp>
#include <cgl/buffer.hpp>

#include <linalg.hpp>

struct Pendulum {
    vec2 major;
    vec2 _0;
    vec2 minor;
    vec2 _1;

    float phase;
    float period;
    float decay;
    vec1 _2;

    Pendulum(vec2 major, vec2 minor, float phase, float period, float decay)
        : major(major), minor(minor), phase(phase), period(period), decay(decay) {}
};

void run(GLFWwindow *window) {
    auto vs = cgl::sh::vert::file("shaders/main.vs.glsl");
    auto fs = cgl::sh::frag::file("shaders/main.fs.glsl");
    auto pgm = cgl::Program();
    pgm.attach(vs);
    pgm.attach(fs);
    pgm.link();

    float low = 0;
    float high = 2 << 15;
    unsigned int count = 2 << 20;

    std::vector<Pendulum> pendula = {
        Pendulum({.75, 0}, {0, 0}, 0, 3.001, 0.001),
        Pendulum({0, 0.5}, {0, 0}, 0, 4.001, 0.001),
        Pendulum({.25, .25}, {0, 0}, 0, 1.001, 0.001),
        Pendulum({-.25, -.25}, {0, 0}, 0, 2.999, 0.001),
    };

    cgl::Buffer<Pendulum> ubo{};
    ubo.put(pendula);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, ubo);

    while (!glfwWindowShouldClose(window)) {
        float time = glfwGetTime();

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glLineWidth(1);

        glUseProgram(pgm);
        glUniform2f(0, low, high);
        glUniform1f(1, 0);
        glUniform1i(2, count);
        glUniform1i(3, pendula.size());

        glDrawArrays(GL_LINE_STRIP, 0, count);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_VERSION_MINOR, 5);

    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_DEPTH_BITS, 0);
    glfwWindowHint(GLFW_STENCIL_BITS, 0);
    glfwWindowHint(GLFW_SAMPLES, 32);

    auto window = glfwCreateWindow(
        1920, 1080,
        "Harmonograph",
        nullptr, nullptr
    );

    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    run(window);

    glfwTerminate();
    return EXIT_SUCCESS;
}
