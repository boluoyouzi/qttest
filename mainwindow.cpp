#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glwidget.h"
#include "tree_model.h"
#include <QFileDialog>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化3D视图
    qDebug() << "OpenGL控件句柄:" << ui->glWidget;

    // 初始化树形菜单
    treeModel = new TreeModel(this);
    ui->treeView->setModel(treeModel);
    ui->treeView->expandAll();

    // 连接信号槽
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::on_actionOpen_triggered);
    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::on_treeView_clicked);

    // 添加以下调试代码
    ui->menubar->show();
    ui->mainToolBar->show();
    ui->tabWidget->show();
    ui->splitter->setSizes({300, 500}); // 强制分割比例
    ui->tabWidget->setCurrentIndex(0); // 激活消息标签页
    qDebug() << "当前布局层级:" << ui->verticalLayout->parentWidget()->children();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Open OBJ File", "", "OBJ Files (*.obj)");
    if (!fileName.isEmpty()) {
        glWidget->loadModel(fileName);
        statusBar()->showMessage("Loaded: " + fileName, 3000);
    }
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    QStandardItem *item = treeModel->itemFromIndex(index);
    statusBar()->showMessage("Selected: " + item->text(), 2000);
}
