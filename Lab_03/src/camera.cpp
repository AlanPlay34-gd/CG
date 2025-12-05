// camera.cpp
#include "../Include/camera.h"
#include <cmath>

Camera::Camera()
    : eye(0, 0, 3), center(0, 0, 0), up(0, 1, 0),
      fov(45.0f), aspect(1.0f), znear(0.1f), zfar(100.0f) {
    updateViewMatrix();
}

Camera::Camera(const Vec3f& eye, const Vec3f& center, const Vec3f& up)
    : eye(eye), center(center), up(up),
      fov(45.0f), aspect(1.0f), znear(0.1f), zfar(100.0f) {
    this->up = up.normalized();
    updateViewMatrix();
}

void Camera::setPosition(const Vec3f& new_eye) {
    eye = new_eye;
    updateViewMatrix();
}

void Camera::setTarget(const Vec3f& new_center) {
    center = new_center;
    updateViewMatrix();
}

void Camera::setUpVector(const Vec3f& new_up) {
    up = new_up;
    up.normalize();
    updateViewMatrix();
}

void Camera::updateViewMatrix() {
    // Создаем матрицу вида (look-at)
    Vec3f z = (eye - center).normalized();
    Vec3f x = cross(up, z).normalize();
    Vec3f y = cross(z, x).normalize();

    Matrix Minv = Matrix::identity();
    Matrix Tr   = Matrix::identity();

    for (int i = 0; i < 3; i++) {
        Minv[0][i] = x[i];
        Minv[1][i] = y[i];
        Minv[2][i] = z[i];
        Tr[i][3] = -center[i];
    }

    ModelView = Minv * Tr;
}

void Camera::setOrthographic(float left, float right, float bottom, float top, float near, float far) {
    znear = near;
    zfar = far;

    Projection = Matrix::identity();
    Projection[0][0] = 2.0f / (right - left);
    Projection[1][1] = 2.0f / (top - bottom);
    Projection[2][2] = -2.0f / (far - near);
    Projection[0][3] = -(right + left) / (right - left);
    Projection[1][3] = -(top + bottom) / (top - bottom);
    Projection[2][3] = -(far + near) / (far - near);
}

void Camera::setViewport(int x, int y, int w, int h) {
    Viewport = Matrix::identity();
    Viewport[0][3] = x + w/2.0f;
    Viewport[1][3] = y + h/2.0f;
    Viewport[2][3] = 1.0f;

    Viewport[0][0] = w/2.0f;
    Viewport[1][1] = h/2.0f;
    Viewport[2][2] = 0;
}

void Camera::moveForward(float distance) {
    Vec3f direction = (center - eye).normalize();
    eye = eye + direction * distance;
    center = center + direction * distance;
    updateViewMatrix();
}

void Camera::moveRight(float distance) {
    Vec3f z = (eye - center).normalize();
    Vec3f x = cross(up, z).normalize();
    eye = eye + x * distance;
    center = center + x * distance;
    updateViewMatrix();
}

void Camera::moveUp(float distance) {
    eye = eye + up * distance;
    center = center + up * distance;
    updateViewMatrix();
}

void Camera::rotate(float angle_x, float angle_y) {
    // Поворот вокруг вертикальной оси (angle_y) и горизонтальной (angle_x)
    Vec3f direction = (center - eye);
    float radius = direction.norm();

    // Вычисляем сферические координаты
    float theta = atan2(direction.z, direction.x);
    float phi = acos(direction.y / radius);

    // Применяем поворот
    theta += angle_y;
    phi += angle_x;

    // Ограничиваем phi чтобы избежать переворота
    phi = std::max(0.1f, std::min((float)M_PI - 0.1f, phi));

    // Преобразуем обратно в декартовы координаты
    direction.x = radius * sin(phi) * cos(theta);
    direction.y = radius * cos(phi);
    direction.z = radius * sin(phi) * sin(theta);

    center = eye + direction;
    updateViewMatrix();
}