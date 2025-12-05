#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cmath>
#include <vector>
#include <iostream>
#include <cassert>

/////////////////////////////////////////////////////////////////////////////////

template <typename T> struct vec2;
template <typename T> struct vec3;
template <typename T> struct vec4;

/////////////////////////////////////////////////////////////////////////////////

template <typename T> struct vec2 {
    T x, y;

    vec2() : x(T()), y(T()) {}
    vec2(T _x, T _y) : x(_x), y(_y) {}

    // Конструктор преобразования типа
    template <typename U> vec2(const vec2<U> &v) : x(T(v.x)), y(T(v.y)) {}

    vec2<T>& operator =(const vec2<T> &v) {
        x = v.x;
        y = v.y;
        return *this;
    }

    T& operator[](const int i) {
        assert(i>=0 && i<2);
        return i==0 ? x : y;
    }

    const T& operator[](const int i) const {
        assert(i>=0 && i<2);
        return i==0 ? x : y;
    }

    vec2<T> operator+(const vec2<T> &v) const { return vec2<T>(x+v.x, y+v.y); }
    vec2<T> operator-(const vec2<T> &v) const { return vec2<T>(x-v.x, y-v.y); }
    vec2<T> operator*(float f) const { return vec2<T>(x*f, y*f); }
    vec2<T> operator/(float f) const { assert(f!=0); return vec2<T>(x/f, y/f); }

    float norm() const { return std::sqrt(x*x + y*y); }
    vec2<T>& normalize(float l=1) { *this = (*this)*(l/norm()); return *this; }
};

/////////////////////////////////////////////////////////////////////////////////

template <typename T> struct vec3 {
    T x, y, z;

    vec3() : x(T()), y(T()), z(T()) {}
    vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

    // Конструктор преобразования типа
    template <typename U> vec3(const vec3<U> &v) : x(T(v.x)), y(T(v.y)), z(T(v.z)) {}

    vec3<T>& operator =(const vec3<T> &v) {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }

    T& operator[](const int i) {
        assert(i>=0 && i<3);
        return i==0 ? x : (i==1 ? y : z);
    }

    const T& operator[](const int i) const {
        assert(i>=0 && i<3);
        return i==0 ? x : (i==1 ? y : z);
    }

    vec3<T> operator+(const vec3<T> &v) const { return vec3<T>(x+v.x, y+v.y, z+v.z); }
    vec3<T> operator-(const vec3<T> &v) const { return vec3<T>(x-v.x, y-v.y, z-v.z); }
    vec3<T> operator*(float f) const { return vec3<T>(x*f, y*f, z*f); }
    vec3<T> operator/(float f) const { assert(f!=0); return vec3<T>(x/f, y/f, z/f); }

    float norm() const { return std::sqrt(x*x + y*y + z*z); }
    vec3<T>& normalize(float l=1) { *this = (*this)*(l/norm()); return *this; }
};

/////////////////////////////////////////////////////////////////////////////////

template <typename T> struct vec4 {
    T x, y, z, w;

    vec4() : x(T()), y(T()), z(T()), w(T()) {}
    vec4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}

    template <typename U> vec4(const vec4<U> &v) : x(T(v.x)), y(T(v.y)), z(T(v.z)), w(T(v.w)) {}

    vec4<T>& operator =(const vec4<T> &v) {
        x = v.x;
        y = v.y;
        z = v.z;
        w = v.w;
        return *this;
    }

    T& operator[](const int i) {
        assert(i>=0 && i<4);
        return i==0 ? x : (i==1 ? y : (i==2 ? z : w));
    }

    const T& operator[](const int i) const {
        assert(i>=0 && i<4);
        return i==0 ? x : (i==1 ? y : (i==2 ? z : w));
    }

    vec4<T> operator*(float f) const { return vec4<T>(x*f, y*f, z*f, w*f); }
    vec4<T> operator/(float f) const { assert(f!=0); return vec4<T>(x/f, y/f, z/f, w/f); }
};

// Типопределения для удобства
typedef vec2<float> Vec2f;
typedef vec2<int>   Vec2i;
typedef vec3<float> Vec3f;
typedef vec3<int>   Vec3i;
typedef vec4<float> Vec4f;
typedef vec4<int>   Vec4i;

/////////////////////////////////////////////////////////////////////////////////

// Функции для работы с векторами
template <typename T>
T dot(const vec2<T>& v1, const vec2<T>& v2) {
    return v1.x*v2.x + v1.y*v2.y;
}

template <typename T>
T dot(const vec3<T>& v1, const vec3<T>& v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

template <typename T>
vec3<T> cross(const vec3<T>& v1, const vec3<T>& v2) {
    return vec3<T>(v1.y*v2.z - v1.z*v2.y,
                   v1.z*v2.x - v1.x*v2.z,
                   v1.x*v2.y - v1.y*v2.x);
}

// Оператор умножения для скалярного произведения vec3
template <typename T>
float operator*(const vec3<T>& v1, const vec3<T>& v2) {
    return dot(v1, v2);
}

// Оператор умножения для скалярного произведения vec2
template <typename T>
float operator*(const vec2<T>& v1, const vec2<T>& v2) {
    return dot(v1, v2);
}

/////////////////////////////////////////////////////////////////////////////////

// Вспомогательные функции

// Функция proj для понижения размерности (vec4 -> vec3, vec3 -> vec2 и т.д.)
template<typename T>
Vec2f proj2(const Vec3f& v) {
    return Vec2f(v.x, v.y);
}

template<typename T>
Vec2f proj2(const Vec4f& v) {
    return Vec2f(v.x, v.y);
}

// Обобщенная версия proj
template<int D, typename T>
vec2<T> proj(const vec3<T>& v) {
    if constexpr (D == 2) {
        return vec2<T>(v.x, v.y);
    } else {
        static_assert(D == 2, "Only projection to 2D is supported");
    }
}

template<int D, typename T>
vec2<T> proj(const vec4<T>& v) {
    if constexpr (D == 2) {
        return vec2<T>(v.x, v.y);
    } else {
        static_assert(D == 2, "Only projection to 2D is supported");
    }
}

// Функция embed для повышения размерности
template<typename T>
Vec3f embed3(const Vec2f& v, T fill = T()) {
    return Vec3f(v.x, v.y, fill);
}

template<typename T>
Vec4f embed4(const Vec3f& v, T fill = T()) {
    return Vec4f(v.x, v.y, v.z, fill);
}

// Обобщенная версия embed
template<int D, typename T>
vec3<T> embed(const vec2<T>& v, T fill = T()) {
    if constexpr (D == 3) {
        return vec3<T>(v.x, v.y, fill);
    } else {
        static_assert(D == 3, "Only embedding to 3D is supported");
    }
}

template<int D, typename T>
vec4<T> embed(const vec3<T>& v, T fill = T()) {
    if constexpr (D == 4) {
        return vec4<T>(v.x, v.y, v.z, fill);
    } else {
        static_assert(D == 4, "Only embedding to 4D is supported");
    }
}

// Операторы вывода
template <typename T>
std::ostream& operator<<(std::ostream& s, const vec2<T>& v) {
    s << "(" << v.x << ", " << v.y << ")";
    return s;
}

template <typename T>
std::ostream& operator<<(std::ostream& s, const vec3<T>& v) {
    s << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return s;
}

template <typename T>
std::ostream& operator<<(std::ostream& s, const vec4<T>& v) {
    s << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return s;
}

/////////////////////////////////////////////////////////////////////////////////
// Класс Matrix
/////////////////////////////////////////////////////////////////////////////////

class Matrix {
    std::vector<std::vector<float> > m;
    int rows, cols;

public:
    Matrix(int r=4, int c=4);
    static Matrix identity(int dimensions=4);

    int nrows() const;
    int ncols() const;

    std::vector<float>& operator[](const int i);
    const std::vector<float>& operator[](const int i) const;

    Matrix operator*(const Matrix& a) const;

    // Умножение матрицы на вектор
    Vec4f operator*(const Vec4f& v) const;

    friend std::ostream& operator<<(std::ostream& s, const Matrix& m);
};

// Реализация Matrix прямо в заголовочном файле (inline)
inline Matrix::Matrix(int r, int c) : rows(r), cols(c), m(std::vector<std::vector<float> >(r, std::vector<float>(c, 0.f))) {
    if (rows==cols) {
        for (int i=0; i<rows; i++) {
            m[i][i] = 1;
        }
    }
}

inline Matrix Matrix::identity(int dimensions) {
    Matrix E(dimensions, dimensions);
    for (int i=0; i<dimensions; i++) {
        for (int j=0; j<dimensions; j++) {
            E[i][j] = (i==j ? 1.f : 0.f);
        }
    }
    return E;
}

inline int Matrix::nrows() const {
    return rows;
}

inline int Matrix::ncols() const {
    return cols;
}

inline std::vector<float>& Matrix::operator[](const int i) {
    assert(i>=0 && i<rows);
    return m[i];
}

inline const std::vector<float>& Matrix::operator[](const int i) const {
    assert(i>=0 && i<rows);
    return m[i];
}

inline Matrix Matrix::operator*(const Matrix& a) const {
    assert(cols == a.rows);
    Matrix result(rows, a.cols);
    for (int i=0; i<rows; i++) {
        for (int j=0; j<a.cols; j++) {
            result.m[i][j] = 0.f;
            for (int k=0; k<cols; k++) {
                result.m[i][j] += m[i][k] * a.m[k][j];
            }
        }
    }
    return result;
}

inline Vec4f Matrix::operator*(const Vec4f& v) const {
    assert(cols == 4);
    Vec4f result;
    for (int i=0; i<rows; i++) {
        result[i] = 0.f;
        for (int j=0; j<cols; j++) {
            result[i] += m[i][j] * v[j];
        }
    }
    return result;
}

inline std::ostream& operator<<(std::ostream& s, const Matrix& m) {
    for (int i=0; i<m.nrows(); i++) {
        for (int j=0; j<m.ncols(); j++) {
            s << m[i][j];
            if (j<m.ncols()-1) s << "\t";
        }
        s << "\n";
    }
    return s;
}

#endif //__GEOMETRY_H__