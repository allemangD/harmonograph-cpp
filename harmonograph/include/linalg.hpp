#pragma once

#include <cmath>
#include <vector>
#include <algorithm>

template<unsigned N>
using vec = std::array<float, N>;

using vec1 = vec<1>;
using vec2 = vec<2>;
using vec3 = vec<3>;
using vec4 = vec<4>;
using vec5 = vec<5>;

template<unsigned N>
using mat = std::array<std::array<float, N>, N>;

using mat1 = mat<1>;
using mat2 = mat<2>;
using mat3 = mat<3>;
using mat4 = mat<4>;
using mat5 = mat<5>;

template<class V>
V operator*(V a, const float &b) {
    for (auto &e : a) e *= b;
    return a;
}

template<class V>
V operator*(const float &b, V a) {
    for (auto &e : a) e *= b;
    return a;
}

template<class V>
V operator/(V a, const float &b) {
    for (auto &e : a) e /= b;
    return a;
}

template<class V>
V operator+(const V &a, V b) {
    for (int i = 0; i < a.size(); ++i) {
        a[i] += b[i];
    }
    return a;
}

template<class V>
V operator-(V a, const V &b) {
    for (int i = 0; i < a.size(); ++i) {
        a[i] -= b[i];
    }
    return a;
}

template<class V>
void operator-=(V &a, const V &b) {
    for (int i = 0; i < a.size(); ++i) {
        a[i] -= b[i];
    }
}

template<class V>
void operator+=(V &a, const V &b) {
    for (int i = 0; i < a.size(); ++i) {
        a[i] += b[i];
    }
}

template<class V>
float length(const V &a) {
    float sum = 0;
    for (const auto &e : a) sum += e * e;
    return sqrtf(sum);
}

template<class V>
V normalized(const V &a) {
    return a / length(a);
}

template<class V>
float dot(int n, const V &a, const V &b) {
    float sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += a[i] * b[i];
    }
    return sum;
}

template<class V>
float dot(const V &a, const V &b) {
    float sum = 0;
    for (int i = 0; i < a.size(); ++i) {
        sum += a[i] * b[i];
    }
    return sum;
}

vec5 mul(vec5 v, mat5 m) {
    vec5 r{};

    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            r[i] += m[i][j] * v[j];

    return r;
}

mat5 mul(mat5 a, mat5 b) {
    mat5 r{};

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            for (int k = 0; k < 5; ++k) {
                r[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    return r;
}

template<unsigned N>
vec<N> stereo(const vec<N + 1> &v) {
    vec<N> r;
    for (int i = 0; i < N; ++i) {
        r[i] = v[i] / (1 - v[N]);
    }
    return r;
}

template<unsigned N>
vec<N> ortho(const vec<N + 1> &v) {
    vec<N> r;
    for (int i = 0; i < N; ++i) {
        r[i] = v[i];
    }
    return r;
}

template<class V>
V project(const V &vec, const V &target) {
    return dot(vec, target) / dot(target, target) * target;
}

template<class V>
V reject(const V &vec, const V &target) {
    return vec - project(vec, target);
}

template<class V>
V reflect(const V &a, const V &axis) {
    return a - 2.f * project(a, axis);
}

template<unsigned N>
mat<N> identity() {
    mat<N> res{};
    for (int i = 0; i < N; ++i)
        res[i][i] = 1;
    return res;
}

mat4 orthoProj(float width, float height, float depth) {
    mat4 res{};
    res[0][0] = 2 / width;
    res[1][1] = 2 / height;
    res[2][2] = 2 / depth;
    res[3][3] = 1;
    return res;
}