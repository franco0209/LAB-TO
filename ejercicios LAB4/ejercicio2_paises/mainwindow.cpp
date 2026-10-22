#include "mainwindow.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Paises, idiomas y capitales");

    paises = {
        {"Peru",      {"Espanol (y quechua, aimara)", "Lima"}},
        {"Argentina", {"Espanol",                     "Buenos Aires"}},
        {"Brasil",    {"Portugues",                   "Brasilia"}},
        {"Chile",     {"Espanol",                     "Santiago"}},
        {"Colombia",  {"Espanol",                     "Bogota"}},
        {"Francia",   {"Frances",                     "Paris"}},
        {"Alemania",  {"Aleman",                      "Berlin"}},
        {"Italia",    {"Italiano",                    "Roma"}},
        {"Japon",     {"Japones",                     "Tokio"}},
        {"Estados Unidos", {"Ingles",                 "Washington D. C."}}
    };

    listaPaises = new QListWidget;
    listaPaises->addItems(paises.keys());

    etiquetaIdioma  = new QLabel("Idioma: (seleccione un pais)");
    etiquetaCapital = new QLabel("Capital: (seleccione un pais)");

    auto *panelDatos = new QVBoxLayout;
    panelDatos->addWidget(etiquetaIdioma);
    panelDatos->addWidget(etiquetaCapital);
    panelDatos->addStretch();

    auto *principal = new QHBoxLayout;
    principal->addWidget(listaPaises);
    principal->addLayout(panelDatos);

    auto *central = new QWidget;
    central->setLayout(principal);
    setCentralWidget(central);

    // Conexion senal -> slot: al cambiar de pais se actualizan las etiquetas.
    connect(listaPaises, &QListWidget::currentItemChanged,
            this, &MainWindow::mostrarDatos);

    // Conexion senal -> lambda: registro en la consola de depuracion.
    connect(listaPaises, &QListWidget::itemClicked, [](QListWidgetItem *item) {
        qDebug() << "Clic sobre:" << item->text();
    });
}

void MainWindow::mostrarDatos(QListWidgetItem *actual)
{
    if (!actual) return;
    const DatosPais &d = paises[actual->text()];
    etiquetaIdioma->setText("Idioma: " + d.idioma);
    etiquetaCapital->setText("Capital: " + d.capital);
}
