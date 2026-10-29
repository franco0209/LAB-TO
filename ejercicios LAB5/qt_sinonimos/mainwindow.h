#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

#include "task.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void addTask();                 // clic en "Agregar sinonimos"
    void removeTask(Task *task);    // recibe removed(Task*) de la fila
    void editTask(Task *task);      // recibe editRequested(Task*) de la fila

private:
    bool existeSinonimo(const QString &texto, const Task *excepto) const;
    QString siguienteSugerencia() const;

    Ui::MainWindow *ui;
    QVector<Task *> mTasks;         // filas instanciadas
};

#endif // MAINWINDOW_H
