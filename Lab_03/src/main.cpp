#include <vector>
#include <iostream>

#include "../Include/tgaimage.h"
#include "../Include/mesh.h"
#include "../Include/math.h"
#include "../Include/our_gl.h"
#include "../Include/camera.h"

Model *model     = NULL;
const int width  = 800;
const int height = 800;

Vec3f light_dir(1,1,1);

Camera cam(
    /* eye    */ Vec3f(1, 1, 3),
    /* center */ Vec3f(0, 0, 0),
    /* up     */ Vec3f(0, 1, 0),
    /* fov    */ 60.f,
    /* near   */ 1.f,
    /* far    */ 1000.f
);

struct Shader : public IShader {
    Vec3f          varying_intensity;
    mat<2,3,float> varying_uv;

    virtual Vec4f vertex(int iface, int nthvert) {
        varying_uv.set_col(nthvert, model->uv(iface, nthvert));
        varying_intensity[nthvert] = std::max(0.f, model->normal(iface, nthvert)*light_dir);
        Vec4f gl_Vertex = embed<4>(model->vert(iface, nthvert));
        return Viewport*Projection*ModelView*gl_Vertex;
    }

    virtual bool fragment(Vec3f bar, TGAColor &color) {
        float intensity = varying_intensity*bar;
        Vec2f uv = varying_uv*bar;
        color = model->diffuse(uv)*intensity;
        return false;
    }
};

int main(int argc, char** argv) {
    if (2==argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("../obj/african_head.obj");
    }

    cam.applyView();
    cam.applyProjection(width, height);
    viewport(width/8, height/8, width*3/4, height*3/4);
    light_dir.normalize();

    TGAImage image  (width, height, TGAImage::RGB);
    TGAImage zbuffer(width, height, TGAImage::GRAYSCALE);

    Shader shader;
    for (int i=0; i<model->nfaces(); i++) {
        Vec4f screen_coords[3];
        for (int j=0; j<3; j++) {
            screen_coords[j] = shader.vertex(i, j);
        }
        triangle(screen_coords, shader, image, zbuffer);
    }

    image.  flip_vertically(); // to place the origin in the bottom left corner of the image
    zbuffer.flip_vertically();
    image.  write_tga_file("output.tga");
    zbuffer.write_tga_file("zbuffer.tga");

    delete model;
    return 0;
}