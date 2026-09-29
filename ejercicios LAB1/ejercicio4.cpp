/**
 * ============================================================================
 * ASIGNATURA: TECNOLOGÍA DE OBJETOS
 * PRÁCTICA 01: C++ estructuras, clases y objetos
 * ESTUDIANTE: Franco Jesus Cahua Soto
 * ARCHIVO: ejercicio4.cpp
 * DESCRIPCIÓN: Implementación en C++ del modelo de Cita Médica de un Hospital
 *              utilizando una ESTRUCTURA (struct) con más de 02 métodos:
 *              1. inicializar(...)
 *              2. calcularCostoNeto(double porcentajeCobertura)
 *              3. mostrarTicketAtencion()
 *              4. reprogramarCita(std::string nuevaFecha, std::string nuevaHora)
 *              5. confirmarAtencion()
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

struct CitaMedica {
    // Atributos de la estructura
    int idCita;
    std::string numeroAsegurado;
    std::string nombrePaciente;
    std::string especialidad;
    std::string medicoAsignado;
    std::string fecha;
    std::string hora;
    std::string tipoAtencion; // Ej: "Consulta Externa", "Teleconsulta", "Emergencia"
    double costoBase;
    double costoFinal;
    std::string estado;       // "PENDIENTE", "CONFIRMADA", "ATENDIDA", "CANCELADA"

    // Método 1: Inicialización de los datos de la cita médica
    void inicializar(int id, std::string numAsegurado, std::string paciente,
                     std::string esp, std::string medico, std::string f,
                     std::string h, std::string tipo, double costo) {
        idCita = id;
        numeroAsegurado = numAsegurado;
        nombrePaciente = paciente;
        especialidad = esp;
        medicoAsignado = medico;
        fecha = f;
        hora = h;
        tipoAtencion = tipo;
        costoBase = costo;
        costoFinal = costo;
        estado = "PENDIENTE";
    }

    // Método 2: Cálculo del costo neto aplicando porcentaje de cobertura del seguro (0.0 a 1.0)
    double calcularCostoNeto(double porcentajeCobertura) {
        if (porcentajeCobertura < 0.0) porcentajeCobertura = 0.0;
        if (porcentajeCobertura > 1.0) porcentajeCobertura = 1.0;
        double descuento = costoBase * porcentajeCobertura;
        costoFinal = costoBase - descuento;
        return costoFinal;
    }

    // Método 3: Impresión formateada del ticket de atención médica
    void mostrarTicketAtencion() const {
        std::cout << "\n+--------------------------------------------------------------+\n";
        std::cout << "|              HOSPITAL NACIONAL - TICKET DE CITA              |\n";
        std::cout << "+--------------------------------------------------------------+\n";
        std::cout << "| ID Cita:            #" << std::left << std::setw(39) << idCita << "|\n";
        std::cout << "| Nro. Asegurado:     " << std::left << std::setw(40) << numeroAsegurado << "|\n";
        std::cout << "| Paciente:           " << std::left << std::setw(40) << nombrePaciente << "|\n";
        std::cout << "| Especialidad:       " << std::left << std::setw(40) << especialidad << "|\n";
        std::cout << "| Medico:             " << std::left << std::setw(40) << medicoAsignado << "|\n";
        std::cout << "| Fecha y Hora:       " << std::left << std::setw(40) << (fecha + " - " + hora) << "|\n";
        std::cout << "| Tipo de Atencion:   " << std::left << std::setw(40) << tipoAtencion << "|\n";
        std::cout << "| Estado de Cita:     " << std::left << std::setw(40) << estado << "|\n";
        std::cout << "+--------------------------------------------------------------+\n";
        std::cout << "| Costo Base:         S/. " << std::right << std::setw(8) << std::fixed << std::setprecision(2) << costoBase << std::setw(28) << " " << "|\n";
        std::cout << "| Costo a Pagar:      S/. " << std::right << std::setw(8) << std::fixed << std::setprecision(2) << costoFinal << std::setw(28) << " " << "|\n";
        std::cout << "+--------------------------------------------------------------+\n";
    }

    // Método 4: Reprogramar fecha y hora de la cita
    void reprogramarCita(const std::string& nuevaFecha, const std::string& nuevaHora) {
        fecha = nuevaFecha;
        hora = nuevaHora;
        std::cout << "[INFO] Cita #" << idCita << " reprogramada exitosamente para " << fecha << " a las " << hora << ".\n";
    }

    // Método 5: Confirmar la atención médica
    void confirmarAtencion() {
        estado = "CONFIRMADA";
        std::cout << "[INFO] Cita #" << idCita << " del paciente " << nombrePaciente << " ha sido CONFIRMADA.\n";
    }
};

int main() {
    std::cout << "=========================================================\n";
    std::cout << "  TECNOLOGÍA DE OBJETOS - LAB 01 | EJERCICIO 04\n";
    std::cout << "  Implementacion con ESTRUCTURA (struct)\n";
    std::cout << "  Estudiante: Franco Jesus Cahua Soto\n";
    std::cout << "=========================================================\n";

    // Creación e inicialización de 3 citas médicas usando struct
    CitaMedica cita1, cita2;

    cita1.inicializar(101, "ASEG-849201", "Franco Cahua", "Cardiologia",
                      "Dr. Roberto Valdivia", "15/10/2026", "09:30", "Consulta Externa", 120.00);

    cita2.inicializar(102, "ASEG-773190", "Maria Gomez Perez", "Traumatologia",
                      "Dra. Claudia Paredes", "16/10/2026", "11:00", "Emergencia", 180.00);

    // Aplicar coberturas de seguro (Método 2)
    cita1.calcularCostoNeto(0.80); // Cobertura del 80% (seguro regular)
    cita2.calcularCostoNeto(1.00); // Cobertura del 100% (SIS o cobertura total)

    // Mostrar tickets originales (Método 3)
    std::cout << "\n>>> MOSTRANDO CITAS INICIALMENTE REGISTRADAS:";
    cita1.mostrarTicketAtencion();
    cita2.mostrarTicketAtencion();

    // Invocar método 5: Confirmar atención
    std::cout << "\n>>> ACTUALIZANDO ESTADO:";
    cita1.confirmarAtencion();

    // Invocar método 4: Reprogramar cita
    std::cout << "\n>>> REPROGRAMANDO CITA:";
    cita2.reprogramarCita("20/10/2026", "15:00");

    // Mostrar ticket actualizado de cita 2
    std::cout << "\n>>> TICKET TRAS REPROGRAMACIÓN:";
    cita2.mostrarTicketAtencion();

    return 0;
}
