#ifndef ROOM_MANAGER_H
#define ROOM_MANAGER_H

#include <string>

// Función para registrar una habitación
void registrar_habitacion(int numero, const std::string& tipo, double precio);

// Función para consultar las habitaciones
void consultar_habitaciones();

#endif