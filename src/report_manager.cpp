#include "report_manager.h"
#include <iostream>
#include <fstream>
#include "utils.h"
#include <pqxx/pqxx> // Para acceder a la base de datos
#include <hpdf.h>    // Librería libharu para generar PDF
#include <chrono>
#include <iomanip>   // Para std::put_time - Para el formato de fecha y hora de archivo

void error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void* user_data) {
    std::cerr << "Error al generar PDF: " << error_no << ", detalle: " << detail_no << std::endl;
    throw std::runtime_error("Error en la generacion del PDF.");
}

// Función para obtener la fecha y hora actual en formato "YYYY-MM-DD_HH-MM-SS"
std::string obtener_fecha_y_hora_actual() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time);

    // Crear un string con el formato deseado
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");  // Formato: 2025-01-23_14-55-30
    return ss.str();
}

void generar_reporte(const std::string& archivo_pdf) {
    try {
        pqxx::connection conn = connect_to_db(); // Conexión a la base de datos
        pqxx::work txn(conn);

        // Crear un nuevo documento PDF
        HPDF_Doc pdf = HPDF_New(error_handler, nullptr);
        if (!pdf) throw std::runtime_error("No se pudo crear el documento PDF.");

        // Obtener la fecha y hora actual para usar en el nombre del archivo
        std::string fecha_y_hora = obtener_fecha_y_hora_actual();

        // Generar el nombre del archivo PDF con fecha y hora actual
        std::string ruta_completa = "C:/msys64/home/natha/HotelManagementC/pdf/reporte_" + fecha_y_hora + ".pdf";

        // Agregar una página
        HPDF_Page page = HPDF_AddPage(pdf);
        HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

        // Configurar el texto
        HPDF_Font font = HPDF_GetFont(pdf, "Times-Roman", nullptr);
        HPDF_Font font_bold = HPDF_GetFont(pdf, "Times-Bold", nullptr);  // Negrita para encabezados
        HPDF_Page_SetFontAndSize(page, font, 12);

        // Iniciar el PDF
        HPDF_Page_BeginText(page);
        HPDF_Page_TextOut(page, 50, 800, "Reporte del Sistema de Gestion de Hoteles");

        // Añadir encabezado
        int y_position = 750;
        HPDF_Page_SetFontAndSize(page, font_bold, 12);  // Usar negrita para el encabezado
        HPDF_Page_TextOut(page, 50, y_position, "Listado de Clientes");
        y_position -= 20; // Espacio después del encabezado

        // Mostrar los clientes
        std::string header = "ID | Cliente ID | Nombre | Correo | Telefono";
        HPDF_Page_TextOut(page, 50, y_position, header.c_str());
        y_position -= 20;

        // Consultar datos de la base de datos (Clientes)
        std::string query = "SELECT id, nombre, correo, telefono FROM clientes;";
        pqxx::result result = txn.exec(query);

        // Escribir los datos de clientes en el PDF
        for (const auto& row : result) {
            std::string line = "Cliente ID: " + row["id"].as<std::string>() + " | " +
                               "Nombre: " + row["nombre"].as<std::string>() + " | " +
                               "Correo: " + row["correo"].as<std::string>() + " | " +
                               "Teléfono: " + row["telefono"].as<std::string>();

            HPDF_Page_TextOut(page, 50, y_position, line.c_str());
            y_position -= 15; // Espacio entre líneas

            // Si llega al final de la página, agregar una nueva
            if (y_position < 50) {
                HPDF_Page_EndText(page);
                page = HPDF_AddPage(pdf);
                y_position = 750;
                HPDF_Page_SetFontAndSize(page, font, 10);
                HPDF_Page_BeginText(page);
            }
        }

        // Salto de línea antes de mostrar el cliente con más reservas
        y_position -= 20;  // Mueve la posición hacia abajo

        // Añadir el cliente con más reservas
        std::string query_mas_reservas = "SELECT cliente_id, COUNT(*) AS reservas_count FROM reservas GROUP BY cliente_id ORDER BY reservas_count DESC LIMIT 1;";
        pqxx::result result_mas_reservas = txn.exec(query_mas_reservas);

        std::string mas_reservas = "El cliente con mas reservas es el ID: " +
                                result_mas_reservas[0]["cliente_id"].as<std::string>() + " con " +
                                result_mas_reservas[0]["reservas_count"].as<std::string>() + " reservas.";

        HPDF_Page_TextOut(page, 50, y_position, mas_reservas.c_str());
        y_position -= 20; // Mueve la posición hacia abajo para la siguiente sección

        // Consultar las reservas para el día de hoy
        std::string query_reservas_hoy = "SELECT id, cliente_id, fecha_inicio FROM reservas WHERE fecha_inicio = CURRENT_DATE;";
        pqxx::result result_reservas_hoy = txn.exec(query_reservas_hoy);
        HPDF_Page_TextOut(page, 50, y_position, "Reservas para el dia de hoy:");
        y_position -= 20;

        // Escribir las reservas en el PDF
        for (const auto& row : result_reservas_hoy) {
            std::string reserva = "Reserva ID: " + row["id"].as<std::string>() + " | " +
                                  "Cliente ID: " + row["cliente_id"].as<std::string>() + " | " +
                                  "Fecha Inicio: " + row["fecha_inicio"].as<std::string>();
            HPDF_Page_TextOut(page, 50, y_position, reserva.c_str());
            y_position -= 15;
        }

        // Finalizar el texto en el PDF
        HPDF_Page_EndText(page);

        // Guardar el archivo en la ubicación especificada
        HPDF_SaveToFile(pdf, ruta_completa.c_str());

        // Cerrar el documento PDF
        HPDF_Free(pdf);

        std::cout << "Reporte generado con exito: " << ruta_completa << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error al generar el reporte: " << e.what() << std::endl;
    }
}
