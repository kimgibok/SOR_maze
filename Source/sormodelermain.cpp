//
//  pra6.cpp
//  Computer_Graphics
//
//  Created by ���ؿ� on 12/6/23.
//

#include <list>
#include <iostream>
#include <cmath>
#include<new>
#include<stdio.h>
#include<vector>
#include<fstream>
#include<string>
#include<set>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <GL/glut.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ����ڰ� ���� ���� ������ �����ϴ� ���� ����
GLint Pointnum;

// ȸ����Ű�� ���� ����
GLfloat angle = 0.0f;

// �� ũ�⸦ �����ϴ� ���� ����
GLfloat pointSize;

// ������ ������ �����ϴ� ����
GLsizei winWidth = 1000, winHeight = 700;
GLint edgeLength = 10;


// ����ڰ� ���� ���� x, y��ǥ�� �����ϴ� ���� -> ���� ���������� �����쿡 �׷��ִ� �Լ��� Draw2Ddot()�� ����
struct Point {
    GLfloat x, y;
    Point(GLfloat _x, GLfloat _y) : x(_x), y(_y) {}
};

std::vector<Point> points;


// ����ڰ� ���� ��(���� �Է¹��� ��)�� ȸ���ϱ� ��, �ĸ� �����ϴ� Ŭ���� ����
class xPoint3D {
public:
    float x, y, z, w;

    // ����Ʈ ������
    xPoint3D() : x(0), y(0), z(0), w(1) {}

    // �Ű������� �޴� ������
    xPoint3D(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
};

// ����ڰ� �Է��� ���� ������ �� ����
std::vector<xPoint3D> arPoints;

// ����ڰ� �Է��� ���� �Է°����� ȸ���� ������ ������ �� ���� Mesh�� �������� �׸� �� ����
std::vector<xPoint3D> arRotPoints;

// ����ڰ� �Է��� ���� �Է°����� ȸ���� ������ ������ �� ���� arRotPoints�� ���� ������ �ݴ��̸� Mesh�� �������� �׸� �� ����
std::vector<xPoint3D> ParRotPoints;

// arRotPoints�� ������ �̿��� ���� ���͸� ���� �� ����
std::vector<glm::vec3> _nvertices;

// ������ ���� ���͸� ������
std::vector<glm::vec3> normals;



// ����ó�� - �Է°����� �Է� -> ȸ�� -> Mesh ��� -> Polygon ��� -> .obj�� ���� <= �� ������ ����� �ʰ� ���ִ� bool
bool B1;
bool B2;
bool B3;
bool B4;
bool B5;

// ����ó�� - �Է°����� ���� �ʾ����� true, �޾����� false�� �Է°����� �ι� �̻� ���� ���ϰ� ���� bool
bool isAngle = true;

// ����ó�� - �Է¹��� ���� ȸ�������� true, ���� ȸ������ �ʾ����� false ȸ���� �̹� ����Ǿ����� ���� ���̻� �Է� ���� ���ϰ� ���� bool
bool isRotate;

// ����ó�� -
bool isSave;




// �����쿡 ��µǴ� ���� ���� ��� �ٲٴ� �Լ�, ����ڰ� ������ �� ����
void SetPointColor(float r, float g, float b) {
    glColor3f(r, g, b);
}

// �����쿡 ��µǴ� ���� ũ�⸦ ���� �ٲٴ� ����, ����ڰ� ������ �� ����
void SetPointSize(float size) {
    glPointSize(size);
}

// �����쿡 xy����� ����� ���� x��, y���� �׷��ִ� �Լ�
void draw2DAxes() {
    glBegin(GL_LINES);

    // x��(������)
    SetPointColor(0.0, 0.0, 0.0);
    glVertex3f(-10.0, 0.0, 0.0);
    glVertex3f(10.0, 0.0, 0.0);

    // y��(�ʷϻ�)
    SetPointColor(0.0, 0.0, 0.0);
    glVertex3f(0.0, -10.0, 0.0);
    glVertex3f(0.0, 10.0, 0.0);

    glEnd();

}

// �����쿡 x��, y��, z���� �׷��ִ� �Լ�
void draw3DAxes() {
    glBegin(GL_LINES);

    // x�� (������)
    SetPointColor(1.0, 0.0, 0.0);
    glVertex3f(-10.0, 0.0, 0.0);
    glVertex3f(10.0, 0.0, 0.0);

    // y�� (�ʷϻ�)
    SetPointColor(0.0, 1.0, 0.0);
    glVertex3f(0.0, -10.0, 0.0);
    glVertex3f(0.0, 10.0, 0.0);

    // z�� (�Ķ���)
    SetPointColor(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, -10.0);
    glVertex3f(0.0, 0.0, 10.0);

    glEnd();
}

// ���콺 ��Ŭ������ �Է¹��� ���� �����쿡 ����ϴ� �Լ�
void Draw2Ddot() {
    glBegin(GL_POINTS);
    for (const Point& p : points) {
        SetPointColor(1.0f, 1.0f, 1.0f);
        glVertex2f(p.x, p.y);
    }
    glEnd();
}

// �Է°����� ȸ���� ������ �����쿡 ����ϴ� �Լ�
void Draw3Ddot() {
    glBegin(GL_POINTS);
    for (const xPoint3D& p : arRotPoints) {
        SetPointColor(1.0f, 1.0f, 1.0f);
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
}

// ī�޶��� ������ xy������� ����
void View2D() {
    glClear(GL_COLOR_BUFFER_BIT);

    // ����Ʈ ����
    glViewport(0, 0, winWidth, winHeight);

    // ���� ��� ����
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, 0.1, 100.0);

    // �� ��� ���� (ī�޶� ���� �� ����)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);

    // ���� ���� (ȸ��)
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
}

// ī�޶��� ������ (2, 2, 2)�� 3D�� ����
void View3D() {
    glClear(GL_COLOR_BUFFER_BIT);

    // ����Ʈ ����
    glViewport(0, 0, winWidth, winHeight);

    // ���� ��� ����
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)winWidth / (GLfloat)winHeight, 0.1, 100.0);

    // �� ��� ���� (ī�޶� ���� �� ����)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(2, 2, 2, 0, 0, 0, 0, 1, 0);

    // ���� ���� (ȸ��)
    glClearColor(0.25f, 0.25f, 0.25f, 0.0f);
}

// Mesh���·� �����쿡 ����ϴ� �Լ�
void DrawMesh() {
    glBegin(GL_LINES);

    // ȸ���� Ƚ���� �ǹ��ϴ� ����
    GLint newnum = 360 / angle;

    // �������� �׷��ִ� �Լ�
    for (int k = 0; k < newnum; k++) {
        for (int l = 0; l < Pointnum - 1; l++) {
            SetPointColor(1.0f, 1.0f, 1.0f);
            glVertex3f(arRotPoints[Pointnum * k + l].x, arRotPoints[Pointnum * k + l].y, arRotPoints[Pointnum * k + l].z);
            glVertex3f(arRotPoints[Pointnum * k + 1 + l].x, arRotPoints[Pointnum * k + 1 + l].y, arRotPoints[Pointnum * k + 1 + l].z);
        }
    }

    // �������� �׸� ���� ����ó���� ���� ����
    int _r = 1;

    // �������� �׷��ִ��Լ�
    for (int i = 0; i < ParRotPoints.size(); i++) {
        for (int k = 0; k < Pointnum; k++) {
            for (int l = 0; l < newnum - 1; l++) {
                SetPointColor(1.0f, 1.0f, 1.0f);
                glVertex3f(ParRotPoints[newnum * k + l].x, ParRotPoints[newnum * k + l].y, ParRotPoints[newnum * k + l].z);
                glVertex3f(ParRotPoints[newnum * k + 1 + l].x, ParRotPoints[newnum * k + 1 + l].y, ParRotPoints[newnum * k + 1 + l].z);

                if (i == (newnum * _r - 1) && i != 0) {
                    glVertex3f(ParRotPoints[newnum * _r - 1].x, ParRotPoints[newnum * _r - 1].y, ParRotPoints[newnum * _r - 1].z);
                    glVertex3f(ParRotPoints[newnum * (_r - 1)].x, ParRotPoints[newnum * (_r - 1)].y, ParRotPoints[newnum * (_r - 1)].z);
                    _r++;
                }
            }
        }
    }


    // "\"������ �밢���� �׸� ���� ����ó���� ���� ����
    int _p = 1;

    // "\"������ �밢���� �׷��ִ� �Լ�
    for (int i = 0; i < (Pointnum) * (newnum - 1); i++) {
        SetPointColor(1.0f, 1.0f, 1.0f);

        if (i != (Pointnum * _p) - 1) {
            glVertex3f(arRotPoints[i].x, arRotPoints[i].y, arRotPoints[i].z);
            glVertex3f(arRotPoints[i + Pointnum + 1].x, arRotPoints[i + Pointnum + 1].y, arRotPoints[i + Pointnum + 1].z);
        }

        else if (i == (Pointnum * _p) - 1) {
            _p++;
        }

    }

    // "/"������ �밢���� �׷��ִ� �Լ�
    _p = 1;
    for (int i = 0; i < (Pointnum) * (newnum - 1); i++) {
        SetPointColor(1.0f, 1.0f, 1.0f);

        if (i != (Pointnum * _p) - 1) {
            glVertex3f(arRotPoints[i + 1].x, arRotPoints[i + 1].y, arRotPoints[i + 1].z);
            glVertex3f(arRotPoints[i + Pointnum].x, arRotPoints[i + Pointnum].y, arRotPoints[i + Pointnum].z);
        }

        else if (i == (Pointnum * _p) - 1) {
            _p++;
        }

    }

    // "\"������ �밢���� �׷��� �� ������ �밢������ �׷��ִ� �Լ�
    int _q = 0;

    for (int i = (Pointnum) * (newnum - 1); i < (Pointnum) * (newnum)-1; i++) {
        glVertex3f(arRotPoints[i].x, arRotPoints[i].y, arRotPoints[i].z);
        glVertex3f(arRotPoints[_q + 1].x, arRotPoints[_q + 1].y, arRotPoints[_q + 1].z);

        _q++;
    }

    // "/"������ �밢���� �׷��� �� ������ �밢������ �׷��ִ� �Լ�
    _q = 0;

    for (int i = (Pointnum) * (newnum - 1); i < (Pointnum) * (newnum)-1; i++) {
        glVertex3f(arRotPoints[i + 1].x, arRotPoints[i + 1].y, arRotPoints[i + 1].z);
        glVertex3f(arRotPoints[_q].x, arRotPoints[_q].y, arRotPoints[_q].z);

        _q++;
    }

    glEnd();
}

// Polygon���·� �����쿡 ����ϴ� �Լ�
void DrawPolygon() {
    // �ε����� ǥ���ϴ� ������
    int i = 0;
    int p = Pointnum;
    int n = 0;

    // Polygon�� �׸� ���� ����ó���� ���� ����
    int _p = 1;

    // ����, �Ʒ����� ������ ��� ���� �׸���
    for (int j = 0; j < (Pointnum) * (360 / angle - 1); j++) {
        // �׸��� �ȵǴ� ���� ����
        if (i != (Pointnum * _p) - 1) {
            SetPointColor(1.0f, 1.0f, 1.0f);
            glBegin(GL_POLYGON);
            glVertex3f(arRotPoints[i].x, arRotPoints[i].y, arRotPoints[i].z);
            i++;
            glVertex3f(arRotPoints[i].x, arRotPoints[i].y, arRotPoints[i].z);
            p++;
            glVertex3f(arRotPoints[p].x, arRotPoints[p].y, arRotPoints[p].z);
            p--;
            glVertex3f(arRotPoints[p].x, arRotPoints[p].y, arRotPoints[p].z);
            i--;
            i++;
            p++;
            glEnd();
        }
        else {
            _p++;
            i++;
            p++;
        }
        n++;
    }

    // ������ �� �׸��� �ε��� �ʱ�ȭ
    i = Pointnum * ((360 / angle) - 1);
    p = 0;

    // ������ �� �׸���
    for (int a = 0; a < Pointnum - 1; a++) {
        glBegin(GL_POLYGON);
        glVertex3f(arRotPoints[i].x, arRotPoints[i].y, arRotPoints[i].z);
        i++;
        glVertex3f(arRotPoints[i].x, arRotPoints[i].y, arRotPoints[i].z);
        p++;
        glVertex3f(arRotPoints[p].x, arRotPoints[p].y, arRotPoints[p].z);
        p--;
        glVertex3f(arRotPoints[p].x, arRotPoints[p].y, arRotPoints[p].z);
        i--;
        i++;
        p++;
        glEnd();
    }

    // ���� �׸��� �ε��� �ʱ�ȭ
    i = 0;
    // ���� �׸���
    glBegin(GL_POLYGON);
    for (int k = 0; k < (360 / angle); k++) {
        glVertex3f(arRotPoints[i].x, arRotPoints[i].y, arRotPoints[i].z);
        i += Pointnum;
    }
    glEnd();

    // �Ʒ��� �׸��� �ε��� �ʱ�ȭ
    i = Pointnum * (360 / angle) - 1;
    // �Ʒ��� �׸���
    glBegin(GL_POLYGON);
    for (int k = 0; k < (360 / angle); k++) {
        glVertex3f(arRotPoints[i].x, arRotPoints[i].y, arRotPoints[i].z);
        i -= Pointnum;
    }
    glEnd();
}

// �Է°����� ȸ���� ������ Mesh���·� ǥ����, �̸� xy��鿡�� �ٶ󺸴� �Լ�
void MeshDisplay() {
    // ī�޶� �� ����
    View2D();

    SetPointColor(1.0f, 1.0f, 1.0f);

    // �� �� �� �׸���
    draw2DAxes();
    SetPointSize(2.0f);
    DrawMesh();

    glutSwapBuffers();
}


// �Է°����� ȸ���� ������ Mesh���·� ǥ����, �̸� 3�������� �ٶ󺸴� �Լ�
void MeshDis3D() {
    // ī�޶� �� ����
    View3D();

    SetPointColor(1.0f, 1.0f, 1.0f);

    // �� �� �� �׸���
    draw3DAxes();
    SetPointSize(2.0f);
    DrawMesh();

    glutSwapBuffers();

}

// �Է°����� ȸ���� ������ Polygon���·� ǥ����, �̸� xy��鿡�� �ٶ󺸴� �Լ�
void PolygonDisplay() {
    // ī�޶� �� ����
    View2D();

    SetPointColor(1.0f, 1.0f, 1.0f);

    // �� �� �� �׸���
    draw2DAxes();
    SetPointSize(2.0f);

    DrawPolygon();

    glutSwapBuffers();

}


// �Է°����� ȸ���� ������ Polygon���·� ǥ����, �̸� 3�������� �ٶ󺸴� �Լ�
void PolDis3D() {
    // ī�޶� �� ����
    View3D();

    SetPointColor(1.0f, 1.0f, 1.0f);

    // �� �� �� �׸���
    draw3DAxes();
    SetPointSize(2.0f);

    DrawPolygon();

    glutSwapBuffers();

}

// ���� ���͸� ���� ���� ���ϱ� ���� _nvertices�� ���� ����, arRotPoints�� �ι� ����
void saveNewThing() {

    GLint newnum = 360 / angle;

    _nvertices.clear();

    for (int k = 0; k < newnum; k++) {
        for (int l = 0; l < Pointnum; l++) {

            if (k != newnum - 1) {
                _nvertices.push_back(glm::vec3(arRotPoints[Pointnum * k + l].x, arRotPoints[Pointnum * k + l].y, arRotPoints[Pointnum * k + l].z));

                _nvertices.push_back(glm::vec3(arRotPoints[Pointnum * (k + 1) + l].x, arRotPoints[Pointnum * (k + 1) + l].y, arRotPoints[Pointnum * (k + 1) + l].z));
            }

            else if (k == newnum - 1) {
                _nvertices.push_back(glm::vec3(arRotPoints[Pointnum * k + l].x, arRotPoints[Pointnum * k + l].y, arRotPoints[Pointnum * k + l].z));

                _nvertices.push_back(glm::vec3(arRotPoints[l].x, arRotPoints[l].y, arRotPoints[l].z));
            }
        }
    }
}

// ���� ���͸� ���ϴ� �Լ�
void CNT(const std::vector<glm::vec3>& nvertices, std::vector<glm::vec3>& normals) {

    normals.clear();

    for (int i = 0; i < nvertices.size(); i++) {
        // Calculate normal vector for each face
        glm::vec3 _normals = glm::normalize(glm::cross(nvertices[i + 2] - nvertices[i], nvertices[i + 1] - nvertices[i]));
        normals.push_back(_normals);
    }

    // ���� ��������
    glm::vec3 _normals = glm::vec3(0, 1, 0);
    normals.push_back(_normals);

    // �Ʒ��� ��������
    _normals = glm::vec3(0, -1, 0);
    normals.push_back(_normals);
}


// �������� .obj�������� �����ϴ� �Լ� - ���� �ﰢ������ ����
void SaveObjFileT(const std::vector<xPoint3D>& vertices, const std::string& outputFilename) {

    saveNewThing();

    // �Է¹��� ���� �̸��� ����Ͽ� ������ ������
    std::ofstream objFile(outputFilename);

    CNT(_nvertices, normals);

    // ������ ���ȴ��� Ȯ��
    if (!objFile.is_open()) {
        std::cerr << "������ �� �� �����ϴ�." << std::endl;
        return;
    }
    else
        std::cout << "���� �����մϴ�...! ���� �� ������ ȸ�� ������ ���� �����ɸ� �� �ֽ��ϴ�..." << std::endl;


    // .obj ���� ���Ŀ� ���� �� ������ ��ǥ�� ����
    for (const xPoint3D& vertex : vertices) {
        objFile << "v " << vertex.x << " " << vertex.y << " " << vertex.z << "\n";
    }

    objFile << "\n";

    // ���� ���� ������ ����
    for (int i = 0; i < normals.size(); i++) {
        objFile << "vn " << normals[i].x << " " << normals[i].y << " " << normals[i].z << "\n";
    }

    objFile << "\n";

    // �� ������ ���Ͽ� ����

    // �ε����� 1���� ����
    int i = 1;
    int l = 1;
    int p = Pointnum + 1;
    int q = Pointnum + 1;

    int n = 0;

    // �ﰢ������ ����
    for (int j = 0; j < (Pointnum) * (360 / angle - 1); j++) {
        int _p = 1;

        if (i % (Pointnum * _p) != 0) {
            objFile << "f ";

            objFile << i << "//" << l << " ";

            i++;

            objFile << i << "//" << l << " ";

            objFile << p << "//" << l << " ";

            objFile << "\n";
            objFile << "f ";

            objFile << i << "//" << l << " ";

            p++;
            q++;

            objFile << p << "//" << l << " ";

            p--;
            q--;

            objFile << p << "//" << l << " ";

            objFile << "\n";

            i--;

            i++;
            l++;
            l++;
            p++;
            q++;
        }

        else {
            _p++;
            i++;
            l++;
            l++;
            p++;
            q++;
        }
        n++;
    }

    // ������ �� ���� �ε��� �ʱ�ȭ
    i = (Pointnum) * ((360 / angle) - 1) + 1;


    p = 1;
    q = 1;

    // ������ �� ����
    for (int a = 0; a < Pointnum - 1; a++) {
        objFile << "f ";

        objFile << i << "//" << l << " ";

        i++;

        objFile << i << "//" << l << " ";

        objFile << p << "//" << l << " ";

        objFile << "\n";
        objFile << "f ";

        objFile << i << "//" << l << " ";

        p++;
        q++;

        objFile << p << "//" << l << " ";

        p--;
        q--;

        objFile << p << "//" << l << " ";

        objFile << "\n";

        i--;

        i++;
        l++;
        l++;
        p++;
        q++;
    }

    // ����, �Ʒ��� ���� �ε��� �ʱ�ȭ
    int _i = 1;
    i = 1;
    l = Pointnum * (360 / angle) * 2 + 1;

    // ���� ����
    for (int k = 2; k < (360 / angle); k++) {
        objFile << "f " << _i << "//" << l << " ";

        for (int t = 0; t < 2; t++) {
            i += Pointnum;
            objFile << i << "//" << l << " ";
        }
        i -= Pointnum;
        objFile << "\n";
    }

    // �Ʒ��� ����
    _i = Pointnum * (360 / angle);
    i = Pointnum * (360 / angle);
    l = Pointnum * (360 / angle) * 2 + 2;
    for (int k = 2; k < (360 / angle); k++) {
        objFile << "f " << _i << "//" << l << " ";

        for (int t = 0; t < 2; t++) {
            i -= Pointnum;
            objFile << i << "//" << l << " ";
        }
        i += Pointnum;
        objFile << "\n";
    }

    std::cout << "������ �ݽ��ϴ�." << std::endl;

    objFile.close();

    std::cout << "���� " << outputFilename << "�� ���������� ����Ǿ����ϴ�...!" << std::endl;
}


// �������� .obj�������� �����ϴ� �Լ� - ���� �ﰢ���� �ƴ� �ٰ������� ����
void SaveObjFileS(const std::vector<xPoint3D>& vertices, const std::string& outputFilename) {

    saveNewThing();

    // �Է¹��� ���� �̸��� ����Ͽ� ���� ����
    std::ofstream objFile(outputFilename);

    CNT(_nvertices, normals);

    // ������ ���ȴ��� Ȯ��
    if (!objFile.is_open()) {
        std::cerr << "������ �� �� �����ϴ�." << std::endl;
        return;
    }
    else
        std::cout << "���� �����մϴ�...! ���� �� ������ ȸ�� ������ ���� �����ɸ� �� �ֽ��ϴ�..." << std::endl;


    // .obj ���� ���Ŀ� ���� �� ������ ��ǥ�� ���Ͽ� ����
    for (const xPoint3D& vertex : vertices) {
        objFile << "v " << vertex.x << " " << vertex.y << " " << vertex.z << "\n";
    }

    objFile << "\n";

    // ���� ���� ������ ���Ͽ� ����
    for (int i = 0; i < normals.size(); i++) {
        objFile << "vn " << normals[i].x << " " << normals[i].y << " " << normals[i].z << "\n";
    }

    objFile << "\n";

    // �� ������ ���Ͽ� ����

    // �ε����� 1���� ����
    int i = 1;
    int l = 2;
    int p = Pointnum + 1;
    int q = Pointnum + 1;

    int n = 0;

    // �ٰ������� ����
    for (int j = 0; j < (Pointnum) * (360 / angle - 1); j++) {
        int _p = 1;

        if (i % (Pointnum * _p) != 0) {
            objFile << "f ";

            objFile << i << "//" << l << " ";  // 1 2 8 7 / 18 19 23 22 / 19 20 24 23

            i++;

            objFile << i << "//" << l << " ";

            p++;
            q++;

            objFile << p << "//" << l << " ";

            p--;
            q--;

            objFile << p << "//" << l << " ";

            objFile << "\n";

            i--;

            i++;
            l++;
            l++;
            p++;
            q++;
        }

        else {
            _p++;
            i++;
            l++;
            l++;
            p++;
            q++;
        }
        n++;
    }

    // ������ �� ���� �ε��� �ʱ�ȭ
    i = (Pointnum) * ((360 / angle) - 1) + 1;
    p = 1;
    q = 1;

    // ������ �� ����
    // �ٰ������� ����
    for (int a = 0; a < Pointnum - 1; a++) {
        objFile << "f ";

        objFile << i << "//" << l << " ";

        i++;

        objFile << i << "//" << l << " ";

        p++;
        q++;

        objFile << p << "//" << l << " ";

        p--;
        q--;

        objFile << p << "//" << l << " ";

        objFile << "\n";

        i--;

        i++;
        l++;
        l++;
        p++;
        q++;
    }

    // ����, �Ʒ��� ���� �ε��� �ʱ�ȭ
    i = 1;
    l = Pointnum * (360 / angle) * 2 + 1;

    // ���� ����
    objFile << "f ";
    for (int k = 0; k < (360 / angle); k++) {
        objFile << i << "//" << l << " ";
        i += Pointnum;

    }
    objFile << "\n";

    // �Ʒ��� ����
    i = Pointnum * (360 / angle);
    l = Pointnum * (360 / angle) * 2 + 2;
    objFile << "f ";
    for (int k = 0; k < (360 / angle); k++) {
        objFile << i << "//" << l << " ";
        i -= Pointnum;

    }
    objFile << "\n";

    std::cout << "������ �ݽ��ϴ�." << std::endl;
    // ������ �ݽ��ϴ�.
    objFile.close();

    std::cout << "���� " << outputFilename << "�� ���������� ����Ǿ����ϴ�...!" << std::endl;
}

void MyDisplay() {
    View2D();

    SetPointColor(1.0f, 1.0f, 1.0f);

    // �� �� �� �׸���
    draw2DAxes();
    SetPointSize(2.0f);
    Draw2Ddot();

    // �ֱٿ� ���� ���� ��ǥ ���
    if (!points.empty()) {
        const Point& recentPoint = points.back();
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(-0.9f, 0.9f);

        std::string recentPointStr = "(" + std::to_string(recentPoint.x) + ", " + std::to_string(recentPoint.y) + ")";
        const char* recentPointCStr = recentPointStr.c_str();

        // ���ڿ� ���
        while (*recentPointCStr) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *recentPointCStr);
            recentPointCStr++;
        }
    }

    // ���� ���
    glRasterPos2f(-0.9f, 0.85f);
    std::string pointCount = "Point Count: " + std::to_string(points.size());
    const char* countStr = pointCount.c_str();
    while (*countStr) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *countStr);
        countStr++;
    }

    glutSwapBuffers();
}

// �Է°����� ȸ���� ������ ����ϰ�, �̸� xy��鿡�� �ٶ󺸴� �Լ�
void My2Display() {
    View2D();

    SetPointColor(1.0f, 1.0f, 1.0f);

    // �� �� �� �׸���
    draw2DAxes();
    SetPointSize(2.0f);
    Draw3Ddot();

    glutSwapBuffers();
}

void My3Display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // ���� ���� (ȸ��)
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    SetPointColor(1.0f, 1.0f, 1.0f);

    // �� �� �� �׸���
    draw3DAxes();
    SetPointSize(2.0f);
    Draw3Ddot();

    glutSwapBuffers();
}

// ���콺 ��Ŭ���� ���� �����쿡 ���� ��� + �� ���� ��ǥ�� �����ϴ� �Լ�
void MouseClick(GLint button, GLint state, GLint x, GLint y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && isRotate == false) {
        if (B1 == false)
            B1 = true;

        GLfloat pointX = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f;
        GLfloat pointY = 1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT);
        if (pointX != 0.0000000) {
            points.emplace_back(pointX, pointY);
            arPoints.push_back(xPoint3D{ pointX, pointY, 0, 1 });

            Pointnum++;

            glutPostRedisplay();
        }
    }
}

// ������ ������ �Է°����� ȸ���ϰ� �����ϴµ� �ʿ��� ��İ�� �Լ�
void rotatePoint() {
    xPoint3D newpt;
    for (int l = 0; l < (360 / angle); l++) {
        for (int i = 0; i < Pointnum; i++) {
            float fRotAngle = angle;
            float radian;

            fRotAngle = fRotAngle * l; // ȸ�� ������ ���� ȸ�� Ƚ���� ����
            radian = fRotAngle * (M_PI / 180.0f);

            xPoint3D newpt;
            newpt.x = arPoints[i].z * sin(radian) + arPoints[i].x * cos(radian);
            newpt.y = arPoints[i].y;
            newpt.z = arPoints[i].z * cos(radian) - arPoints[i].x * sin(radian);
            arRotPoints.push_back(xPoint3D{ newpt.x, newpt.y, newpt.z, 1 });

            glutPostRedisplay();

        }
    }
    for (int i = 0; i < Pointnum; i++) {
        for (int l = 0; l < (360 / angle); l++) {
            float fRotAngle = angle;
            float radian;

            fRotAngle = fRotAngle * (l + 1);
            radian = fRotAngle * (M_PI / 180.);

            newpt.x = arPoints[i].z * sin(radian) + arPoints[i].x * cos(radian);
            newpt.y = arPoints[i].y;
            newpt.z = arPoints[i].z * cos(radian) - arPoints[i].x * sin(radian);
            ParRotPoints.push_back(xPoint3D{ newpt.x, newpt.y, newpt.z, 1 });

            glutPostRedisplay();
        }
    }
}





void menu(int value) {
    switch (value) {
    case 1:
        isRotate = true;
        if (B1 == true && isAngle == true) {
            do {
                std::cout << "ȸ�� ������ �Է��ϼ��� (360�� ���): ";
                std::cin >> angle;

                // �Է� ���� ������ 360�� ������� Ȯ��
                if (fmod(360.0f, angle) == 0) {
                    std::cout << "�Է� �޾ҽ��ϴ�...!" << std::endl;
                    B2 = true;
                    break;
                }

                else {
                    std::cout << "360�� ����� �ƴմϴ�. �ٽ� �Է��ϼ���." << std::endl;
                }
            } while (true);

            // ������ [0, 360) ���� ���� �ֵ��� ����
            angle = fmod(angle, 360.0f);
            rotatePoint();

            glutPostRedisplay();

            isAngle = false;

            break;
        }
        else if (B1 != true) {
            std::cout << "!!!���� ���μ����� �Ϸ����� �ʾҽ��ϴ�!!!" << std::endl;
            break;
        }
        else if (isAngle != true) {
            std::cout << "!!!�̹� ������ �Է��߽��ϴ�!!!" << std::endl;
            break;
        }

    case 2:
        if (B2 == true) {
            std::cout << "ȸ���� ���� �׸��ϴ�...!" << std::endl;
            glutDisplayFunc(My2Display);
            glutPostRedisplay();
            B3 = true;
            break;
        }
        else {
            std::cout << "!!!���� ���μ����� �Ϸ����� �ʾҽ��ϴ�!!!" << std::endl;
            break;
        }

    case 3:
        if (B3 == true) {
            std::cout << "ȸ���� ���� ������ �׸��ϴ�...!" << std::endl;
            glutDisplayFunc(MeshDisplay);
            glutPostRedisplay();
            B4 = true;
            break;
        }
        else {
            std::cout << "!!!���� ���μ����� �Ϸ����� �ʾҽ��ϴ�!!!" << std::endl;
            break;
        }

    case 4:
        if (B3 == true) {
            std::cout << "Mesh Model�� 3D��� ����մϴ�...!" << std::endl;
            glutDisplayFunc(MeshDis3D);
            glutPostRedisplay();
            B4 = true;
            break;
        }
        else {
            std::cout << "!!!���� ���μ����� �Ϸ����� �ʾҽ��ϴ�!!!" << std::endl;
            break;
        }

    case 5:
        if (B4 == true) {
            std::cout << "Mesh�� Polygon ���·� ����մϴ�...!" << std::endl;
            glutDisplayFunc(PolygonDisplay);
            glutPostRedisplay();
            B5 = true;
            break;
        }
        else {
            std::cout << "!!!���� ���μ����� �Ϸ����� �ʾҽ��ϴ�!!!" << std::endl;
            break;
        }

    case 6:
        if (B4 == true) {
            std::cout << "Polygon Model�� 3D��� ����մϴ�...!" << std::endl;
            glutDisplayFunc(PolDis3D);
            glutPostRedisplay();
            B5 = true;
            break;
        }
        else {
            std::cout << "!!!���� ���μ����� �Ϸ����� �ʾҽ��ϴ�!!!" << std::endl;
            break;
        }

    case 7:
        if (B5 == true && isSave == false) {
            isSave = true;
            if (arRotPoints.size() > 0) {
                std::string filename;
                std::cout << "Wavefront .obj ���Ϸ� ������ ���� �̸��� �Է� �ϼ���: ";
                std::cin >> filename;

                filename = "build/obj/" + filename;
                std::cout << filename << std::endl;
                // ���� Wavefront .obj ���Ϸ� �����մϴ�.
                SaveObjFileT(arRotPoints, filename);

                isSave = false;
                filename = "";
            }
            break;
        }
        else {
            std::cout << "!!!���� ���μ����� �Ϸ����� �ʾҽ��ϴ�!!!" << std::endl;
            break;
        }
    case 8:
        if (B5 == true && isSave == false) {
            isSave = true;
            if (arRotPoints.size() > 0) {
                std::string filename;
                std::cout << "Wavefront .obj ���Ϸ� ������ ���� �̸��� �Է� �ϼ���: ";
                std::cin >> filename;

                filename = "build/obj/" + filename;

                // ���� Wavefront .obj ���Ϸ� �����մϴ�.
                SaveObjFileS(arRotPoints, filename);

                isSave = false;
                filename = "";
            }
            break;
        }
        else {
            std::cout << "!!!���� ���μ����� �Ϸ����� �ʾҽ��ϴ�!!!" << std::endl;
            break;
        }

    case 9:
        std::cout << "���� �ʱ�ȭ ��..." << std::endl;
        arPoints.clear();
        arRotPoints.clear();
        ParRotPoints.clear();
        points.clear();
        _nvertices.clear();
        normals.clear();
        Pointnum = 0;
        angle = 0;

        B1 = false;
        B2 = false;
        B3 = false;
        B4 = false;
        B5 = false;

        isAngle = true;
        isRotate = false;
        isSave = false;
        glutDisplayFunc(MyDisplay);
        glutPostRedisplay();
        std::cout << "����...!" << std::endl;
        break;

    case 0:
        std::cout << "���α׷��� �����մϴ�." << std::endl;
        exit(0);
        break;

    }
}





int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("SOR MODELLER");

    glutDisplayFunc(MyDisplay);
    glutMouseFunc(MouseClick);

    glutCreateMenu(menu);
    glutAddMenuEntry("Get Angle", 1);
    glutAddMenuEntry("Rotate", 2);
    glutAddMenuEntry("Draw Mesh", 3);
    glutAddMenuEntry("Draw 3D Mesh", 4);
    glutAddMenuEntry("Draw Polygon", 5);
    glutAddMenuEntry("Draw 3D Polygon", 6);
    glutAddMenuEntry("Save to Triangle Method", 7);
    glutAddMenuEntry("Save to Square Method", 8);
    glutAddMenuEntry("Clear", 9);
    glutAddMenuEntry("Exit", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();

    return 0;
}

