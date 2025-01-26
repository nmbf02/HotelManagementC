#include "utils.h"
#include <pqxx/pqxx>
#include <iostream>

pqxx::connection connect_to_db() {
    try {
        // Cambia los valores según tu configuración de PostgreSQL
        std::string connection_str = "host=localhost port=5432 dbname=hotel_management_c user=postgres password=0211";
        pqxx::connection conn(connection_str);
        if (conn.is_open()) {
            std::cout << "Conexión a la base de datos exitosa." << std::endl;
        } else {
            throw std::runtime_error("No se pudo conectar a la base de datos.");
        }
        return conn;
    } catch (const std::exception& e) {
        std::cerr << "Error al conectar a la base de datos: " << e.what() << std::endl;
        throw;
    }
}
