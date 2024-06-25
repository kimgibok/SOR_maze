//
//  pra6.cpp
//  Computer_Graphics
//
//  Created by 박준영 on 12/6/23.
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

// 사용자가 찍은 점의 개수를 저장하는 변수 선언
GLint Pointnum;

// 회전시키는 각도 선언
GLfloat angle = 0.0f;

// 점 크기를 설정하는 변수 선언
GLfloat pointSize;

// 윈도우 사이즈 설정하는 변수
GLsizei winWidth = 1000, winHeight = 700;
GLint edgeLength = 10;


// 사용자가 찍은 점의 x, y좌표를 저장하는 변수 -> 점을 찍을때마다 윈도우에 그려주는 함수인 Draw2Ddot()에 쓰임
struct Point {
    GLfloat x, y;
    Point(GLfloat _x, GLfloat _y) : x(_x), y(_y) {}
};

std::vector<Point> points;


// 사용자가 찍은 점(이하 입력받은 점)을 회전하기 전, 후를 저장하는 클래스 선언
class xPoint3D {
public:
    float x, y, z, w;

    // 디폴트 생성자
    xPoint3D() : x(0), y(0), z(0), w(1) {}

    // 매개변수를 받는 생성자
    xPoint3D(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
};

// 사용자가 입력한 점을 저장할 때 쓰임
std::vector<xPoint3D> arPoints;

// 사용자가 입력한 점을 입력각도로 회전한 점들을 저장할 때 쓰임 Mesh의 세로줄을 그릴 때 쓰임
std::vector<xPoint3D> arRotPoints;

// 사용자가 입력한 점을 입력각도로 회전한 점들을 저장할 때 쓰임 arRotPoints와 저장 순서가 반대이며 Mesh의 가로줄을 그릴 때 쓰임
std::vector<xPoint3D> ParRotPoints;

// arRotPoints의 정보를 이용해 법선 벡터를 구할 때 쓰임
std::vector<glm::vec3> _nvertices;

// 점들의 법선 벡터를 저장함
std::vector<glm::vec3> normals;



// 예외처리 - 입력각도를 입력 -> 회전 -> Mesh 출력 -> Polygon 출력 -> .obj로 저장 <= 이 순서를 어기지 않게 해주는 bool
bool B1;
bool B2;
bool B3;
bool B4;
bool B5;

// 예외처리 - 입력각도를 받지 않았으면 true, 받았으면 false로 입력각도를 두번 이상 받지 못하게 막는 bool
bool isAngle = true;

// 예외처리 - 입력받은 점을 회전했으면 true, 아직 회전하지 않았으면 false 회전이 이미 진행되었으면 점을 더이상 입력 받지 못하게 막는 bool
bool isRotate;

// 예외처리 -
bool isSave;




// 윈도우에 출력되는 점의 색을 쇱게 바꾸는 함수, 사용자가 지정할 수 없음
void SetPointColor(float r, float g, float b) {
    glColor3f(r, g, b);
}

// 윈도우에 출력되는 점의 크기를 쉽게 바꾸는 변수, 사용자가 지정할 수 없음
void SetPointSize(float size) {
    glPointSize(size);
}

// 윈도우에 xy평면을 출력할 때의 x축, y축을 그려주는 함수
void draw2DAxes() {
    glBegin(GL_LINES);

    // x축(빨간색)
    SetPointColor(0.0, 0.0, 0.0);
    glVertex3f(-10.0, 0.0, 0.0);
    glVertex3f(10.0, 0.0, 0.0);

    // y축(초록색)
    SetPointColor(0.0, 0.0, 0.0);
    glVertex3f(0.0, -10.0, 0.0);
    glVertex3f(0.0, 10.0, 0.0);

    glEnd();

}

// 윈도우에 x축, y축, z축을 그려주는 함수
void draw3DAxes() {
    glBegin(GL_LINES);

    // x축 (빨간색)
    SetPointColor(1.0, 0.0, 0.0);
    glVertex3f(-10.0, 0.0, 0.0);
    glVertex3f(10.0, 0.0, 0.0);

    // y축 (초록색)
    SetPointColor(0.0, 1.0, 0.0);
    glVertex3f(0.0, -10.0, 0.0);
    glVertex3f(0.0, 10.0, 0.0);

    // z축 (파란색)
    SetPointColor(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, -10.0);
    glVertex3f(0.0, 0.0, 10.0);

    glEnd();
}

// 마우스 좌클릭으로 입력받은 점을 윈도우에 출력하는 함수
void Draw2Ddot() {
    glBegin(GL_POINTS);
    for (const Point& p : points) {
        SetPointColor(1.0f, 1.0f, 1.0f);
        glVertex2f(p.x, p.y);
    }
    glEnd();
}

// 입력각도로 회전한 점들을 윈도우에 출력하는 함수
void Draw3Ddot() {
    glBegin(GL_POINTS);
    for (const xPoint3D& p : arRotPoints) {
        SetPointColor(1.0f, 1.0f, 1.0f);
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
}

// 카메라의 시점을 xy평면으로 설정
void View2D() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 뷰포트 설정
    glViewport(0, 0, winWidth, winHeight);

    // 투영 행렬 설정
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, 0.1, 100.0);

    // 뷰 행렬 설정 (카메라 시점 및 방향)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);

    // 배경색 설정 (회색)
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
}

// 카메라의 시점을 (2, 2, 2)의 3D로 설정
void View3D() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 뷰포트 설정
    glViewport(0, 0, winWidth, winHeight);

    // 투영 행렬 설정
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)winWidth / (GLfloat)winHeight, 0.1, 100.0);

    // 뷰 행렬 설정 (카메라 시점 및 방향)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(2, 2, 2, 0, 0, 0, 0, 1, 0);

    // 배경색 설정 (회색)
    glClearColor(0.25f, 0.25f, 0.25f, 0.0f);
}

// Mesh형태로 윈도우에 출력하는 함수
void DrawMesh() {
    glBegin(GL_LINES);

    // 회전한 횟수를 의미하는 변수
    GLint newnum = 360 / angle;

    // 세로줄을 그려주는 함수
    for (int k = 0; k < newnum; k++) {
        for (int l = 0; l < Pointnum - 1; l++) {
            SetPointColor(1.0f, 1.0f, 1.0f);
            glVertex3f(arRotPoints[Pointnum * k + l].x, arRotPoints[Pointnum * k + l].y, arRotPoints[Pointnum * k + l].z);
            glVertex3f(arRotPoints[Pointnum * k + 1 + l].x, arRotPoints[Pointnum * k + 1 + l].y, arRotPoints[Pointnum * k + 1 + l].z);
        }
    }

    // 가로줄을 그릴 때의 예외처리를 위한 변수
    int _r = 1;

    // 가로줄을 그려주는함수
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


    // "\"방향의 대각선을 그릴 때의 예외처리를 위한 변수
    int _p = 1;

    // "\"방향의 대각선을 그려주는 함수
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

    // "/"방형의 대각선을 그려주는 함수
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

    // "\"방향의 대각선을 그려줄 때 마지막 대각선들을 그려주는 함수
    int _q = 0;

    for (int i = (Pointnum) * (newnum - 1); i < (Pointnum) * (newnum)-1; i++) {
        glVertex3f(arRotPoints[i].x, arRotPoints[i].y, arRotPoints[i].z);
        glVertex3f(arRotPoints[_q + 1].x, arRotPoints[_q + 1].y, arRotPoints[_q + 1].z);

        _q++;
    }

    // "/"방향의 대각선을 그려줄 때 마지막 대각선들을 그려주는 함수
    _q = 0;

    for (int i = (Pointnum) * (newnum - 1); i < (Pointnum) * (newnum)-1; i++) {
        glVertex3f(arRotPoints[i + 1].x, arRotPoints[i + 1].y, arRotPoints[i + 1].z);
        glVertex3f(arRotPoints[_q].x, arRotPoints[_q].y, arRotPoints[_q].z);

        _q++;
    }

    glEnd();
}

// Polygon형태로 윈도우에 출력하는 함수
void DrawPolygon() {
    // 인덱스를 표현하는 변수들
    int i = 0;
    int p = Pointnum;
    int n = 0;

    // Polygon을 그릴 때의 예외처리를 위한 변수
    int _p = 1;

    // 윗면, 아랫면을 제외한 모든 면을 그리기
    for (int j = 0; j < (Pointnum) * (360 / angle - 1); j++) {
        // 그리면 안되는 면을 제외
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

    // 마지막 면 그리기 인덱스 초기화
    i = Pointnum * ((360 / angle) - 1);
    p = 0;

    // 마지막 면 그리기
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

    // 윗면 그리기 인덱스 초기화
    i = 0;
    // 윗면 그리기
    glBegin(GL_POLYGON);
    for (int k = 0; k < (360 / angle); k++) {
        glVertex3f(arRotPoints[i].x, arRotPoints[i].y, arRotPoints[i].z);
        i += Pointnum;
    }
    glEnd();

    // 아랫면 그리기 인덱스 초기화
    i = Pointnum * (360 / angle) - 1;
    // 아랫면 그리기
    glBegin(GL_POLYGON);
    for (int k = 0; k < (360 / angle); k++) {
        glVertex3f(arRotPoints[i].x, arRotPoints[i].y, arRotPoints[i].z);
        i -= Pointnum;
    }
    glEnd();
}

// 입력각도로 회전한 점들을 Mesh형태로 표현고, 이를 xy평면에서 바라보는 함수
void MeshDisplay() {
    // 카메라 뷰 설정
    View2D();

    SetPointColor(1.0f, 1.0f, 1.0f);

    // 선 및 점 그리기
    draw2DAxes();
    SetPointSize(2.0f);
    DrawMesh();

    glutSwapBuffers();
}


// 입력각도로 회전한 점들을 Mesh형태로 표현고, 이를 3차원에서 바라보는 함수
void MeshDis3D() {
    // 카메라 뷰 설정
    View3D();

    SetPointColor(1.0f, 1.0f, 1.0f);

    // 선 및 점 그리기
    draw3DAxes();
    SetPointSize(2.0f);
    DrawMesh();

    glutSwapBuffers();

}

// 입력각도로 회전한 점들을 Polygon형태로 표현고, 이를 xy평면에서 바라보는 함수
void PolygonDisplay() {
    // 카메라 뷰 설정
    View2D();

    SetPointColor(1.0f, 1.0f, 1.0f);

    // 선 및 점 그리기
    draw2DAxes();
    SetPointSize(2.0f);

    DrawPolygon();

    glutSwapBuffers();

}


// 입력각도로 회전한 점들을 Polygon형태로 표현고, 이를 3차원에서 바라보는 함수
void PolDis3D() {
    // 카메라 뷰 설정
    View3D();

    SetPointColor(1.0f, 1.0f, 1.0f);

    // 선 및 점 그리기
    draw3DAxes();
    SetPointSize(2.0f);

    DrawPolygon();

    glutSwapBuffers();

}

// 법선 벡터를 보다 쉽게 구하기 위해 _nvertices에 점을 정리, arRotPoints의 두배 길이
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

// 법선 벡터를 구하는 함수
void CNT(const std::vector<glm::vec3>& nvertices, std::vector<glm::vec3>& normals) {

    normals.clear();

    for (int i = 0; i < nvertices.size(); i++) {
        // Calculate normal vector for each face
        glm::vec3 _normals = glm::normalize(glm::cross(nvertices[i + 2] - nvertices[i], nvertices[i + 1] - nvertices[i]));
        normals.push_back(_normals);
    }

    // 윗면 법선벡터
    glm::vec3 _normals = glm::vec3(0, 1, 0);
    normals.push_back(_normals);

    // 아랫면 법선벡터
    _normals = glm::vec3(0, -1, 0);
    normals.push_back(_normals);
}


// 정보들을 .obj형식으로 저장하는 함수 - 면을 삼각형으로 저장
void SaveObjFileT(const std::vector<xPoint3D>& vertices, const std::string& outputFilename) {

    saveNewThing();

    // 입력받은 파일 이름을 사용하여 파일을 열어줌
    std::ofstream objFile(outputFilename);

    CNT(_nvertices, normals);

    // 파일이 열렸는지 확인
    if (!objFile.is_open()) {
        std::cerr << "파일을 열 수 없습니다." << std::endl;
        return;
    }
    else
        std::cout << "저장 시작합니다...! 찍은 점 개수와 회전 각도에 따라 오래걸릴 수 있습니다..." << std::endl;


    // .obj 파일 형식에 맞춰 각 정점의 좌표를 저장
    for (const xPoint3D& vertex : vertices) {
        objFile << "v " << vertex.x << " " << vertex.y << " " << vertex.z << "\n";
    }

    objFile << "\n";

    // 법선 벡터 정보를 저장
    for (int i = 0; i < normals.size(); i++) {
        objFile << "vn " << normals[i].x << " " << normals[i].y << " " << normals[i].z << "\n";
    }

    objFile << "\n";

    // 면 정보를 파일에 저장

    // 인덱스는 1부터 시작
    int i = 1;
    int l = 1;
    int p = Pointnum + 1;
    int q = Pointnum + 1;

    int n = 0;

    // 삼각형으로 저장
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

    // 마지막 면 저장 인덱스 초기화
    i = (Pointnum) * ((360 / angle) - 1) + 1;


    p = 1;
    q = 1;

    // 마지막 면 저장
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

    // 윗면, 아랫면 저장 인덱스 초기화
    int _i = 1;
    i = 1;
    l = Pointnum * (360 / angle) * 2 + 1;

    // 윗면 저장
    for (int k = 2; k < (360 / angle); k++) {
        objFile << "f " << _i << "//" << l << " ";

        for (int t = 0; t < 2; t++) {
            i += Pointnum;
            objFile << i << "//" << l << " ";
        }
        i -= Pointnum;
        objFile << "\n";
    }

    // 아랫면 저장
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

    std::cout << "파일을 닫습니다." << std::endl;

    objFile.close();

    std::cout << "모델이 " << outputFilename << "에 성공적으로 저장되었습니다...!" << std::endl;
}


// 정보들을 .obj형식으로 저장하는 함수 - 면을 삼각형이 아닌 다각형으로 저장
void SaveObjFileS(const std::vector<xPoint3D>& vertices, const std::string& outputFilename) {

    saveNewThing();

    // 입력받은 파일 이름을 사용하여 파일 열기
    std::ofstream objFile(outputFilename);

    CNT(_nvertices, normals);

    // 파일이 열렸는지 확인
    if (!objFile.is_open()) {
        std::cerr << "파일을 열 수 없습니다." << std::endl;
        return;
    }
    else
        std::cout << "저장 시작합니다...! 찍은 점 개수와 회전 각도에 따라 오래걸릴 수 있습니다..." << std::endl;


    // .obj 파일 형식에 맞춰 각 정점의 좌표를 파일에 저장
    for (const xPoint3D& vertex : vertices) {
        objFile << "v " << vertex.x << " " << vertex.y << " " << vertex.z << "\n";
    }

    objFile << "\n";

    // 법선 벡터 정보를 파일에 저장
    for (int i = 0; i < normals.size(); i++) {
        objFile << "vn " << normals[i].x << " " << normals[i].y << " " << normals[i].z << "\n";
    }

    objFile << "\n";

    // 면 정보를 파일에 저장

    // 인덱스는 1부터 시작
    int i = 1;
    int l = 2;
    int p = Pointnum + 1;
    int q = Pointnum + 1;

    int n = 0;

    // 다각형으로 저장
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

    // 마지막 면 저장 인덱스 초기화
    i = (Pointnum) * ((360 / angle) - 1) + 1;
    p = 1;
    q = 1;

    // 마지막 면 저장
    // 다각형으로 저장
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

    // 윗면, 아랫면 저장 인덱스 초기화
    i = 1;
    l = Pointnum * (360 / angle) * 2 + 1;

    // 윗면 저장
    objFile << "f ";
    for (int k = 0; k < (360 / angle); k++) {
        objFile << i << "//" << l << " ";
        i += Pointnum;

    }
    objFile << "\n";

    // 아랫면 저장
    i = Pointnum * (360 / angle);
    l = Pointnum * (360 / angle) * 2 + 2;
    objFile << "f ";
    for (int k = 0; k < (360 / angle); k++) {
        objFile << i << "//" << l << " ";
        i -= Pointnum;

    }
    objFile << "\n";

    std::cout << "파일을 닫습니다." << std::endl;
    // 파일을 닫습니다.
    objFile.close();

    std::cout << "모델이 " << outputFilename << "에 성공적으로 저장되었습니다...!" << std::endl;
}

void MyDisplay() {
    View2D();

    SetPointColor(1.0f, 1.0f, 1.0f);

    // 선 및 점 그리기
    draw2DAxes();
    SetPointSize(2.0f);
    Draw2Ddot();

    // 최근에 찍힌 점의 좌표 출력
    if (!points.empty()) {
        const Point& recentPoint = points.back();
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(-0.9f, 0.9f);

        std::string recentPointStr = "(" + std::to_string(recentPoint.x) + ", " + std::to_string(recentPoint.y) + ")";
        const char* recentPointCStr = recentPointStr.c_str();

        // 문자열 출력
        while (*recentPointCStr) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *recentPointCStr);
            recentPointCStr++;
        }
    }

    // 개수 출력
    glRasterPos2f(-0.9f, 0.85f);
    std::string pointCount = "Point Count: " + std::to_string(points.size());
    const char* countStr = pointCount.c_str();
    while (*countStr) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *countStr);
        countStr++;
    }

    glutSwapBuffers();
}

// 입력각도로 회전한 점들을 출력하고, 이를 xy평면에서 바라보는 함수
void My2Display() {
    View2D();

    SetPointColor(1.0f, 1.0f, 1.0f);

    // 축 및 점 그리기
    draw2DAxes();
    SetPointSize(2.0f);
    Draw3Ddot();

    glutSwapBuffers();
}

void My3Display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 배경색 설정 (회색)
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    SetPointColor(1.0f, 1.0f, 1.0f);

    // 선 및 점 그리기
    draw3DAxes();
    SetPointSize(2.0f);
    Draw3Ddot();

    glutSwapBuffers();
}

// 마우스 좌클릭을 통해 윈도우에 점을 출력 + 그 점의 좌표를 저장하는 함수
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

// 저장한 점들을 입력각도로 회전하고 저장하는데 필요한 행렬계산 함수
void rotatePoint() {
    xPoint3D newpt;
    for (int l = 0; l < (360 / angle); l++) {
        for (int i = 0; i < Pointnum; i++) {
            float fRotAngle = angle;
            float radian;

            fRotAngle = fRotAngle * l; // 회전 각도를 현재 회전 횟수에 고정
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
                std::cout << "회전 각도를 입력하세요 (360의 약수): ";
                std::cin >> angle;

                // 입력 받은 각도가 360의 약수인지 확인
                if (fmod(360.0f, angle) == 0) {
                    std::cout << "입력 받았습니다...!" << std::endl;
                    B2 = true;
                    break;
                }

                else {
                    std::cout << "360의 약수가 아닙니다. 다시 입력하세요." << std::endl;
                }
            } while (true);

            // 각도가 [0, 360) 범위 내에 있도록 제한
            angle = fmod(angle, 360.0f);
            rotatePoint();

            glutPostRedisplay();

            isAngle = false;

            break;
        }
        else if (B1 != true) {
            std::cout << "!!!이전 프로세스를 완료하지 않았습니다!!!" << std::endl;
            break;
        }
        else if (isAngle != true) {
            std::cout << "!!!이미 각도를 입력했습니다!!!" << std::endl;
            break;
        }

    case 2:
        if (B2 == true) {
            std::cout << "회전한 점을 그립니다...!" << std::endl;
            glutDisplayFunc(My2Display);
            glutPostRedisplay();
            B3 = true;
            break;
        }
        else {
            std::cout << "!!!이전 프로세스를 완료하지 않았습니다!!!" << std::endl;
            break;
        }

    case 3:
        if (B3 == true) {
            std::cout << "회전한 점을 선으로 그립니다...!" << std::endl;
            glutDisplayFunc(MeshDisplay);
            glutPostRedisplay();
            B4 = true;
            break;
        }
        else {
            std::cout << "!!!이전 프로세스를 완료하지 않았습니다!!!" << std::endl;
            break;
        }

    case 4:
        if (B3 == true) {
            std::cout << "Mesh Model을 3D뷰로 출력합니다...!" << std::endl;
            glutDisplayFunc(MeshDis3D);
            glutPostRedisplay();
            B4 = true;
            break;
        }
        else {
            std::cout << "!!!이전 프로세스를 완료하지 않았습니다!!!" << std::endl;
            break;
        }

    case 5:
        if (B4 == true) {
            std::cout << "Mesh를 Polygon 형태로 출력합니다...!" << std::endl;
            glutDisplayFunc(PolygonDisplay);
            glutPostRedisplay();
            B5 = true;
            break;
        }
        else {
            std::cout << "!!!이전 프로세스를 완료하지 않았습니다!!!" << std::endl;
            break;
        }

    case 6:
        if (B4 == true) {
            std::cout << "Polygon Model을 3D뷰로 출력합니다...!" << std::endl;
            glutDisplayFunc(PolDis3D);
            glutPostRedisplay();
            B5 = true;
            break;
        }
        else {
            std::cout << "!!!이전 프로세스를 완료하지 않았습니다!!!" << std::endl;
            break;
        }

    case 7:
        if (B5 == true && isSave == false) {
            isSave = true;
            if (arRotPoints.size() > 0) {
                std::string filename;
                std::cout << "Wavefront .obj 파일로 저장할 파일 이름을 입력 하세요: ";
                std::cin >> filename;

                filename = "build/obj/" + filename;
                std::cout << filename << std::endl;
                // 모델을 Wavefront .obj 파일로 저장합니다.
                SaveObjFileT(arRotPoints, filename);

                isSave = false;
                filename = "";
            }
            break;
        }
        else {
            std::cout << "!!!이전 프로세스를 완료하지 않았습니다!!!" << std::endl;
            break;
        }
    case 8:
        if (B5 == true && isSave == false) {
            isSave = true;
            if (arRotPoints.size() > 0) {
                std::string filename;
                std::cout << "Wavefront .obj 파일로 저장할 파일 이름을 입력 하세요: ";
                std::cin >> filename;

                filename = "build/obj/" + filename;

                // 모델을 Wavefront .obj 파일로 저장합니다.
                SaveObjFileS(arRotPoints, filename);

                isSave = false;
                filename = "";
            }
            break;
        }
        else {
            std::cout << "!!!이전 프로세스를 완료하지 않았습니다!!!" << std::endl;
            break;
        }

    case 9:
        std::cout << "점을 초기화 중..." << std::endl;
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
        std::cout << "성공...!" << std::endl;
        break;

    case 0:
        std::cout << "프로그램을 종료합니다." << std::endl;
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

