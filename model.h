#ifndef MODEL_H
#define MODEL_H

#include <QVector>
#include <QVector3D>

class Model {
public:
    Model(); // 仅声明
    ~Model(); // 仅声明
    bool load(const QString &path); // 声明
    const float* vertexData() const;
    const unsigned int* indexData() const;
    int vertexDataSize() const;
    int indexDataSize() const;
    int indexCount() const;
    int vertexCount() const;
    QVector3D getBoundingMin() const;
    QVector3D getBoundingMax() const;

private:
    QVector<float> vertices;
    QVector<unsigned int> indices;
};

#endif // MODEL_H
