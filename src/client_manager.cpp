#include "client_manager.h"
#include "utils.h"  // Para la conexión a la base de datos
#include <fstream>  // Necesario para trabajar con archivos
#include <iostream>
#include <thread>   // Para manejar los hilos
#include <chrono>   // Para obtener la fecha y hora actual
#include <string>   // Para trabajar con cadenas

// Función para mostrar el hilo que está ejecutando el proceso
void mostrar_hilo(const std::string& proceso) {
    std::cout << "Hilo " << std::this_thread::get_id() << " ejecutando proceso: " << proceso << std::endl;
}

// Validar datos del cliente
void validar_datos(const std::string& nombre, const std::string& correo, const std::string& telefono, bool& datos_validos) {
    mostrar_hilo("Validar datos del cliente");
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Simular trabajo
    // Validación de datos
    if (nombre.empty() || correo.empty() || telefono.empty()) {
        datos_validos = false;
        std::cerr << "Error: Los datos del cliente no son válidos." << std::endl;
    } else {
        datos_validos = true;
        std::cout << "Datos válidos: Nombre: " << nombre << ", Correo: " << correo << ", Teléfono: " << telefono << std::endl;
    }
}

// Verificar si el cliente ya existe en la base de datos
void verificar_existencia_en_db(const std::string& correo, bool& cliente_existe) {
    mostrar_hilo("Verificar existencia en la base de datos");
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Simular trabajo
    try {
        pqxx::connection conn = connect_to_db(); // Usa la conexión definida en utils.h
        pqxx::work txn(conn);

        std::string query = "SELECT COUNT(*) AS count FROM clientes WHERE correo = " + txn.quote(correo) + ";";
        pqxx::result result = txn.exec(query);

        cliente_existe = result[0]["count"].as<int>() > 0;
        if (cliente_existe) {
            std::cout << "El cliente con correo " << correo << " ya existe en la base de datos." << std::endl;
        } else {
            std::cout << "El cliente con correo " << correo << " no existe en la base de datos." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error al verificar existencia en la base de datos: " << e.what() << std::endl;
    }
}

// Insertar cliente en la base de datos
void insertar_cliente_en_db(const std::string& nombre, const std::string& correo, const std::string& telefono) {
    mostrar_hilo("Insertar cliente en la base de datos");
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Simular trabajo
    try {
        pqxx::connection conn = connect_to_db(); // Usa la conexión definida en utils.h
        pqxx::work txn(conn);

        std::string query = "INSERT INTO clientes (nombre, correo, telefono) VALUES (" +
                            txn.quote(nombre) + ", " +
                            txn.quote(correo) + ", " +
                            txn.quote(telefono) + ");";

        txn.exec(query);
        txn.commit();
        std::cout << "Cliente insertado en la base de datos: Nombre: " << nombre << ", Correo: " << correo
                  << ", Teléfono: " << telefono << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error al insertar cliente en la base de datos: " << e.what() << std::endl;
    }
}

// Registrar logs del evento
void registrar_logs(const std::string& evento, const std::string& nombre, const std::string& correo, const std::string& telefono) {
    mostrar_hilo("Registrar logs");
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Simular trabajo
    std::ofstream archivo_logs("C:/msys64/home/natha/HotelManagementC/logs/registro_eventos.txt", std::ios::app);

    if (archivo_logs.is_open()) {
        auto tiempo_actual = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        archivo_logs << std::ctime(&tiempo_actual) << ": " << evento << std::endl;
        archivo_logs << "Cliente: Nombre: " << nombre << ", Correo: " << correo << ", Teléfono: " << telefono << std::endl;
        archivo_logs << "----------------------------------------" << std::endl;
        std::cout << "Log registrado: " << evento << std::endl;
    } else {
        std::cerr << "No se pudo abrir el archivo para registrar el log." << std::endl;
    }

    archivo_logs.close();
}

// Registrar cliente con procesos en paralelo
void registrar_cliente(const std::string& nombre, const std::string& correo, const std::string& telefono) {
    bool datos_validos = false;
    bool cliente_existe = false;

    // Crear hilos para la validación y verificación en paralelo
    std::thread hilo_validacion(validar_datos, std::ref(nombre), std::ref(correo), std::ref(telefono), std::ref(datos_validos));
    std::thread hilo_verificacion(verificar_existencia_en_db, std::ref(correo), std::ref(cliente_existe));

    // Esperar a que ambos hilos terminen
    hilo_validacion.join();
    hilo_verificacion.join();

    // Procesar el resultado de las validaciones y verificación
    if (datos_validos && !cliente_existe) {
        // Crear hilos para la inserción de cliente y registro de logs
        std::thread hilo_insercion(insertar_cliente_en_db, nombre, correo, telefono);

        // Llamar a registrar_logs en paralelo con los datos del cliente
        std::thread hilo_logs(registrar_logs, "Cliente registrado con éxito", std::ref(nombre), std::ref(correo), std::ref(telefono));

        // Esperar a que ambos hilos terminen
        hilo_insercion.join();
        hilo_logs.detach();  // Desprender el hilo de logs para que se ejecute en paralelo

        std::cout << "Proceso de registro completado con éxito." << std::endl;
    } else if (!datos_validos) {
        std::cerr << "El registro del cliente falló debido a datos inválidos." << std::endl;
    } else if (cliente_existe) {
        std::cerr << "El cliente ya existe en la base de datos. No se realizó el registro." << std::endl;
    }
}

// Consultar clientes
void consultar_clientes() {
    try {
        pqxx::connection conn = connect_to_db(); // Usa la conexión definida en utils.h
        pqxx::work txn(conn);

        // Consulta SQL para obtener todos los clientes
        std::string query = "SELECT id, nombre, correo, telefono FROM clientes;";
        pqxx::result result = txn.exec(query);

        // Mostrar los clientes en la consola
        std::cout << " " << std::endl;
        std::cout << "---------------------------------------------------" << std::endl;
        std::cout << "                  Lista de Clientes                " << std::endl;
        std::cout << "---------------------------------------------------" << std::endl;
        std::cout << " " << std::endl;
        for (auto row : result) {
            std::cout << "ID: " << row["id"].as<int>()
                      << ", Nombre: " << row["nombre"].as<std::string>()
                      << ", Correo: " << row["correo"].as<std::string>()
                      << ", Teléfono: " << row["telefono"].as<std::string>() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error al consultar clientes: " << e.what() << std::endl;
    }
}