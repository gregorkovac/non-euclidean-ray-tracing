#include <../include/Mesh.h>

Mesh::Mesh(Vector position, Vector rotation, Vector scale, Color color, float reflectivity, float translucency, float refractiveIndex, char* colorType, char* objFilePath) : Object(position, rotation, scale, color, reflectivity, translucency, refractiveIndex, colorType) {
    strcpy(this->type_, "Mesh");
    this->parseObjFile(objFilePath);
    lastIntersectionTriangleIndex = -1;
}

void Mesh::parseObjFile(char* objFilePath) {


    FILE* objFile = fopen(objFilePath, "r");

    if (objFile == NULL) {
        printf("Error: Could not open file %s\n", objFilePath);
        exit(1);
    }

    char line[256];
    int numVertices = 0;
    int numFaces = 0;

    while (fgets(line, sizeof(line), objFile)) {
        if (line[0] == 'v' && line[1] == ' ') {
            numVertices++;
        } else if (line[0] == 'f' && line[1] == ' ') {
            numFaces++;
        }
    }

    rewind(objFile);

    Vector* vertices = new Vector[numVertices];
    Triangle* triangles = new Triangle[numFaces];

    int vertexIndex = 0;
    int triangleIndex = 0;

    while (fgets(line, sizeof(line), objFile)) {
        if (line[0] == 'v' && line[1] == ' ') {
            sscanf(line, "v %f %f %f", &vertices[vertexIndex].x, &vertices[vertexIndex].y, &vertices[vertexIndex].z);
            vertexIndex++;
        } else if (line[0] == 'f' && line[1] == ' ') {
            int a, b, c;
            int n1, n2, n3, n4, n5, n6;
            sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &a, &n1, &n2, &b, &n3, &n4, &c, &n5, &n6);
            triangles[triangleIndex].a = vertices[a - 1];
            triangles[triangleIndex].b = vertices[b - 1];
            triangles[triangleIndex].c = vertices[c - 1];

            Matrix M = Matrix::translation(this->position_) * Matrix::rotation(this->rotation_) * Matrix::scale(this->scale_);
            triangles[triangleIndex].a = M * triangles[triangleIndex].a;
            triangles[triangleIndex].b = M * triangles[triangleIndex].b;
            triangles[triangleIndex].c = M * triangles[triangleIndex].c;

            triangles[triangleIndex].normal = (triangles[triangleIndex].b - triangles[triangleIndex].a).cross(triangles[triangleIndex].c - triangles[triangleIndex].a).normalize();
            triangleIndex++;
        }
    }

    fclose(objFile);

    this->numTriangles = numFaces;
    
    for (int i = 0; i < numFaces; i++) {
        this->triangles[i] = triangles[i];
    }
}

float Mesh::equation(Vector v) {
    for (int i = 0; i < this->numTriangles; i++) {
        Vector a = this->triangles[i].a;

        Vector normal = this->triangles[i].normal;

        float d = -(normal * this->triangles[i].a);

        float result = normal * v + d;

        Vector projection = v - (normal * result);

        Vector ab = this->triangles[i].b - this->triangles[i].a;
        Vector bc = this->triangles[i].c - this->triangles[i].b;
        Vector ca = this->triangles[i].a - this->triangles[i].c;

        Vector ap = projection - this->triangles[i].a;
        Vector bp = projection - this->triangles[i].b;
        Vector cp = projection - this->triangles[i].c;

        Vector abp = ab.cross(ap);
        Vector bcp = bc.cross(bp);
        Vector cap = ca.cross(cp);

        if (abp * bcp >= 0 && bcp * cap >= 0) {
            lastIntersectionTriangleIndex = i;
            return result;
        }
    }

    lastIntersectionTriangleIndex = -1;
    return 10000;
}

Vector Mesh::gradient(Vector v) {
    if (lastIntersectionTriangleIndex == -1)
        return Vector(0, 0, 0);

    return this->triangles[lastIntersectionTriangleIndex].normal;
}

Vector Mesh::normal(Vector v) {
    if (lastIntersectionTriangleIndex == -1)
        return Vector(0, 0, 0);

    return this->triangles[lastIntersectionTriangleIndex].normal;
}

float Mesh::u(Vector v) {
    return 0;
}

float Mesh::v(Vector v) {
    return 0;
}
