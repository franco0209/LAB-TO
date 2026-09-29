/**
 * ============================================================================
 * ASIGNATURA: TECNOLOGÍA DE OBJETOS
 * PRÁCTICA 03: C++ Herencia, Polimorfismo, Qt
 * ESTUDIANTE: Franco Jesus Cahua Soto
 * ARCHIVO: ejercicio2.cpp
 * DESCRIPCIÓN: HERENCIA MÚLTIPLE aplicada a las CEPAS DEL CORONAVIRUS,
 *              reutilizando el patrón de clasificación taxonómica del
 *              ejercicio 1 (personas por oficio -> variantes virales).
 *
 *   Composición del modelo (rombo clásico de la herencia múltiple):
 *
 *                        Fenotipo  (abstracta, raíz común)
 *                       /    |      |        |       \
 *                 CepaBase  AltaI.  Transm.  Resist.   <- mezcla simple
 *                    |       \       |        /          y múltiple
 *          +---------+-----+--+-------+----+-----+
 *          |         |     |              |
 *        Alfa      Delta   Lambda       Omicron        <- HERENCIA MÚLTIPLE
 *
 *   - `Delta` y `Omicron` heredan SIMULTÁNEAMENTE de 4 clases base,
 *     combinando fenotipos (infectividad, vía de transmisión, resistencia
 *     antiviral) que no guardan jerarquía entre sí.
 *   - El rombo (Fenotipo alcanzable por varios caminos) se resuelve con
 *     HERENCIA VIRTUAL (`public virtual Fenotipo`).
 *   - `dynamic_cast` (RTTI) permite preguntar en tiempo de ejecución qué
 *     fenotipos posee cada cepa.
 *   - El personal de salud modelado en el ejercicio 1 (Médico / Enfermera /
 *     Investigador) se reutiliza para asignar respuesta clínica a cada cepa.
 * ============================================================================
 */

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

// ============================================================================
// RAÍZ COMÚN DE TODOS LOS FENOTIPOS (base virtual de la jerarquía múltiple)
// ============================================================================
class Fenotipo {
protected:
    std::string codigoFenotipo;

public:
    explicit Fenotipo(const std::string& codigo) : codigoFenotipo(codigo) {}
    virtual ~Fenotipo() = default;

    // Interfaz común: cada combinación concreta produce su propio resumen.
    virtual std::string resumenFenotipo() const = 0;

    std::string getCodigo() const { return codigoFenotipo; }
};

// ============================================================================
// COMPONENTE 1 - IDENTIDAD VIRAL (herencia simple desde la raíz)
// ============================================================================
class CepaBase : public virtual Fenotipo {
protected:
    std::string nombreCepa;    // SARS-CoV-2 Omicron B.1.529
    std::string linaje;        // Omicron
    std::string paisOrigen;
    int periodoIncubacion;    // días
    double letalidadIfr;      // Infected Fatality Ratio en porcentaje

public:
    CepaBase(const std::string& nombre, const std::string& lin,
             const std::string& origen, int incubacion, double letalidad)
        : nombreCepa(nombre), linaje(lin), paisOrigen(origen),
          periodoIncubacion(incubacion), letalidadIfr(letalidad) {}

    virtual ~CepaBase() = default;

    // Identidad genética específica: obliga a cada variante a definirse.
    virtual std::string genotipado() const = 0;

    // Método virtual con implementación por defecto (patrón Template Method):
    // imprime la parte común y delega el detalle a resumenFenotipo().
    virtual void mostrarFicha() const {
        std::cout << "  Codigo        : " << getCodigo() << "\n"
                  << "  Linaje        : " << linaje << "\n"
                  << "  Denominacion  : " << nombreCepa << "\n"
                  << "  Origen        : " << paisOrigen << "\n"
                  << "  Incubacion    : " << periodoIncubacion << " dias\n"
                  << "  Letalidad IFR : " << std::fixed << std::setprecision(2)
                  << letalidadIfr << " %\n"
                  << "  Fenotipo      : " << resumenFenotipo() << "\n";
    }

    virtual std::string nivelSeveridad() const {
        if (letalidadIfr >= 2.0) return "CRITICA";
        if (letalidadIfr >= 0.5) return "ALTA";
        return "MODERADA";
    }

    const std::string& getNombre() const { return nombreCepa; }
    const std::string& getLinaje() const { return linaje; }
    double getLetalidad() const { return letalidadIfr; }
    int getIncubacion() const { return periodoIncubacion; }
};

// ============================================================================
// COMPONENTE 2 - ALTA INFECTIVIDAD (mezcla de comportamiento)
// ============================================================================
class AltaInfectividad : public virtual Fenotipo {
private:
    double afinidadSpike;   // afinidad de la proteína spike por el receptor ACE2

public:
    AltaInfectividad() : Fenotipo("MIX-IN: INFECTIVIDAD"), afinidadSpike(0.0) {}

    void setAfinidadSpike(double v) { afinidadSpike = v; }
    double getAfinidadSpike() const { return afinidadSpike; }

    void mostrarInfectividad() const {
        std::cout << "    [MIX-IN 1] Afinidad spike/ACE2 = " << std::fixed
                  << std::setprecision(2) << afinidadSpike << "\n";
    }
};

// ============================================================================
// COMPONENTE 3 - TRANSMISIÓN AÉREA (mezcla de comportamiento)
// ============================================================================
class TransmisionAerea : public virtual Fenotipo {
private:
    double r0;
    std::string principalVecto;

public:
    TransmisionAerea() : Fenotipo("MIX-IN: TRANSMISION"), r0(0.0) {}

    void setR0(double v) { r0 = v; }
    void setPrincipalVecto(const std::string& v) { principalVecto = v; }
    double getR0() const { return r0; }

    void mostrarTransmision() const {
        std::cout << "    [MIX-IN 2] R0 = " << std::fixed << std::setprecision(2)
                  << r0 << "  via " << principalVecto << "\n";
    }
};

// ============================================================================
// COMPONENTE 4 - RESISTENCIA ANTIVIRAL (mezcla de comportamiento)
// ============================================================================
class ResistenciaAntiviral : public virtual Fenotipo {
private:
    double reduccionRemdesivir;   // % de pérdida de sensibilidad al antiviral
    double escapeVacunal;         // % de escape de anticuerpos

public:
    ResistenciaAntiviral() : Fenotipo("MIX-IN: ANTIVIRAL"),
                             reduccionRemdesivir(0.0), escapeVacunal(0.0) {}

    void setResistencia(double remdesivir, double evasion) {
        reduccionRemdesivir = remdesivir;
        escapeVacunal = evasion;
    }
    double getReduccionRemdesivir() const { return reduccionRemdesivir; }
    double getEscapeVacunal() const { return escapeVacunal; }

    void mostrarAntiviral() const {
        std::cout << "    [MIX-IN 3] Remdesivir -" << std::fixed
                  << std::setprecision(0) << reduccionRemdesivir
                  << "% sensibilidad  escape vacunal "
                  << std::setprecision(0) << escapeVacunal << "%\n";
    }
};

// ============================================================================
// VARIANTE 0 - CEPA ORIGINAL (HERENCIA SIMPLE, referencia de contraste)
// ============================================================================
class CepaOriginal : public CepaBase {
public:
    CepaOriginal() : Fenotipo("ORIGINAL-SIMPLE"),
                     CepaBase("SARS-CoV-2 / Wuhan", "ORIGINAL", "China", 5, 2.60) {}

    std::string genotipado() const override {
        return "Lineage A - raiz genomica de referencia (2019)";
    }

    std::string resumenFenotipo() const override {
        return "SIN MEZCLAS (herencia simple de control)";
    }
};

// ============================================================================
// VARIANTE ALFA - HERENCIA MÚLTIPLE (base viral + 2 mixins)
// ============================================================================
class Alfa : public CepaBase, public AltaInfectividad, public TransmisionAerea {
public:
    Alfa() : Fenotipo("ALFA-MULTI"),
             CepaBase("SARS-CoV-2 Alfa B.1.1.7", "ALFA", "Reino Unido", 5, 3.40) {
        setAfinidadSpike(1.85);
        setR0(1.50);
        setPrincipalVecto("aerosol en espacios cerrados");
    }

    std::string genotipado() const override {
        return "N501Y + D614G - mutaciones en proteina spike";
    }

    std::string resumenFenotipo() const override {
        return "Infectividad ALTA + Transmision AEREA";
    }

    void mostrarFicha() const override {
        CepaBase::mostrarFicha();
        std::cout << "  Genotipo     : " << genotipado() << "\n"
                  << "  Severidad    : " << nivelSeveridad() << "\n";
        mostrarInfectividad();
        mostrarTransmision();
    }
};

// ============================================================================
// VARIANTE LAMBDA - HERENCIA MÚLTIPLE (base viral + 2 mixins)
// ============================================================================
class Lambda : public CepaBase, public TransmisionAerea, public ResistenciaAntiviral {
public:
    Lambda() : Fenotipo("LAMBDA-MULTI"),
               CepaBase("SARS-CoV-2 Lambda C.14", "LAMBDA", "Peru", 5, 0.80) {
        setR0(1.30);
        setPrincipalVecto("aerosol + contacto estrecho");
        setResistencia(12.0, 15.0);
    }

    std::string genotipado() const override {
        return "Lineage C.14 - variante de interes peruano";
    }

    std::string resumenFenotipo() const override {
        return "Transmision AEREA + Resistencia ANTIVIRAL";
    }

    void mostrarFicha() const override {
        CepaBase::mostrarFicha();
        std::cout << "  Genotipo     : " << genotipado() << "\n"
                  << "  Severidad    : " << nivelSeveridad() << "\n";
        mostrarTransmision();
        mostrarAntiviral();
    }
};

// ============================================================================
// VARIANTE DELTA - HERENCIA MÚLTIPLE (base viral + 3 mixins, rombo completo)
// ============================================================================
class Delta : public CepaBase, public AltaInfectividad,
              public TransmisionAerea, public ResistenciaAntiviral {
public:
    Delta() : Fenotipo("DELTA-MULTI"),
              CepaBase("SARS-CoV-2 Delta B.1.617.2", "DELTA", "India", 4, 3.20) {
        setAfinidadSpike(2.30);
        setR0(2.20);
        setPrincipalVecto("aerosol + eventos masivos");
        setResistencia(35.0, 40.0);
    }

    std::string genotipado() const override {
        return "L452R + T478K - escape a anticuerpos";
    }

    std::string resumenFenotipo() const override {
        return "Infectividad ALTA + Transmision AEREA + Antiviral";
    }

    void mostrarFicha() const override {
        CepaBase::mostrarFicha();
        std::cout << "  Genotipo     : " << genotipado() << "\n"
                  << "  Severidad    : " << nivelSeveridad() << "\n";
        mostrarInfectividad();
        mostrarTransmision();
        mostrarAntiviral();
    }
};

// ============================================================================
// VARIANTE OMICRON - HERENCIA MÚLTIPLE (base viral + 3 mixins)
// ============================================================================
class Omicron : public CepaBase, public AltaInfectividad,
                public TransmisionAerea, public ResistenciaAntiviral {
public:
    Omicron() : Fenotipo("OMICRON-MULTI"),
                CepaBase("SARS-CoV-2 Omicron B.1.529", "OMICRON", "Sudafrica", 3, 0.30) {
        setAfinidadSpike(4.10);
        setR0(3.50);
        setPrincipalVecto("aerosol - alta capacidad");
        setResistencia(80.0, 65.0);
    }

    std::string genotipado() const override {
        return "50+ mutaciones en spike - recombinacion";
    }

    std::string resumenFenotipo() const override {
        return "Infectividad ALTA + Transmision AEREA + Antiviral";
    }

    void mostrarFicha() const override {
        CepaBase::mostrarFicha();
        std::cout << "  Genotipo     : " << genotipado() << "\n"
                  << "  Severidad    : " << nivelSeveridad() << "\n";
        mostrarInfectividad();
        mostrarTransmision();
        mostrarAntiviral();
    }
};

// ============================================================================
// PERSONAL DE SALUD: reutilización del modelo de profesiones del ejercicio 1
// ============================================================================
class PersonalSalud {
protected:
    std::string nombre;
    std::string region;
    int turnosCubiertos;

public:
    PersonalSalud(const std::string& nom, const std::string& reg, int turnos)
        : nombre(nom), region(reg), turnosCubiertos(turnos) {}
    virtual ~PersonalSalud() = default;

    virtual std::string obtenerRol() const = 0;
    virtual std::string obtenerAccion() const = 0;
    virtual void asignarRespuesta(const CepaBase& c) const = 0;

    void mostrarAsignacion(const CepaBase& c) const {
        std::cout << "  " << std::left << std::setw(14) << obtenerRol()
                  << std::setw(21) << nombre
                  << std::setw(7) << region
                  << std::right << std::setw(5) << turnosCubiertos
                  << "  " << obtenerAccion() << "\n";
        asignarRespuesta(c);
    }
};

class Medico : public PersonalSalud {
private:
    std::string especialidad;

public:
    Medico(const std::string& nom, const std::string& reg, int turnos,
           const std::string& esp)
        : PersonalSalud(nom, reg, turnos), especialidad(esp) {}

    std::string obtenerRol() const override { return "MEDICO"; }
    std::string obtenerAccion() const override { return "Esquema terapeutico"; }

    void asignarRespuesta(const CepaBase& c) const override {
        std::cout << "      (IFR de la variante: " << std::fixed
                  << std::setprecision(2) << c.getLetalidad() << "%)\n";
    }
};

class Enfermera : public PersonalSalud {
public:
    Enfermera(const std::string& nom, const std::string& reg, int turnos)
        : PersonalSalud(nom, reg, turnos) {}

    std::string obtenerRol() const override { return "ENFERMERA"; }
    std::string obtenerAccion() const override { return "Aislamiento y constantes"; }

    void asignarRespuesta(const CepaBase& c) const override {
        std::cout << "      (nivel de severidad: " << c.nivelSeveridad() << ")\n";
    }
};

class Investigador : public PersonalSalud {
public:
    Investigador(const std::string& nom, const std::string& reg, int turnos)
        : PersonalSalud(nom, reg, turnos) {}

    std::string obtenerRol() const override { return "INVESTIGADOR"; }
    std::string obtenerAccion() const override { return "Secuenciamiento genomico"; }

    void asignarRespuesta(const CepaBase& c) const override {
        std::cout << "      (linaje: " << c.getLinaje() << ")\n";
    }
};

// ============================================================================
// FUNCIÓN AUXILIAR: reporte polimórfico del comité clínico
// ============================================================================
void imprimirComite(const CepaBase& c, const std::vector<PersonalSalud*>& equipo) {
    std::cout << "\n  Plan de respuesta: " << c.getNombre()
              << "  [" << c.nivelSeveridad() << "]\n";
    std::cout << "  " << std::left << std::setw(14) << "ROL"
              << std::setw(21) << "RESPONSABLE"
              << std::setw(7) << "SEDE"
              << std::right << std::setw(5) << "TURN"
              << "  " << "ACCION" << "\n";
    for (const PersonalSalud* p : equipo) {
        p->mostrarAsignacion(c);
    }
}

int main() {
    std::cout << "=========================================================\n";
    std::cout << "  TECNOLOGÍA DE OBJETOS - LAB 03 | EJERCICIO 02\n";
    std::cout << "  HERENCIA MULTIPLE: CEPAS DEL CORONAVIRUS\n";
    std::cout << "  Estudiante: Franco Jesus Cahua Soto\n";
    std::cout << "=========================================================\n";

    // 1. Constructores de las variantes: la clase MÁS DERIVADA inicializa la
    //    base virtual Fenotipo una única vez, resolviendo el rombo.
    CepaOriginal* original = new CepaOriginal();
    Alfa* alfa = new Alfa();
    Lambda* lambda = new Lambda();
    Delta* delta = new Delta();
    Omicron* omicron = new Omicron();

    std::cout << "\n>>> 1. FICHAS TECNICAS POR HERENCIA MULTIPLE\n";

    std::cout << "\n[ORIGINAL] CepaBase (herencia simple de control)\n";
    original->mostrarFicha();
    std::cout << "  Genotipo     : " << original->genotipado() << "\n"
              << "  Severidad    : " << original->nivelSeveridad() << "\n";

    std::cout << "\n[ALFA] CepaBase + AltaInfectividad + TransmisionAerea\n";
    alfa->mostrarFicha();

    std::cout << "\n[LAMBDA] CepaBase + TransmisionAerea + ResistenciaAntiviral\n";
    lambda->mostrarFicha();

    std::cout << "\n[DELTA] CepaBase + 3 mix-ins (rombo completo)\n";
    delta->mostrarFicha();

    std::cout << "\n[OMICRON] CepaBase + 3 mix-ins (rombo completo)\n";
    omicron->mostrarFicha();

    // 2. Polimorfismo a través de la clase base: una sola llamada, 5 cuerpos
    std::vector<CepaBase*> variantes;
    variantes.push_back(original);
    variantes.push_back(alfa);
    variantes.push_back(lambda);
    variantes.push_back(delta);
    variantes.push_back(omicron);

    std::cout << "\n>>> 2. POLIMORFISMO: MISMA INTERFAZ, 5 COMPORTAMIENTOS\n";
    std::cout << "  " << std::left << std::setw(12) << "LINAJE"
              << std::right << std::setw(8) << "INCUB"
              << std::setw(10) << "IFR(%)"
              << std::setw(8) << "R0"
              << std::setw(13) << "RESIST.REMD." << "\n";
    std::cout << "  " << "-------------------------------------------------------------\n";
    for (CepaBase* v : variantes) {
        std::cout << "  " << std::left << std::setw(12) << v->getLinaje()
                  << std::right << std::setw(8) << v->getIncubacion() << "d";
        std::cout << std::setw(10) << std::fixed << std::setprecision(2)
                  << v->getLetalidad();
        const TransmisionAerea* tr = dynamic_cast<const TransmisionAerea*>(v);
        if (tr) {
            std::cout << std::setw(8) << tr->getR0();
        } else {
            std::cout << std::setw(8) << "n/d";
        }
        const ResistenciaAntiviral* res =
            dynamic_cast<const ResistenciaAntiviral*>(v);
        if (res) {
            std::cout << std::setw(13) << "-" << res->getReduccionRemdesivir() << "%";
        } else {
            std::cout << std::setw(13) << "n/d";
        }
        std::cout << "\n";
    }

    // 3. Comprobación del rombo: `dynamic_cast` identifica el tipo dinámico
    //    real cuando la variable se manipula como puntero a la clase base.
    std::cout << "\n>>> 3. RTTI: IDENTIFICACION DE FENOTIPOS (dynamic_cast)\n";
    for (CepaBase* v : variantes) {
        std::cout << "  " << std::left << std::setw(10) << v->getLinaje() << " -> ";
        if (dynamic_cast<AltaInfectividad*>(v) != nullptr)
            std::cout << "Infectividad ";
        if (dynamic_cast<TransmisionAerea*>(v) != nullptr)
            std::cout << "Transmision ";
        if (dynamic_cast<ResistenciaAntiviral*>(v) != nullptr)
            std::cout << "Antiviral ";
        std::cout << "| raiz: " << v->getCodigo() << "\n";
    }

    // 4. Reutilización del modelo de personal del ejercicio 1
    std::vector<PersonalSalud*> equipo;
    equipo.push_back(new Medico("Dra. Elena Aguilar", "Lima", 8, "Neumologia"));
    equipo.push_back(new Enfermera("Sr. Luis Ramos", "Lima", 12));
    equipo.push_back(new Investigador("Dr. Aldo Medina", "Cusco", 6));

    std::cout << "\n>>> 4. COMITE DE RESPUESTA CLINICA (polimorfismo combinado)\n";
    imprimirComite(*omicron, equipo);
    imprimirComite(*alfa, equipo);

    // 5. Liberación de memoria dinámica
    for (CepaBase* v : variantes) delete v;
    for (PersonalSalud* p : equipo) delete p;
    std::cout << "\n>>> 5. MEMORIA DINAMICA LIBERADA: 5 variantes y 3 agentes.\n";

    return 0;
}
