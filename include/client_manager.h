#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

// client_manager.h
#include <mutex>
#include <string>

// Declaración de las funciones para registro de clientes
void validar_datos(const std::string& nombre, const std::string& correo, const std::string& telefono, bool& datos_validos);
void verificar_existencia_en_db(const std::string& correo, bool& cliente_existe);
void insertar_cliente_en_db(const std::string& nombre, const std::string& correo, const std::string& telefono);
void registrar_logs(const std::string& evento, const std::string& nombre, const std::string& correo, const std::string& telefono);


// Declaración de la función para registrar clientes con procesos en paralelo
void registrar_cliente(const std::string& nombre, const std::string& correo, const std::string& telefono);

// Declaración para consulta de clientes
void consultar_clientes();

#endif