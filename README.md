# Información T1 - S1

## Ejercicios para Matricula 1-18-xxxx hasta xx-xx-xxxx

1) Desarrollar una app en C++ con varios procesos en paralelos y explicar su código. (Los procesos pueden ser: Sistema de Facturación en el que puedan consultar datos de una BD y al mismo tiempo guardar la factura, actualizar el inventario, actualizar el balance del cliente y envio del reporte por correo electrónico (Este ejemplo ya no es valido, buscar una opción parecida)). Todos estos procesos se deben realizar en paralelos para usar la cpu.

## Sistema de Gestión de Hoteles

## Procesos a realizar:

C++:

Registrar Clientes
Registrar Habitaciones
Registrar Reservas
Consultar Clientes
Consultar Habitaciones
Consultar Reservas
Consulta de los clients que tendrán reservas el día actual
Generar un reporte y guardarlo por pdf 

Procesos en paralelo para usa la CPU

------------------------------------------------------------

BD: PostgreSQL
Editor de código: VSC
Estoy usando: MSYS2 UCRT64
Host: localhost
User name: postgres
Contraseña: 0211
port: 5432

------------------------------------------------------------

Script de BD: hotel_management_c

-- Crear la base de datos
CREATE DATABASE hotel_management_c;

-- Conectar a la base de datos
\c hotel_management_c;

-- Crear la tabla de Clientes (Guests)
CREATE TABLE Guests (
    GuestID SERIAL PRIMARY KEY,
    Name VARCHAR(100) NOT NULL,
    Email VARCHAR(100) UNIQUE NOT NULL,
    PhoneNumber VARCHAR(20),
    Address VARCHAR(255),
    Status VARCHAR(10) DEFAULT 'Active' -- Estado del cliente (Active/Inactive)
);

-- Crear la tabla de Habitaciones (Rooms)
CREATE TABLE Rooms (
    RoomID SERIAL PRIMARY KEY,
    RoomNumber INT UNIQUE NOT NULL,
    Type VARCHAR(10) NOT NULL,
    PricePerNight DECIMAL(10, 2) NOT NULL,
    Status VARCHAR(15) DEFAULT 'Available' -- Estado de la habitación (Available/Occupied/Maintenance)
);

-- Crear la tabla de Reservas (Reservations)
CREATE TABLE Reservations (
    ReservationID SERIAL PRIMARY KEY,
    GuestID INT NOT NULL REFERENCES Guests(GuestID) ON DELETE CASCADE,
    RoomID INT NOT NULL REFERENCES Rooms(RoomID) ON DELETE CASCADE,
    CheckInDate DATE NOT NULL,
    CheckOutDate DATE NOT NULL,
    Status VARCHAR(15) DEFAULT 'Pending' -- Estado de la reserva (Pending/Confirmed/Cancelled)
);

-- Insertar datos de ejemplo en Guests
INSERT INTO Guests (Name, Email, PhoneNumber, Address, Status) VALUES
('Juan Pérez', 'juan.perez@example.com', '8091234567', 'Santo Domingo', 'Active'),
('Ana Gómez', 'ana.gomez@example.com', '8097654321', 'Santiago', 'Active'),
('Luis Rodríguez', 'luis.rodriguez@example.com', '8291234567', 'Puerto Plata', 'Inactive'),
('María López', 'maria.lopez@example.com', '8491239876', 'La Romana', 'Active'),
('Carlos Fernández', 'carlos.fernandez@example.com', '8093332222', 'Bávaro', 'Inactive');

-- Insertar datos de ejemplo en Rooms
INSERT INTO Rooms (RoomNumber, Type, PricePerNight, Status) VALUES
(101, 'Single', 75.00, 'Available'),
(102, 'Double', 120.00, 'Occupied'),
(201, 'Suite', 250.00, 'Available'),
(202, 'Single', 80.00, 'Maintenance'),
(301, 'Double', 130.00, 'Available');

-- Insertar datos de ejemplo en Reservations
INSERT INTO Reservations (GuestID, RoomID, CheckInDate, CheckOutDate, Status) VALUES
(1, 101, '2025-01-01', '2025-01-03', 'Confirmed'),
(2, 102, '2025-01-02', '2025-01-05', 'Pending'),
(3, 201, '2025-01-04', '2025-01-07', 'Cancelled'),
(4, 301, '2025-01-03', '2025-01-06', 'Confirmed'),
(5, 202, '2025-01-08', '2025-01-10', 'Pending');
