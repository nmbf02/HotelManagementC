CREATE TABLE clientes (
    id SERIAL PRIMARY KEY,
    nombre VARCHAR(100),
    correo VARCHAR(100),
    telefono VARCHAR(15)
);

CREATE TABLE habitaciones (
    id SERIAL PRIMARY KEY,
    numero INT NOT NULL,
    tipo VARCHAR(50),
    precio DECIMAL(10, 2)
);

CREATE TABLE reservas (
    id SERIAL PRIMARY KEY,
    cliente_id INT REFERENCES clientes(id),
    habitacion_id INT REFERENCES habitaciones(id),
    fecha_reserva DATE,
    fecha_inicio DATE,
    fecha_fin DATE
);
