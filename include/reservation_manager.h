#ifndef RESERVATION_MANAGER_H
#define RESERVATION_MANAGER_H

#include <string>

// Función para registrar reservas
void registrar_reserva(int cliente_id, int habitacion_id, const std::string& fecha_inicio, const std::string& fecha_fin);

// Función para consultar las reservas
void consultar_reservas();

// Función para consultar reservas del día actual
void consultar_reservas_del_dia_actual();

#endif
