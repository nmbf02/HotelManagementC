#include "reservation_manager.h"
#include "../include/utils.h"
#include <iostream>

void registrar_reserva(int cliente_id, int habitacion_id, const std::string& fecha_inicio, const std::string& fecha_fin) {
    try {
        pqxx::connection conn = connect_to_db();
        pqxx::work txn(conn);

        // Consulta SQL para registrar una reserva
        std::string query = "INSERT INTO reservas (cliente_id, habitacion_id, fecha_inicio, fecha_fin) VALUES (" +
                            txn.quote(cliente_id) + ", " +
                            txn.quote(habitacion_id) + ", " +
                            txn.quote(fecha_inicio) + ", " +
                            txn.quote(fecha_fin) + ");";

        txn.exec(query);
        txn.commit();
        std::cout << "Reserva registrada exitosamente." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error al registrar la reserva: " << e.what() << std::endl;
    }
}

void consultar_reservas() {
    try {
        pqxx::connection conn = connect_to_db(); // Conexión a la base de datos
        pqxx::work txn(conn);

        // Consulta SQL para obtener las reservas
        std::string query = R"(
            SELECT r.id, c.nombre AS cliente, h.numero AS habitacion, r.fecha_inicio, r.fecha_fin
            FROM reservas r
            JOIN clientes c ON r.cliente_id = c.id
            JOIN habitaciones h ON r.habitacion_id = h.id;
        )";
        pqxx::result result = txn.exec(query);

        // Mostrar las reservas en la consola
        std::cout << "---------------------------------------------------" << std::endl;
        std::cout << "                     Lista de reservas             " << std::endl;
        std::cout << "---------------------------------------------------" << std::endl;
        for (const auto& row : result) {
            std::cout << "ID: " << row["id"].as<int>()
                      << ", Cliente: " << row["cliente"].as<std::string>()
                      << ", Habitacion: " << row["habitacion"].as<int>()
                      << ", Fecha Inicio: " << row["fecha_inicio"].as<std::string>()
                      << ", Fecha Fin: " << row["fecha_fin"].as<std::string>() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error al consultar reservas: " << e.what() << std::endl;
    }
}

void consultar_reservas_del_dia_actual() {
    try {
        pqxx::connection conn = connect_to_db(); // Conexión a la base de datos
        pqxx::work txn(conn);

        // Consulta SQL para obtener las reservas activas del día actual
        std::string query = R"(
            SELECT r.id, c.nombre AS cliente, h.numero AS habitacion, r.fecha_inicio, r.fecha_fin
            FROM reservas r
            JOIN clientes c ON r.cliente_id = c.id
            JOIN habitaciones h ON r.habitacion_id = h.id
            WHERE CURRENT_DATE BETWEEN r.fecha_inicio AND r.fecha_fin;
        )";
        pqxx::result result = txn.exec(query);

        // Mostrar las reservas del día actual
        std::cout << "---------------------------------------------------" << std::endl;
        std::cout << "           Reservas Activas del Dia Actual         " << std::endl;
        std::cout << "---------------------------------------------------" << std::endl;
        for (const auto& row : result) {
            std::cout << "Reserva ID: " << row["id"].as<int>()
                      << ", Cliente: " << row["cliente"].as<std::string>()
                      << ", Habitacion: " << row["habitacion"].as<int>()
                      << ", Fecha Inicio: " << row["fecha_inicio"].as<std::string>()
                      << ", Fecha Fin: " << row["fecha_fin"].as<std::string>() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error al consultar reservas del dia actual: " << e.what() << std::endl;
    }
}