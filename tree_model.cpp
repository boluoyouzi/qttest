#include "tree_model.h"
#include <QStandardItem>
#include <QIcon>

TreeModel::TreeModel(QObject *parent) : QStandardItemModel(parent) {
    setupModel();
}

void TreeModel::setupModel()
{
    setHorizontalHeaderLabels({"Component", "Details"});

    // 第一级节点
    QStandardItem *root = invisibleRootItem();

    QStandardItem *general = new QStandardItem("General Modeling");
    general->setIcon(QIcon::fromTheme("folder"));
    root->appendRow(general);

    QStandardItem *hydro = new QStandardItem("Hydro Modeling");
    hydro->setIcon(QIcon::fromTheme("folder"));
    root->appendRow(hydro);

    // 添加子项
    QList<QStandardItem*> subItems;
    subItems << new QStandardItem("Terrain Analysis") << new QStandardItem("");
    general->appendRow(subItems);

    subItems.clear();
    subItems << new QStandardItem("Flow Simulation") << new QStandardItem("");
    hydro->appendRow(subItems);
}
