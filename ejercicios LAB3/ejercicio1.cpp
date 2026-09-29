/**
 * ============================================================================
 * ASIGNATURA: TECNOLOGÍA DE OBJETOS
 * PRÁCTICA 03: C++ Herencia, Polimorfismo, Qt
 * ESTUDIANTE: Franco Jesus Cahua Soto
 * ARCHIVO: ejercicio1.cpp
 * DESCRIPCIÓN: Jerarquía de HERENCIA SIMPLE que modela el oficio como
 *              clasificación taxonómica de las personas:
 *
 *                    (abstracta) Persona
 *                   /      |      |      \
 *              Arquitecto Doctor Enfermera Bombero
 *
 *              - La clase base declara la INTERFAZ común mediante funciones
 *                virtuales puras (oficio, tareaPrincipal, salarioMensual).
 *              - Los datos comunes (nombre, edad, documento, experiencia,
 *                salario base) se declaran como `protected` para que las
 *                derivadas accedan directamente SIN exponerlos al exterior.
 *              - Cada derivada REDEFINE la interfaz con su comportamiento
 *                propio, lo que se materializa en POLIMORFISMO al invocar los
 *                métodos a través de punteros/referencias a la clase base.
 *              - Se demuestra además el efecto de los distintos NIVELES DE
 *                ACCESO de la herencia (public / protected / private) sobre
 *                la visibilidad de la interfaz heredada.
 * ============================================================================
 */

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

// ============================================================================
// CLASE BASE ABSTRACTA: Persona
// Encapsula todo lo que TODA persona workforce comparte, sinergiando la
// reutilizacion de codigo (DRY) a traves de la herencia.
// ============================================================================
class Persona {
protected:
    std::string nombre;
    std::string apellido;
    int edad;
    std::string documento;
    int aniosExperiencia;
    double salarioBase;

public:
    Persona(const std::string& nom, const std::string& ape, int ed,
            const std::string& doc, int anios, double salario)
        : nombre(nom), apellido(ape), edad(ed), documento(doc),
          aniosExperiencia(anios), salarioBase(salario) {}

    // Destructor virtual: imprescindible para destruir correctamente los
    // objetos proprios de las clases derivadas a traves de un puntero base.
    virtual ~Persona() = default;

    // --- INTERFAZ PURAMENTE VIRTUAL: obliga a toda derivada a implementarla ---
    virtual std::string obtenerOficio() const = 0;
    virtual std::string tareaPrincipal() const = 0;
    virtual double calcularSalarioMensual() const = 0;

    // --- METODO VIRTUAL CON IMPLEMENTACION POR DEFECTO (herencia de comportamiento) ---
    virtual void presentarHabilidad() const {
        std::cout << "  [" << obtenerOficio() << "] " << nombre << " " << apellido
                  << " -> " << tareaPrincipal() << "\n";
    }

    // --- METODO NO VIRTUAL: comportamiento identico para toda la jerarquia ---
    void mostrarDatosBasicos() const {
        std::cout << "    Nombre      : " << nombre << " " << apellido << "\n"
                  << "    Edad        : " << edad << " anios\n"
                  << "    Documento   : " << documento << "\n"
                  << "    Experiencia : " << aniosExperiencia << " anios\n"
                  << "    Salario base: S/. " << std::fixed << std::setprecision(2)
                  << salarioBase << "\n";
    }

    int getEdad() const { return edad; }
    int getAniosExperiencia() const { return aniosExperiencia; }
    std::string getNombreCompleto() const { return nombre + " " + apellido; }
};

// ============================================================================
// CLASE DERIVADA 1: Arquitecto  (herencia publica)
// ============================================================================
class Arquitecto : public Persona {
private:
    std::string especialidad;
    std::string colegiatura;
    int proyectosTerminados;

public:
    Arquitecto(const std::string& nom, const std::string& ape, int ed,
               const std::string& doc, int anios, double salario,
               const std::string& esp, const std::string& coleg, int proyectos)
        : Persona(nom, ape, ed, doc, anios, salario),
          especialidad(esp), colegiatura(coleg), proyectosTerminados(proyectos) {}

    std::string obtenerOficio() const override { return "ARQUITECTO"; }

    std::string tareaPrincipal() const override {
        return "Elabora planos y dirige obras de construccion";
    }

    // Redefinicion con comportamiento especifico de la clase derivada
    void presentarHabilidad() const override {
        std::cout << "  [ARQUITECTO] " << getNombreCompleto()
                  << " | " << especialidad << " | " << colegiatura << "\n"
                  << "      => Actividad: " << tareaPrincipal() << "\n"
                  << "      => Proyectos terminados: " << proyectosTerminados << "\n";
    }

    double calcularSalarioMensual() const override {
        return salarioBase + (proyectosTerminados * 350.0) + (aniosExperiencia * 25.0);
    }

    std::string getEspecialidad() const { return especialidad; }
    int getProyectosTerminados() const { return proyectosTerminados; }
};

// ============================================================================
// CLASE DERIVADA 2: Doctor  (herencia publica)
// ============================================================================
class Doctor : public Persona {
private:
    std::string especialidad;
    std::string colegiatura;
    int pacientesAtendidos;

public:
    Doctor(const std::string& nom, const std::string& ape, int ed,
           const std::string& doc, int anios, double salario,
           const std::string& esp, const std::string& coleg, int pacientes)
        : Persona(nom, ape, ed, doc, anios, salario),
          especialidad(esp), colegiatura(coleg), pacientesAtendidos(pacientes) {}

    std::string obtenerOficio() const override { return "DOCTOR"; }

    std::string tareaPrincipal() const override {
        return "Diagnostica, receta y hace seguimiento clinico";
    }

    void presentarHabilidad() const override {
        std::cout << "  [DOCTOR] " << getNombreCompleto()
                  << " | " << especialidad << " | " << colegiatura << "\n"
                  << "      => Actividad: " << tareaPrincipal() << "\n"
                  << "      => Pacientes atendidos: " << pacientesAtendidos << "\n";
    }

    double calcularSalarioMensual() const override {
        // Honorarios por consulta mas un recargo por cada paciente atendido.
        return salarioBase + (pacientesAtendidos * 45.0) + (aniosExperiencia * 60.0);
    }

    std::string getEspecialidad() const { return especialidad; }
    int getPacientesAtendidos() const { return pacientesAtendidos; }
};

// ============================================================================
// CLASE DERIVADA 3: Enfermera  (herencia publica)
// ============================================================================
class Enfermera : public Persona {
private:
    std::string turno;
    std::string especialidad;
    int pacientesACargo;

public:
    Enfermera(const std::string& nom, const std::string& ape, int ed,
              const std::string& doc, int anios, double salario,
              const std::string& esp, const std::string& tur, int pacientes)
        : Persona(nom, ape, ed, doc, anios, salario),
          turno(tur), especialidad(esp), pacientesACargo(pacientes) {}

    std::string obtenerOficio() const override { return "ENFERMERA"; }

    std::string tareaPrincipal() const override {
        return "Administra medicamentos y toma signos vitales";
    }

    void presentarHabilidad() const override {
        std::cout << "  [ENFERMERA] " << getNombreCompleto()
                  << " | " << especialidad << " | Turno " << turno << "\n"
                  << "      => Actividad: " << tareaPrincipal() << "\n"
                  << "      => Pacientes a cargo: " << pacientesACargo << "\n";
    }

    double calcularSalarioMensual() const override {
        // Sobresueldo nocturno por turno y por carga de peligrosidad clinica.
        double sobresueldoNocturno = (turno == "Nocturno") ? 450.0 : 0.0;
        return salarioBase + sobresueldoNocturno + (pacientesACargo * 20.0);
    }

    std::string getTurno() const { return turno; }
    int getPacientesACargo() const { return pacientesACargo; }
};

// ============================================================================
// CLASE DERIVADA 4: Bombero  (herencia publica)
// ============================================================================
class Bombero : public Persona {
private:
    std::string estacion;
    std::string certificacion;
    int incidentesAtendidos;

public:
    Bombero(const std::string& nom, const std::string& ape, int ed,
            const std::string& doc, int anios, double salario,
            const std::string& est, const std::string& cert, int sincidentes)
        : Persona(nom, ape, ed, doc, anios, salario),
          estacion(est), certificacion(cert), incidentesAtendidos(sincidentes) {}

    std::string obtenerOficio() const override { return "BOMBERO"; }

    std::string tareaPrincipal() const override {
        return "Extingue incendios y realiza rescates";
    }

    void presentarHabilidad() const override {
        std::cout << "  [BOMBERO] " << getNombreCompleto()
                  << " | Estacion " << estacion << " | " << certificacion << "\n"
                  << "      => Actividad: " << tareaPrincipal() << "\n"
                  << "      => Incidentes atendidos: " << incidentesAtendidos << "\n";
    }

    double calcularSalarioMensual() const override {
        // Riesgo de vida: bono por incidente + bono por peligrosidad.
        return salarioBase + (incidentesAtendidos * 80.0) + (aniosExperiencia * 40.0);
    }

    std::string getEstacion() const { return estacion; }
    int getIncidentesAtendidos() const { return incidentesAtendidos; }
};

// ============================================================================
// DEMOSTRACION DE NIVELES DE ACCESO DE LA HERENCIA
// La clase `PasanteInvestigacion` hereda PRIVADAMENTE de `Arquitecto`, por lo
// que su interfaz queda oculta: no puede invocar los metodos publicos de la
// base a traves de la derivada. Compilar "p.asignarObra()" falla con:
//     error: 'PasanteInvestigacion' has no member named 'asignarObra'.
// ============================================================================
class AsistenteTecnico : private Arquitecto {
public:
    AsistenteTecnico(const std::string& nom, const std::string& ape, int ed,
                      const std::string& doc, int anios, double salario)
        : Arquitecto(nom, ape, ed, doc, anios, salario, "Estructuras", "SIC-2026", 0) {}

    // La clase debe "re-exponer" explicitamente lo que desea publicar.
    std::string getOficioDelegado() const { return obtenerOficio(); }
};

// ============================================================================
// FUNCION AUXILIAR: reporte polimorfico homogeneouso
// ============================================================================
void generarReporte(const std::vector<Persona*>& plantilla) {
    std::cout << "\n>>> 4. REPORTE CONSOLIDADO DEL PERSONAL (enlace polimorfico)\n";
    std::cout << "-----------------------------------------------------------------\n";
    double planchaTotal = 0.0;
    for (const Persona* p : plantilla) {
        p->mostrarDatosBasicos();
        std::cout << "    Oficio      : " << p->obtenerOficio() << "\n"
                  << "    Salario final: S/. " << std::fixed << std::setprecision(2)
                  << p->calcularSalarioMensual() << "\n\n";
        planchaTotal += p->calcularSalarioMensual();
    }
    std::cout << "    PLANCHA MENSUAL TOTAL: S/. " << std::fixed << std::setprecision(2)
              << planchaTotal << "\n";
}

// ============================================================================
// PROGRAMA PRINCIPAL
// ============================================================================
int main() {
    std::cout << "=========================================================\n";
    std::cout << "  TECNOLOGÍA DE OBJETOS - LAB 03 | EJERCICIO 01\n";
    std::cout << "  HERENCIA: PERSONAS CLASIFICADAS POR OFICIO\n";
    std::cout << "  Estudiante: Franco Jesus Cahua Soto\n";
    std::cout << "=========================================================\n";

    // 1. Instanciacion de las clases derivadas en memoria dinamica (Heap).
    //    Al construirse como punteros, la liberacion `delete` del punto 6
    //    ejercita el destructor virtual de la clase base.
    Arquitecto* arq = new Arquitecto("Lucia", "Fernandez Ramirez", 41, "DNI 43829175", 15, 3500.0,
                                     "Arq. Sustentable", "SIC-2011-4478", 28);
    Doctor* doc = new Doctor("Andres", "Quispe Loayza", 47, "CMP 31245-RNE", 19, 4200.0,
                             "Cardiologia", "RNE-8871-22", 145);
    Enfermera* enf = new Enfermera("Rosa", "Quispe Mamani", 33, "CEP 58210-AB", 8, 2200.0,
                                   "Enf. Intensiva", "Nocturno", 6);
    Bombero* bom = new Bombero("Carlos", "Ccahuana Tito", 39, "DNI 40118392", 13, 2600.0,
                               "Central 104", "Brigadista PARAM.", 67);

    std::cout << "\n>>> 1. FICHA INDIVIDUAL DE CADA PROFESIONAL (invocacion directa):\n";
    arq->mostrarDatosBasicos();
    arq->presentarHabilidad();
    std::cout << "    Salario mensual calculado: S/. " << std::fixed << std::setprecision(2)
              << arq->calcularSalarioMensual() << "\n\n";

    doc->mostrarDatosBasicos();
    doc->presentarHabilidad();
    std::cout << "    Salario mensual calculado: S/. " << std::fixed << std::setprecision(2)
              << doc->calcularSalarioMensual() << "\n\n";

    enf->mostrarDatosBasicos();
    enf->presentarHabilidad();
    std::cout << "    Salario mensual calculado: S/. " << std::fixed << std::setprecision(2)
              << enf->calcularSalarioMensual() << "\n\n";

    bom->mostrarDatosBasicos();
    bom->presentarHabilidad();
    std::cout << "    Salario mensual calculado: S/. " << std::fixed << std::setprecision(2)
              << bom->calcularSalarioMensual() << "\n";

    // 2. Polimorfismo: un vector de punteros a la clase base y la misma llamada
    //    resuelve a la implementacion concreta de cada objeto.
    std::vector<Persona*> plantilla;
    plantilla.push_back(arq);
    plantilla.push_back(doc);
    plantilla.push_back(enf);
    plantilla.push_back(bom);

    std::cout << "\n>>> 2. ITERACION POLIMORFICA (misma llamada, 4 cuerpos):\n";
    for (Persona* p : plantilla) {
        p->presentarHabilidad();
    }

    std::cout << "\n>>> 3. POLIMORFISMOS EN EL CALCULO DEL SUELDO (misma interfaz):\n";
    std::cout << "  OFICIO         EDAD   EXPERIENCIA   SUELDO MENSUAL\n";
    std::cout << "  ------------------------------------------------------\n";
    for (Persona* p : plantilla) {
        std::cout << "  " << std::left << std::setw(14) << p->obtenerOficio()
                  << std::setw(7) << p->getEdad()
                  << std::setw(14) << p->getAniosExperiencia()
                  << "S/. " << std::right << std::setw(10) << std::fixed
                  << std::setprecision(2) << p->calcularSalarioMensual() << "\n";
    }

    // 4. Reporte consolidado mediante la funcion que recibe la plantilla
    generarReporte(plantilla);

    // 5. Demostracion del nivel de acceso `private` en la herencia
    std::cout << "\n>>> 5. NIVEL DE ACCESO `private` EN LA HERENCIA:\n";
    AsistenteTecnico asis("Diego", "Palomino Vega", 25, "DNI 70122345", 2, 1800.0);
    std::cout << "    La interfaz heredada queda oculta para el usuario externo.\n"
              << "    asis.obtenerOficio()        -> ERROR de compilacion (es privado)\n"
              << "    asis.calcularSalarioMensual()-> ERROR de compilacion (es privado)\n"
              << "    Delegacion permitida       -> " << asis.getOficioDelegado() << "\n";

    // 6. Liberacion: el destructor virtual de Persona permite limpiar el Heap
    //    sin fugas aunque se libere a traves de punteros a la clase base.
    for (Persona* p : plantilla) {
        delete p;
    }
    std::cout << "\n>>> 6. DESTRUCTORES VIRTUALES EJECUTADOS: 4 objetos liberados.\n";

    return 0;
}
