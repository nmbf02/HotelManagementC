#include "room_manager.h"
#include "utils.h"
#include <iostream>

void registrar_habitacion(int numero, const std::string& tipo, double precio) {
    try {
        pqxx::connection conn = connect_to_db();
        pqxx::work txn(conn);

        std::string query = "INSERT INTO habitaciones (numero, tipo, precio) VALUES (" +
                            txn.quote(numero) + ", " +
                            txn.quote(tipo) + ", " +
                            txn.quote(precio) + ");";

        txn.exec(query);
        txn.commit();
        std::cout << "Habitacion registrada exitosamente." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error al registrar habitacion: " << e.what() << std::endl;
    }
}

void consultar_habitaciones() {
    try {
        pqxx::connection conn = connect_to_db();
        pqxx::work txn(conn);

        // Consulta SQL para obtener habitaciones
        std::string query = "SELECT id, numero, tipo, precio FROM habitaciones;";
        pqxx::result result = txn.exec(query);

        // Mostrar las habitaciones en la consola
        std::cout << "---------------------------------------------------" << std::endl;
        std::cout << "                  Lista de Habitaciones            " << std::endl;
        std::cout << "---------------------------------------------------" << std::endl;
        for (const auto& row : result) {
            std::cout << "ID: " << row["id"].as<int>()
                      << ", Numero: " << row["numero"].as<int>()
                      << ", Tipo: " << row["tipo"].as<std::string>()
                      << ", Precio: " << row["precio"].as<double>() << " USD" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error al consultar habitaciones: " << e.what() << std::endl;
    }
}