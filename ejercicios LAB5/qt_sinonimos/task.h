#ifndef TASK_H
#define TASK_H

#include <QWidget>
#include <QString>

namespace Ui {
class Task;
}

// Fila del checklist: checkbox (sin marcar) + texto con los sinonimos +
// botones Editar y Quitar. Hereda de QWidget y usa Q_OBJECT para poder
// declarar signals.
class Task : public QWidget
{
    Q_OBJECT

public:
    explicit Task(const QString &sinonimos, QWidget *parent = nullptr);
    ~Task();

    void setSynonyms(const QString &sinonimos);
    QString synonyms() const;
    bool isChecked() const;

signals:
    void removed(Task *task);        // emitida desde clicked() de "Quitar"
    void editRequested(Task *task);  // emitida desde clicked() de "Editar"

private:
    Ui::Task *ui;
};

#endif // TASK_H
