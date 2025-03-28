#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class GLWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();
    void on_treeView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    GLWidget *glWidget;
    QStandardItemModel *treeModel;

    void setupTreeView();
};
#endif // MAINWINDOW_H
