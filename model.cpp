#include "model.h"
#include <QFile>
#include <QTextStream>
#include <QtDebug>

Model::Model() = default;

Model::~Model() = default;

bool Model::load(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "无法打开文件:" << path;
        return false;
    }

    vertices.clear();
    indices.clear();

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        QStringList parts = line.split(" ", Qt::SkipEmptyParts);
        if (parts.isEmpty()) continue;

        if (parts[0] == "v") {
            float x = parts[1].toFloat();
            float y = parts[2].toFloat();
            float z = parts[3].toFloat();
            // 添加顶点位置和固定颜色
            vertices << x << y << z
                     << 0.5f << 0.8f << 1.0f; // RGBA
        }
        else if (parts[0] == "f") {
            for (int i = 1; i < parts.size(); ++i) {
                QStringList indicesParts = parts[i].split("/");
                unsigned int idx = indicesParts[0].toUInt() - 1;
                indices << idx;
            }
        }
    }

    file.close();
    return !vertices.isEmpty();
}

int Model::indexCount() const { // 新增实现
    return static_cast<int>(indices.size());
}
const float* Model::vertexData() const { // 实现必须包含 const
    return vertices.constData();
}

// 确保其他函数如 indexData() 也有实现
const unsigned int* Model::indexData() const {
    return indices.constData();
}
int Model::vertexDataSize() const {
    return vertices.size() * sizeof(float); // 计算顶点数据总字节数
}
int Model::indexDataSize() const {
    return indices.size() * sizeof(unsigned int); // 计算索引数据总字节数
}
int Model::vertexCount() const {
    return static_cast<int>(vertices.size()); // 假设顶点数据存储在vertices容器
}
QVector3D Model::getBoundingMin() const {
    if (vertices.empty()) return QVector3D(0,0,0);

    float minX = vertices[0], minY = vertices[1], minZ = vertices[2];
    for (size_t i = 0; i < vertices.size(); i += 6) { // 假设每个顶点包含6个float（位置+颜色）
        minX = std::min(minX, vertices[i]);
        minY = std::min(minY, vertices[i+1]);
        minZ = std::min(minZ, vertices[i+2]);
    }
    return QVector3D(minX, minY, minZ);
}

QVector3D Model::getBoundingMax() const {
    if (vertices.empty()) return QVector3D(0,0,0);

    float maxX = vertices[0], maxY = vertices[1], maxZ = vertices[2];
    for (size_t i = 0; i < vertices.size(); i += 6) {
        maxX = std::max(maxX, vertices[i]);
        maxY = std::max(maxY, vertices[i+1]);
        maxZ = std::max(maxZ, vertices[i+2]);
    }
    return QVector3D(maxX, maxY, maxZ);
}
