#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QMatrix4x4>
#include <QVector3D>
#include <QQuaternion>
#include <QOpenGLShaderProgram>
#include <QMouseEvent>
#include <QWheelEvent>
#include "model.h"


class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = nullptr); // 声明构造函数
    ~GLWidget(); // 仅声明

    void loadModel(const QString &path); // 声明

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    // ... 成员变量声明 ...
    QOpenGLShaderProgram program;
    Model *currentModel;  // 指向当前模型的指针
    GLuint vao, vbo, ebo; // OpenGL 缓冲区对象
    QMatrix4x4 projection;
    QMatrix4x4 view;
    QMatrix4x4 modelMatrix;
    QVector3D cameraPos;
    QPoint lastMousePos;
    float zoomFactor;
    void setupBuffers();
    void cleanupGL();
};

#endif // GLWIDGET_H
