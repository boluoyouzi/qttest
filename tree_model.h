#ifndef TREE_MODEL_H
#define TREE_MODEL_H

#include <QStandardItemModel>

class TreeModel : public QStandardItemModel
{
    //Q_OBJECT
public:
    explicit TreeModel(QObject *parent = nullptr);

private:
    void setupModel();
};
#endif // TREE_MODEL_H
