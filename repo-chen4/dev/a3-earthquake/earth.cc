/** CSci-4611 Assignment 3:  Earthquake
 */

#include "earth.h"
#include "config.h"

#include <vector>

// for M_PI constant
#define _USE_MATH_DEFINES
#include <math.h>


Earth::Earth() {
}

Earth::~Earth() {
}

void Earth::Init(const std::vector<std::string> &search_path) {
    // init shader program
    shader_.Init();
    
    // init texture: you can change to a lower-res texture here if needed
    earth_tex_.InitFromFile(Platform::FindFile("earth-2k.png", search_path));

    // init geometry
    const int nslices = 10;
    const int nstacks = 10;
    
    DrawPlane(nslices, nstacks);
  
}



void Earth::Draw(const Matrix4 &model_matrix, const Matrix4 &view_matrix, const Matrix4 &proj_matrix) {
    // Define a really bright white light.  Lighting is a property of the "shader"
    DefaultShader::LightProperties light;
    light.position = Point3(10,10,10);
    light.ambient_intensity = Color(1,1,1);
    light.diffuse_intensity = Color(1,1,1);
    light.specular_intensity = Color(1,1,1);
    shader_.SetLight(0, light);

    // Adust the material properties, material is a property of the thing
    // (e.g., a mesh) that we draw with the shader.  The reflectance properties
    // affect the lighting.  The surface texture is the key for getting the
    // image of the earth to show up.
    DefaultShader::MaterialProperties mat;
    mat.ambient_reflectance = Color(0.5, 0.5, 0.5);
    mat.diffuse_reflectance = Color(0.75, 0.75, 0.75);
    mat.specular_reflectance = Color(0.75, 0.75, 0.75);
    mat.surface_texture = earth_tex_;

    // Draw the earth mesh using these settings
    if (earth_mesh_.num_triangles() > 0) {
        shader_.Draw(model_matrix, view_matrix, proj_matrix, &earth_mesh_, mat);
    }
}

void Earth::DrawEarthquake(double longitude, double latitude, double magnitude, const Matrix4 &model_matrix, const Matrix4 &view_matrix, const Matrix4 &proj_matrix) {
    Color ballcol = Color(magnitude/10, (10 - magnitude)/10, (magnitude - 5)/10);
    Point3 location;
    if (GlobeMode) {
        location = LatLongToSphere(latitude, longitude);
    } else {
        location = LatLongToPlane(latitude, longitude);
    }
    double ballsize = (magnitude - 5) / 50;
    Matrix4 ball =
        Matrix4::Translation(location - Point3(0,0,0) ) *
        Matrix4::Scale(Vector3(ballsize, ballsize, ballsize));
    quick_shapes_.DrawSphere(model_matrix * ball, view_matrix, proj_matrix, ballcol);
}

void Earth::DrawPlane(const int nslices, const int nstacks) {
    std::vector<unsigned int> indices;
    std::vector<Point3> vertices;
    std::vector<Point2> tex_coords;
    std::vector<Vector3> normals;
    
    
    // Forming vertices
    // From bottom left to upper right
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    for (double i = -M_PI; i <= M_PI; i += (2*M_PI)/nslices) {
        for (double j = -M_PI/2; j <= M_PI/2; j += M_PI/nstacks) {
            vertices.push_back(Point3(i,j,0));
            normals.push_back(Vector3(0,0,1).ToUnit());
            if (0) {
                std::cout << i << ", " << j << "\n";
            }
        }
    }
    // Dealing with texture.
    double x,y;
    for (int i = 0; i < nslices + 1; i++) {
        for (int j = nstacks; j >= 0 ; j--) {
            x = (double) i / (double) nslices;
            y = (double) j / (double) nstacks;
            tex_coords.push_back(Point2(x,y));
            if (0) {
                std::cout << x << "," << y << "\n";
            }
        }
    }
    
    // Forming Indicies
    int z, offset = 0;
    for (int i = 0; i < nslices; i++) {
        for (int j = 0; j < nstacks; j++) {
            z = i * (nstacks + 1);
            offset = z + j;
            // 0 3 1
            if (0) {
                std::cout << offset<< "\n";
                std::cout << offset + nstacks + 2 << "\n";
                std::cout << offset + 1 << "\n";
            }
            indices.push_back(offset);
            indices.push_back(offset + nstacks + 2);
            indices.push_back(offset + 1);
            // 0 2 3
            if (0) {
                std::cout << offset << "\n";
                std::cout << offset + nstacks + 1 << "\n";
                std::cout << offset + nstacks + 2 << "\n";
            }
            indices.push_back(offset);
            indices.push_back(offset + nstacks + 1);
            indices.push_back(offset + nstacks + 2);
        }
    }
    
    
    earth_mesh_.SetVertices(vertices);
    earth_mesh_.SetIndices(indices);
    earth_mesh_.SetTexCoords(0, tex_coords);
    earth_mesh_.SetNormals(normals);
    earth_mesh_.UpdateGPUMemory();

}

void Earth::DrawGlobe(const int nslices, const int nstacks) {
    // init
    std::vector<unsigned int> indices;
    std::vector<Point3> vertices;
    std::vector<Point2> tex_coords;
    std::vector<Vector3> normals;
    
    
    // Forming vertices
    // From bottom left to upper right
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    double x_cor,y_cor,z_cor;
    for (double i = -M_PI; i <= M_PI; i += (2*M_PI)/nslices) {
        for (double j = -M_PI/2; j <= M_PI/2; j += M_PI/nstacks) {
            x_cor = cos(j) * sin(i);
            y_cor = sin(j);
            z_cor = cos(j) * cos(i);
            vertices.push_back(Point3(x_cor, y_cor, z_cor));
            normals.push_back(Vector3(cos(-i),sin(-i),1).ToUnit());
            if (0) {
                std::cout << i << ", " << j << "\n";
            }
        }
    }
    // Dealing with texture.
    double x,y;
    for (int i = 0; i < nslices + 1; i++) {
        for (int j = nstacks; j >= 0 ; j--) {
            x = (double) i / (double) nslices;
            y = (double) j / (double) nstacks;
            tex_coords.push_back(Point2(x,y));
            if (0) {
                std::cout << x << "," << y << "\n";
            }
        }
    }
    
    // Forming Indicies
    int z, offset = 0;
    for (int i = 0; i < nslices; i++) {
        for (int j = 0; j < nstacks; j++) {
            z = i * (nstacks + 1);
            offset = z + j;
            // 0 3 1
            if (0) {
                std::cout << offset<< "\n";
                std::cout << offset + nstacks + 2 << "\n";
                std::cout << offset + 1 << "\n";
            }
            indices.push_back(offset);
            indices.push_back(offset + nstacks + 2);
            indices.push_back(offset + 1);
            // 0 2 3
            if (0) {
                std::cout << offset << "\n";
                std::cout << offset + nstacks + 1 << "\n";
                std::cout << offset + nstacks + 2 << "\n";
            }
            indices.push_back(offset);
            indices.push_back(offset + nstacks + 1);
            indices.push_back(offset + nstacks + 2);
        }
    }
    
    
    //Sending out to GPU
    earth_mesh_.SetVertices(vertices);
    earth_mesh_.SetIndices(indices);
    earth_mesh_.SetTexCoords(0, tex_coords);
//    earth_mesh_.SetNormals(normals);
    earth_mesh_.UpdateGPUMemory();
}


Point3 Earth::LatLongToSphere(double latitude, double longitude) const {
    // TODO: We recommend filling in this function to put all your
    // lat,long --> sphere calculations in one place.
    double x,y,z;
    x = cos(latitude * (M_PI/180)) * sin(longitude * (M_PI/180));
    y = sin(latitude * (M_PI/180));
    z = cos(latitude * (M_PI/180)) * cos(longitude * (M_PI/180));
    return Point3(x,y,z);
}

Point3 Earth::LatLongToPlane(double latitude, double longitude) const {
    // TODO: We recommend filling in this function to put all your
    // lat,long --> plane calculations in one place.
    double x,y;
    x = longitude * (M_PI/180);
    y = latitude * (M_PI/180);
    return Point3(x, y, 0);
}



void Earth::DrawDebugInfo(const Matrix4 &model_matrix, const Matrix4 &view_matrix, const Matrix4 &proj_matrix) {
    // This draws a cylinder for each line segment on each edge of each triangle in your mesh.
    // So it will be very slow if you have a large mesh, but it's quite useful when you are
    // debugging your mesh code, especially if you start with a small mesh.
    for (int t=0; t<earth_mesh_.num_triangles(); t++) {
        std::vector<unsigned int> indices = earth_mesh_.triangle_vertices(t);
        std::vector<Point3> loop;
        loop.push_back(earth_mesh_.vertex(indices[0]));
        loop.push_back(earth_mesh_.vertex(indices[1]));
        loop.push_back(earth_mesh_.vertex(indices[2]));
        quick_shapes_.DrawLines(model_matrix, view_matrix, proj_matrix,
            Color(1,1,0), loop, QuickShapes::LinesType::LINE_LOOP, 0.005);
    }
}

