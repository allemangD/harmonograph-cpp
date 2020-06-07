#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cgl/shader.hpp>
#include <cgl/program.hpp>
#include <cgl/buffer.hpp>

#include <linalg.hpp>

#include <yaml-cpp/yaml.h>

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

    std::vector<Pendulum> pendula{};

    auto scene = YAML::LoadFile("presets/sample.yaml");
    for (auto pen : scene["pendula"]) {
        vec2 cos{};
        vec2 sin{};
        float phase = 0;
        float period = 1;
        float decay = 0;

        if (pen["cos"].IsDefined())
            cos = pen["cos"].as<vec2>();
        if (pen["sin"].IsDefined())
            sin = pen["sin"].as<vec2>();

        if (pen["phase"].IsDefined())
            phase = pen["phase"].as<float>();
        if (pen["period"].IsDefined())
            period = pen["period"].as<float>();
        if (pen["decay"].IsDefined())
            decay = pen["decay"].as<float>();

        pendula.emplace_back(cos, sin, phase, period, decay);
    }

    cgl::Buffer<Pendulum> ubo{};
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, ubo);

    while (!glfwWindowShouldClose(window)) {
        float time = glfwGetTime();

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        float scale = 3.5;
        mat4 proj = orthoProj(scale * width / height, scale, 2.0f);
        glUniformMatrix4fv(9, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(proj.data()));

        glViewport(0, 0, width, height);
        glClearColor(1, 1, 1, 1);
        glUniform4f(1, 0, 0, 0, 0.1);
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glLineWidth(1);

        unsigned int count = 2 << 19;
        glUniform1i(2, count);

        {
            float low = 0;
            float high = count / 48;
            float anim = std::exp(time / 5) - 1;
            float cap = std::log(high) * 5;

            if (time < cap)
                glUniform2f(0, low, std::min(anim, high));
            else
                glUniform2f(0, low, high);
        }

        ubo.put(pendula);
        glUniform1i(3, pendula.size());

        glUseProgram(pgm);
        glLineWidth(2);
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
