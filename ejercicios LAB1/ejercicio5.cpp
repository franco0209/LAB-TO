/**
 * ============================================================================
 * ASIGNATURA: TECNOLOGÍA DE OBJETOS
 * PRÁCTICA 01: C++ estructuras, clases y objetos
 * ESTUDIANTE: Franco Jesus Cahua Soto
 * ARCHIVO: ejercicio5.cpp
 * DESCRIPCIÓN: Implementación en C++ del modelo de Cita Médica de un Hospital
 *              utilizando una CLASE (class) con principios de POO:
 *              - Encapsulamiento (atributos privados y métodos públicos)
 *              - Constructor parametrizado y por defecto
 *              - Getters y Setters con validación
 *              - Métodos de negocio: calcularCostoFinal con seguro,
 *                reprogramación, cancelación, verificación de urgencia
 *                e impresión formateada de comprobante hospitalario.
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

enum class TipoSeguro {
    PARTICULAR, // 0% de cobertura
    ESSALUD,    // 70% de cobertura
    SIS,        // 100% de cobertura
    PRIVADO_EPS // 85% de cobertura
};

std::string tipoSeguroToString(TipoSeguro seguro) {
    switch (seguro) {
        case TipoSeguro::PARTICULAR:  return "Particular (Sin Seguro)";
        case TipoSeguro::ESSALUD:     return "EsSalud (70% cobertura)";
        case TipoSeguro::SIS:         return "SIS Integral (100% cobertura)";
        case TipoSeguro::PRIVADO_EPS: return "EPS Privado (85% cobertura)";
        default:                      return "Desconocido";
    }
}

class CitaMedicaHospital {
private:
    int idCita;
    std::string numeroAsegurado;
    std::string paciente;
    std::string especialidad;
    std::string medicoAsignado;
    std::string fecha;
    std::string hora;
    std::string tipoAtencion; // "Consulta Externa", "Urgencia", "Cirugia Menor"
    TipoSeguro tipoSeguro;
    double costoBase;
    double costoFinal;
    std::string estado; // "REGISTRADA", "CONFIRMADA", "ATENDIDA", "CANCELADA"
    std::string motivoCancelacion;

public:
    // Constructor por defecto
    CitaMedicaHospital()
        : idCita(0), numeroAsegurado("N/A"), paciente("N/A"),
          especialidad("Medicina General"), medicoAsignado("Por Asignar"),
          fecha("01/01/2026"), hora("08:00"), tipoAtencion("Consulta Externa"),
          tipoSeguro(TipoSeguro::PARTICULAR), costoBase(50.0), costoFinal(50.0),
          estado("REGISTRADA"), motivoCancelacion("") {}

    // Constructor parametrizado
    CitaMedicaHospital(int id, const std::string& numAsegurado, const std::string& nomPaciente,
                       const std::string& esp, const std::string& medico,
                       const std::string& f, const std::string& h,
                       const std::string& tipoAt, TipoSeguro seguro, double costo)
        : idCita(id), numeroAsegurado(numAsegurado), paciente(nomPaciente),
          especialidad(esp), medicoAsignado(medico), fecha(f), hora(h),
          tipoAtencion(tipoAt), tipoSeguro(seguro), costoBase(costo),
          estado("REGISTRADA"), motivoCancelacion("") {
        calcularCostoFinal();
    }

    // Getters y Setters
    int getIdCita() const { return idCita; }
    std::string getPaciente() const { return paciente; }
    std::string getEspecialidad() const { return especialidad; }
    std::string getEstado() const { return estado; }
    double getCostoFinal() const { return costoFinal; }

    void setMedicoAsignado(const std::string& nuevoMedico) {
        medicoAsignado = nuevoMedico;
    }

    // Método de negocio 1: Cálculo del costo final según cobertura de seguro
    double calcularCostoFinal() {
        double factorCobertura = 0.0;
        switch (tipoSeguro) {
            case TipoSeguro::PARTICULAR:  factorCobertura = 0.00; break;
            case TipoSeguro::ESSALUD:     factorCobertura = 0.70; break;
            case TipoSeguro::SIS:         factorCobertura = 1.00; break;
            case TipoSeguro::PRIVADO_EPS: factorCobertura = 0.85; break;
        }
        costoFinal = costoBase * (1.0 - factorCobertura);
        return costoFinal;
    }

    // Método de negocio 2: Reprogramación con validación de estado
    bool reprogramarCita(const std::string& nuevaFecha, const std::string& nuevaHora) {
        if (estado == "CANCELADA" || estado == "ATENDIDA") {
            std::cout << "[ERROR] No se puede reprogramar una cita con estado: " << estado << "\n";
            return false;
        }
        fecha = nuevaFecha;
        hora = nuevaHora;
        estado = "REPROGRAMADA";
        std::cout << "[EXITO] Cita #" << idCita << " reprogramada satisfactoriamente para "
                  << fecha << " a las " << hora << ".\n";
        return true;
    }

    // Método de negocio 3: Cancelación de la cita con motivo
    bool cancelarCita(const std::string& motivo) {
        if (estado == "ATENDIDA") {
            std::cout << "[ERROR] Una cita ya atendida no puede ser cancelada.\n";
            return false;
        }
        estado = "CANCELADA";
        motivoCancelacion = motivo;
        costoFinal = 0.0; // Se anula el cobro
        std::cout << "[INFO] Cita #" << idCita << " cancelada. Motivo: " << motivo << "\n";
        return true;
    }

    // Método de negocio 4: Evaluación de prioridad o urgencia
    bool esAtencionUrgente() const {
        return (tipoAtencion == "Urgencia" || tipoAtencion == "Emergencia");
    }

    // Método de negocio 5: Impresión estructurada de boleta de atención
    void imprimirBoletaAtencion() const {
        std::cout << "\n=================================================================\n";
        std::cout << "                 SISTEMA DE GESTION HOSPITALARIA                 \n";
        std::cout << "               BOLETA DE ATENCION DE CITA MEDICA                 \n";
        std::cout << "=================================================================\n";
        std::cout << "  Numero de Cita:       #" << idCita << "\n";
        std::cout << "  Codigo Asegurado:     " << numeroAsegurado << "\n";
        std::cout << "  Paciente:             " << paciente << "\n";
        std::cout << "  Tipo de Seguro:       " << tipoSeguroToString(tipoSeguro) << "\n";
        std::cout << "  Especialidad:         " << especialidad << "\n";
        std::cout << "  Medico Tratante:      " << medicoAsignado << "\n";
        std::cout << "  Fecha programada:     " << fecha << "  |  Hora: " << hora << "\n";
        std::cout << "  Tipo de Servicio:     " << tipoAtencion
                  << (esAtencionUrgente() ? " [PRIORIDAD ALTA]" : " [REGULAR]") << "\n";
        std::cout << "  Estado Actual:        " << estado << "\n";
        if (!motivoCancelacion.empty()) {
            std::cout << "  Motivo Cancelacion:   " << motivoCancelacion << "\n";
        }
        std::cout << "-----------------------------------------------------------------\n";
        std::cout << "  Costo Arancelario:    S/. " << std::fixed << std::setprecision(2) << costoBase << "\n";
        std::cout << "  Descuento Cobertura:  S/. " << std::fixed << std::setprecision(2) << (costoBase - costoFinal) << "\n";
        std::cout << "  TOTAL A COBRAR:       S/. " << std::fixed << std::setprecision(2) << costoFinal << "\n";
        std::cout << "=================================================================\n";
    }
};

int main() {
    std::cout << "=========================================================\n";
    std::cout << "  TECNOLOGÍA DE OBJETOS - LAB 01 | EJERCICIO 05\n";
    std::cout << "  Implementacion con CLASES (class) Y POO\n";
    std::cout << "  Estudiante: Franco Jesus Cahua Soto\n";
    std::cout << "=========================================================\n";

    // Instanciación de citas con diferentes pólizas de seguro
    CitaMedicaHospital cita1(201, "00481239-01", "Franco Jesus Cahua Soto",
                             "Neurologia", "Dr. Victor Zegarra",
                             "12/10/2026", "10:15", "Consulta Externa",
                             TipoSeguro::PRIVADO_EPS, 150.00);

    CitaMedicaHospital cita2(202, "00912445-00", "Elena Morales Quiroz",
                             "Cardiologia", "Dra. Patricia Aguilar",
                             "12/10/2026", "11:45", "Urgencia",
                             TipoSeguro::SIS, 200.00);

    CitaMedicaHospital cita3(203, "00184491-03", "Carlos Benavente Ramos",
                             "Oftalmologia", "Dr. Luis Chavez",
                             "14/10/2026", "16:00", "Consulta Externa",
                             TipoSeguro::PARTICULAR, 90.00);

    // Presentar las boletas de las 3 citas
    std::cout << "\n>>> LISTADO DE CITAS REGISTRADAS:";
    cita1.imprimirBoletaAtencion();
    cita2.imprimirBoletaAtencion();
    cita3.imprimirBoletaAtencion();

    // Demostrar operaciones y métodos de negocio
    std::cout << "\n>>> OPERACIONES SOBRE OBJETOS:\n";
    std::cout << "- Reprogramando cita de Franco Jesus Cahua Soto:\n";
    cita1.reprogramarCita("19/10/2026", "16:30");
    cita1.imprimirBoletaAtencion();

    std::cout << "- Cancelando cita particular por fuerza mayor:\n";
    cita3.cancelarCita("Paciente solicito cancelacion por viaje");
    cita3.imprimirBoletaAtencion();

    return 0;
}
