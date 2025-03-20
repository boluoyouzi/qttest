#include "glwidget.h"
#include <QOpenGLFunctions>
#include "model.h"
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
    currentModel(nullptr),
    vao(0), vbo(0), ebo(0),
    cameraPos(0, 0, 5),
    zoomFactor(45.0f) {
    // 初始化代码（可为空）
}

GLWidget::~GLWidget() {
    cleanupGL();
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions(); // 必须首先调用

    // 初始化着色器程序（仅执行一次）
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertex.glsl")) {
        qDebug() << "顶点着色器加载失败：" << program.log();
        return; // 加载失败直接返回
    }
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragment.glsl")) {
        qDebug() << "片段着色器加载失败：" << program.log();
        return;
    }
    if (!program.link()) {
        qDebug() << "着色器链接失败：" << program.log();
    } else {
        qDebug() << "着色器链接成功！";
    }

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // 初始化着色器
    //program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertex.glsl");
    //program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragment.glsl");
    //program.link();

    // 初始化摄像机
    //view.lookAt(cameraPos, QVector3D(0,0,0), QVector3D(0,1,0));
    view.setToIdentity();
    view.lookAt(QVector3D(0,0,5),  // 摄像机位置
                QVector3D(0,0,0),  // 观察点
                QVector3D(0,1,0)); // 上方向
    qDebug() << "初始视图矩阵:" << view;
}

void GLWidget::resizeGL(int w, int h)
{
    float aspect = float(w) / std::max(h, 1); // 防止除零
    projection.setToIdentity();
    projection.perspective(zoomFactor, aspect, 0.1f, 100.0f);
    qDebug() << "修正后的投影矩阵:" << projection;
}

void GLWidget::paintGL()
{
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glEnable(GL_DEPTH_TEST);

    // program.bind();
    // program.setUniformValue("projection", projection);
    // program.setUniformValue("view", view);
    // program.setUniformValue("model", modelMatrix);

    // if(currentModel) {
    //     glBindVertexArray(vao);
    //     glDrawElements(GL_TRIANGLES, currentModel->indexCount(), GL_UNSIGNED_INT, 0);
    // }

    // program.release();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    if (!program.bind()) {
        qWarning() << "着色器绑定失败";
        return;
    }

    // 传递矩阵Uniform
    program.setUniformValue("projection", projection);
    program.setUniformValue("view", view);
    program.setUniformValue("model", modelMatrix);

    if (currentModel && currentModel->indexCount() > 0) {
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES,
                       currentModel->indexCount(),
                       GL_UNSIGNED_INT,
                       0);
        qDebug() << "绘制调用完成，索引数:" << currentModel->indexCount();
    }

    program.release();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastMousePos = event->position().toPoint();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint currentPos = event->position().toPoint();
    float dx = currentPos.x() - lastMousePos.x();
    float dy = currentPos.y() - lastMousePos.y();

    if (event->buttons() & Qt::LeftButton) {
        QQuaternion rotX = QQuaternion::fromAxisAndAngle(0, 1, 0, dx * 0.5f);
        QQuaternion rotY = QQuaternion::fromAxisAndAngle(1, 0, 0, dy * 0.5f);
        modelMatrix.rotate(rotX * rotY);
        qDebug() << "模型矩阵更新:\n" << modelMatrix;
        update();
    }
    lastMousePos = currentPos;
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    // 直接获取 y 轴增量值
    zoomFactor -= event->angleDelta().y() * 0.1f;
    zoomFactor = qBound(1.0f, zoomFactor, 90.0f);
    resizeGL(width(), height());
    update();
}

void GLWidget::loadModel(const QString &path)
{
    qDebug() << "尝试加载模型文件:" << QFileInfo(path).absoluteFilePath();
    if (!QFile::exists(path)) {
        qWarning() << "文件不存在!";
        return;
    }
    //makeCurrent();
    //cleanupGL();
    qDebug() << "正在加载模型:" << path;
    currentModel = new Model();
    // currentModel->load(path);
    // if(currentModel->load(path)) {
    //     qDebug() << "顶点数:" << currentModel->vertexCount()
    //         << "索引数:" << currentModel->indexCount();
    //     setupBuffers();
    // } else {
    //     qDebug() << "模型加载失败";
    // }
    if(currentModel->load(path)) {
        qDebug() << "顶点数:" << currentModel->vertexCount()
            << "(" << currentModel->vertexDataSize() << " bytes)";
        qDebug() << "索引数:" << currentModel->indexCount()
                 << "(" << currentModel->indexDataSize() << " bytes)";

        // 打印前3个顶点数据
        const float* vertData = currentModel->vertexData();
        qDebug() << "顶点示例: ("
                 << vertData[0] << "," << vertData[1] << "," << vertData[2] << ")";
    }
    qDebug() << "模型坐标范围: ("
             << currentModel->getBoundingMin().x() << ","
             << currentModel->getBoundingMin().y() << ","
             << currentModel->getBoundingMin().z() << ") to ("
             << currentModel->getBoundingMax().x() << ","
             << currentModel->getBoundingMax().y() << ","
             << currentModel->getBoundingMax().z() << ")";
    setupBuffers();
    update();
}

void GLWidget::setupBuffers()
{
    // if (!currentModel) return;

    // 创建缓冲区对象
    // glGenVertexArrays(1, &vao);
    // glGenBuffers(1, &vbo);
    // glGenBuffers(1, &ebo);

    // glBindVertexArray(vao);

    // // 顶点数据
    // glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // glBufferData(GL_ARRAY_BUFFER, currentModel->vertexDataSize(),
    //              currentModel->vertexData(), GL_STATIC_DRAW);

    // // 索引数据
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, currentModel->indexDataSize(),
    //              currentModel->indexData(), GL_STATIC_DRAW);

    // // 设置属性指针
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);

    // glBindVertexArray(0);
    //---------------------------------------------------
    if (!currentModel || currentModel->vertexCount() == 0) {
        qWarning() << "无可用的模型数据";
        return;
    }

    // 删除旧缓冲区
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    // 创建新缓冲区
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    // 绑定顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 currentModel->vertexDataSize(),
                 currentModel->vertexData(),
                 GL_STATIC_DRAW);

    // 绑定索引数据
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 currentModel->indexDataSize(),
                 currentModel->indexData(),
                 GL_STATIC_DRAW);

    // 配置顶点属性 (假设每个顶点包含位置+颜色)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    qDebug() << "缓冲区配置完成，VAO:" << vao;
}

void GLWidget::cleanupGL()
{
    makeCurrent();
    if (vao) {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
    if (vbo) {
        glDeleteBuffers(1, &vbo);
        vbo = 0;
    }
    if (ebo) {
        glDeleteBuffers(1, &ebo);
        ebo = 0;
    }
    doneCurrent();

    delete currentModel;
    currentModel = nullptr;
}
