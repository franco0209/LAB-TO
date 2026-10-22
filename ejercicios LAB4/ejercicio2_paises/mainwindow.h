#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QString>

class QListWidget;
class QListWidgetItem;
class QLabel;

// Datos de cada pais mostrados al seleccionarlo.
struct DatosPais {
    QString idioma;
    QString capital;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private slots:
    // Slot: se ejecuta cuando cambia el pais seleccionado en la lista.
    void mostrarDatos(QListWidgetItem *actual);

private:
    QListWidget *listaPaises;
    QLabel *etiquetaIdioma;
    QLabel *etiquetaCapital;
    QMap<QString, DatosPais> paises;
};

#endif // MAINWINDOW_H
