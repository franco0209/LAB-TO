#include "task.h"
#include "ui_task.h"

Task::Task(const QString &sinonimos, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Task)
{
    ui->setupUi(this);
    setSynonyms(sinonimos);

    // Paso 4 de la guia: el clicked() del boton "Quitar" emite removed(this).
    connect(ui->removeButton, &QPushButton::clicked, [this] { emit removed(this); });
    connect(ui->editButton, &QPushButton::clicked, [this] { emit editRequested(this); });
}

Task::~Task()
{
    delete ui;
}

void Task::setSynonyms(const QString &sinonimos)
{
    ui->Mitext->setText(sinonimos);
}

QString Task::synonyms() const
{
    return ui->Mitext->text();
}

bool Task::isChecked() const
{
    return ui->checkbox->isChecked();
}
