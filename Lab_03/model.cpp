#include "model.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

Model::Model(const char *filename)
    : verts_(), faces_(), norms_(), uv_(),
    diffusemap_(), normalmap_(), specularmap_() {

    std::cout << "Loading model " << filename << std::endl;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn,err;

    std::string filepath(filename);
    size_t last_slash = filepath.find_last_of("/\\");
    std::string base_dir = (last_slash != std::string::npos)
    ? filepath.substr(0, last_slash + 1): "";

    bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename, base_dir.c_str());

    if (!success) {
        std::cerr << "Error: " << err << std::endl;
        return;
    }

    if (!warn.empty()) {
        std::cerr << "Warning: " << warn << std::endl;
    }

    for (size_t i = 0; i < attrib.vertices.size(); i+=3) {
        verts_.push_back(Vec3f(
            attrib.vertices[i],
            attrib.vertices[i+1],
            attrib.vertices[i+2]));
    }
    for (size_t i = 0; i < attrib.normals.size(); i += 3) {
        norms_.push_back(Vec3f(
            attrib.normals[i],
            attrib.normals[i + 1],
            attrib.normals[i + 2]
        ));
    }
    for (size_t i = 0; i < attrib.texcoords.size(); i += 2) {
        uv_.push_back(Vec2f(
            attrib.texcoords[i],
            1.0f - attrib.texcoords[i + 1]  // flip V
        ));
    }
    for (const auto& shape : shapes) {
        size_t index_offset = 0;

        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            int fv = shape.mesh.num_face_vertices[f];

            if (fv == 3) {
                std::vector<Vec3i> face_indices;

                for (int v = 0; v < fv; v++) {
                    tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

                    Vec3f indices;
                    indices[0] = idx.vertex_index; //v
                    indices[1] = idx.texcoord_index; //vt
                    indices[2] = idx.normal_index; //vn

                    face_indices.push_back(indices);
                }

                faces_.push_back(face_indices);
            }

            index_offset += fv;
        }
    }
    std::cerr << "# v#" << verts_.size()
        << " f#" << faces_.size()
        << " vt#" << uv_.size()
        << " vn#" << norms_.size()
        << std::endl;

    load_texture(filename, "diffuse.tga",diffusemap_);
    load_texture(filename, "spec.tga",specularmap_);
    load_texture(filename, "nm.tga",normalmap_);

    if (!materials.empty()) {
        for (const auto& mat : materials) {
            if (!mat.diffuse_texname.empty()) {
                std::string tex_path = base_dir + mat.diffuse_texname;
                std::cerr << "Trying diffuse texture: " << tex_path << std::endl;
                if (diffusemap_.read_tga_file(tex_path.c_str())) {
                    diffusemap_.flip_vertically();
                    std::cerr << "Loaded diffuse from material" << std::endl;
                }
            }
        }
    }
}

Model::~Model(){}

int Model::nverts() {
    return (int)verts_.size();
}
int Model::nfaces() {
    return (int)faces_.size();
}

std::vector<int> Model::face(int idx) {
    std::vector<int> face;
    if (idx < 0 || idx >= faces_.size()) return face;

    for (int i = 0; i < (int)faces_[idx].size(); i++) {
        face.push_back(faces_[idx][i][0]);
    }
    return face;
}

Vec3f Model::vert(int i) {
    if (i < 0 || i >= verts_.size()) return Vec3f(0, 0, 0);
    return verts_[i];
}

Vec3f Model::vert(int iface, int nthvert) {
    if (iface < 0 || iface >= faces_.size() ||
        nthvert < 0 || nthvert >= faces_[iface].size()) {
        return Vec3f(0, 0, 0);
    }
    int idx = faces_[iface][nthvert][0];
    if (idx < 0 || idx >= verts_.size()) return Vec3f(0, 0, 0);
    return verts_[idx];
}

void Model::load_texture(std::string filename, const char *suffix, TGAImage &img) {
    std::string textfile(filename);
    size_t dot = textfile.find_last_of(".");
    if (dot != std::string::npos) {
        textfile = textfile.substr(0,dot) + std::string(suffix);
        std::cerr << "texture file" << textfile << " loading"
                  << (img.read_tga_file(textfile.c_str()) ? "ok" : "failed")
                  << std::endl;
        img.flip_vertically();
    }
}

TGAColor Model::diffuse(Vec2f uvf) {
    if (diffusemap_.get_width() == 0) {
        return TGAColor(255,255,255,255);
    }

    Vec2i uv(uvf[0] * diffusemap_.get_width(),
        uvf[1] * diffusemap_.get_height());
    uv[0] = std::max(0, std::min(uv[0], diffusemap_.get_width() - 1));
    uv[1] = std::max(0, std::min(uv[1], diffusemap_.get_height() - 1));

    return diffusemap_.get(uv[0],uv[1]);
}

Vec3f Model::normal(Vec2f uvf) {
    if (normalmap_.get_width() == 0) {
        return Vec3f(0, 0, 1); // нормаль по умолчанию
    }

    Vec2i uv(uvf[0] * normalmap_.get_width(),
             uvf[1] * normalmap_.get_height());
    uv[0] = std::max(0, std::min(uv[0], normalmap_.get_width() - 1));
    uv[1] = std::max(0, std::min(uv[1], normalmap_.get_height() - 1));

    TGAColor c = normalmap_.get(uv[0], uv[1]);
    Vec3f res;
    for (int i = 0; i < 3; i++) {
        res[2 - i] = (float)c[i] / 255.f * 2.f - 1.f; // BGR to RGB и [-1,1]
    }
    return res;
}

Vec2f Model::uv(int iface, int nthvert) {
    if (iface < 0 || iface >= faces_.size() ||
        nthvert < 0 || nthvert >= faces_[iface].size() ||
        uv_.empty()) {
        return Vec2f(0, 0);
        }

    int idx = faces_[iface][nthvert][1];
    if (idx < 0 || idx >= uv_.size()) return Vec2f(0, 0);
    return uv_[idx];
}

float Model::specular(Vec2f uvf) {
    if (specularmap_.get_width() == 0) {
        return 1.0f; // по умолчанию
    }

    Vec2i uv(uvf[0] * specularmap_.get_width(),
             uvf[1] * specularmap_.get_height());
    uv[0] = std::max(0, std::min(uv[0], specularmap_.get_width() - 1));
    uv[1] = std::max(0, std::min(uv[1], specularmap_.get_height() - 1));

    return specularmap_.get(uv[0], uv[1])[0] / 1.f;
}

Vec3f Model::normal(int iface, int nthvert) {
    if (iface < 0 || iface >= faces_.size() ||
        nthvert < 0 || nthvert >= faces_[iface].size()) {
        return Vec3f(0, 0, 1);
        }

    int idx = faces_[iface][nthvert][2];

    // Если есть нормаль из файла
    if (idx >= 0 && idx < norms_.size()) {
        return norms_[idx].normalize();
    }

    // Иначе вычисляем нормаль грани
    Vec3f v0 = vert(iface, 0);
    Vec3f v1 = vert(iface, 1);
    Vec3f v2 = vert(iface, 2);

    Vec3f edge1 = v1 - v0;
    Vec3f edge2 = v2 - v0;
    Vec3f normal = cross(edge1, edge2);

    return normal.normalize();
}
