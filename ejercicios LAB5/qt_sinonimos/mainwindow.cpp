#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QStringList>

// Pares sugeridos: el primero es el de la guia (Subir - Ascender). Se van
// entregando en orden para que dos filas nuevas no repitan informacion.
static const QStringList kSugerencias = {
    "Subir - Ascender",
    "Bajar - Descender",
    "Grande - Enorme",
    "Rapido - Veloz",
    "Hablar - Conversar",
    "Bonito - Hermoso"
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mTasks()
{
    ui->setupUi(this);

    // a) el texto del boton cambia de "Agregar tarea" a "Agregar sinonimos"
    ui->addTaskButton->setText("Agregar sinónimos");

    connect(ui->addTaskButton, &QPushButton::clicked, this, &MainWindow::addTask);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addTask()
{
    qDebug() << "Slot corresponde al clic en el boton ...";

    // b) fila nueva: checkbox sin marcar + sinonimos + Editar + Quitar
    Task *task = new Task(siguienteSugerencia());
    mTasks.append(task);
    ui->tasksLayout->addWidget(task);

    connect(task, &Task::removed, this, &MainWindow::removeTask);
    connect(task, &Task::editRequested, this, &MainWindow::editTask);
}

void MainWindow::removeTask(Task *task)
{
    // d) Quitar elimina toda la fila
    mTasks.removeOne(task);
    ui->tasksLayout->removeWidget(task);
    task->setParent(nullptr);
    task->deleteLater();
    qDebug() << "objeto eliminado...";
}

void MainWindow::editTask(Task *task)
{
    // c) Editar: el usuario cambia los sinonimos; se rechaza texto vacio o
    //    una pareja que ya figure en otra fila del checklist.
    bool ok = false;
    const QString nuevo = QInputDialog::getText(
        this, "Editar sinónimos", "Sinónimos (ej. Subir - Ascender):",
        QLineEdit::Normal, task->synonyms(), &ok).trimmed();
    if (!ok)
        return;

    if (nuevo.isEmpty()) {
        QMessageBox::warning(this, "Dato inválido", "El texto no puede estar vacío.");
        return;
    }
    if (existeSinonimo(nuevo, task)) {
        QMessageBox::warning(this, "Dato repetido",
                             "Ya existe una fila con esos sinónimos en el checklist.");
        return;
    }
    task->setSynonyms(nuevo);
    qDebug() << "sinonimos editados:" << nuevo;
}

bool MainWindow::existeSinonimo(const QString &texto, const Task *excepto) const
{
    for (const Task *t : mTasks) {
        if (t != excepto && t->synonyms().compare(texto, Qt::CaseInsensitive) == 0)
            return true;
    }
    return false;
}

QString MainWindow::siguienteSugerencia() const
{
    for (const QString &s : kSugerencias) {
        if (!existeSinonimo(s, nullptr))
            return s;
    }
    // Se agotaron las sugerencias: texto numerado, siempre distinto.
    return QString("Sinónimo %1 - ...").arg(mTasks.size() + 1);
}
