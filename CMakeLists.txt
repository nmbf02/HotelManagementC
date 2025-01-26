cmake_minimum_required(VERSION 3.15)

# Nombre del proyecto
project(HotelManagementC)

# Estándar de C++
set(CMAKE_CXX_STANDARD 17)

# Directorios de encabezados
include_directories(include)

# Archivos fuente
file(GLOB SOURCES "src/*.cpp")

# Configuración de salida
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# Busca bibliotecas necesarias para PostgreSQL
find_package(PostgreSQL REQUIRED)

# Busca libharu
find_library(LIBHPDF hpdf REQUIRED)

# Genera el ejecutable
add_executable(HotelManagementC ${SOURCES})

# Vincula las bibliotecas necesarias
target_link_libraries(HotelManagementC ${LIBHPDF} pqxx pq)