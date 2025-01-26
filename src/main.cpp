#include "client_manager.h"
#include "room_manager.h"
#include "client_manager.h"
#include "report_manager.h"
#include "reservation_manager.h"
#include <iostream>
#include <thread>
#include <string>

void mostrar_menu() {
    std::cout << " " << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    std::cout << "     Sistema de Gestion de Hoteles    " << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    std::cout << " " << std::endl;
    std::cout << "1. Registrar Cliente" << std::endl;
    std::cout << "2. Registrar Habitacion" << std::endl;
    std::cout << "3. Registrar Reserva" << std::endl;
    std::cout << "4. Consultar Clientes" << std::endl;
    std::cout << "5. Consultar Habitaciones" << std::endl;
    std::cout << "6. Consultar Reservas" << std::endl;
    std::cout << "7. Consultar Clientes con Reservas Hoy" << std::endl;
    std::cout << "8. Generar Reporte en PDF" << std::endl;
    std::cout << "9. Salir" << std::endl;
    std::cout << "Seleccione una opcion: ";
    std::cout << " " << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
}

int main() {
    int opcion;
    do {
        mostrar_menu();
        std::cin >> opcion;
        std::cin.ignore(); // Limpia el buffer después de leer la opción

        switch (opcion) {
            case 1: {
                // Registrar cliente
                std::cout << " " << std::endl;
                std::cout << "---------------------------------------------------" << std::endl;
                std::cout << " " << std::endl;
                std::string nombre, correo, telefono;
                std::cout << "Ingrese el nombre del cliente: ";
                std::getline(std::cin, nombre);
                std::cout << "Ingrese el correo del cliente: ";
                std::getline(std::cin, correo);
                std::cout << "Ingrese el telefono del cliente: ";
                std::getline(std::cin, telefono);
                std::cout << " " << std::endl;
                std::cout << "---------------------------------------------------" << std::endl;
                std::cout << " " << std::endl;
                registrar_cliente(nombre, correo, telefono);
                
                break;
            }
            case 2: {
                // Registrar habitación
                std::cout << " " << std::endl;
                std::cout << "---------------------------------------------------" << std::endl;
                std::cout << " " << std::endl;
                int numero;
                std::string tipo;
                double precio;
                std::cout << "Ingrese el numero de la habitacion: ";
                std::cin >> numero;
                std::cin.ignore();
                std::cout << "Ingrese el tipo de habitacion (Ej: Doble, Suite): ";
                std::getline(std::cin, tipo);
                std::cout << "Ingrese el precio por noche: ";
                std::cin >> precio;
                std::cout << " " << std::endl;
                std::cout << "---------------------------------------------------" << std::endl;
                std::cout << " " << std::endl;
                registrar_habitacion(numero, tipo, precio);
                break;
            }
            case 3: {
                // Registrar reserva
                std::cout << " " << std::endl;
                std::cout << "---------------------------------------------------" << std::endl;
                std::cout << " " << std::endl;
                int cliente_id, habitacion_id;
                std::string fecha_inicio, fecha_fin;
                std::cout << "Ingrese el ID del cliente: ";
                std::cin >> cliente_id;
                std::cout << "Ingrese el ID de la habitacion: ";
                std::cin >> habitacion_id;
                std::cin.ignore();
                std::cout << "Ingrese la fecha de inicio (YYYY-MM-DD): ";
                std::getline(std::cin, fecha_inicio);
                std::cout << "Ingrese la fecha de fin (YYYY-MM-DD): ";
                std::getline(std::cin, fecha_fin);
                registrar_reserva(cliente_id, habitacion_id, fecha_inicio, fecha_fin);
                std::cout << " " << std::endl;
                std::cout << "---------------------------------------------------" << std::endl;
                std::cout << " " << std::endl;
                break;
            }
            case 4:
                // Consultar clientes
                consultar_clientes();
                break;
            case 5:
                // Consultar habitaciones
                consultar_habitaciones();
                break;
            case 6:
                // Consultar reservas
                consultar_reservas();
                break;
            case 7:
                // Consultar clientes con reservas hoy
                consultar_reservas_del_dia_actual();
                break;
            case 8:
                // Generar reporte en PDF
                generar_reporte("reporte_hotel.pdf");
                break;
            case 9:
                std::cout << "Saliendo del sistema..." << std::endl;
                break;
            default:
                std::cout << "Opcion invalida. Intente de nuevo." << std::endl;
        }
    } while (opcion != 9);

    return 0;
}
