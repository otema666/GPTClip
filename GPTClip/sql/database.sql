CREATE DATABASE gptClip;
USE gptClip;
CREATE TABLE usuarios (
    id INT AUTO_INCREMENT PRIMARY KEY,    -- PK
    nombre_usuario VARCHAR(50) NOT NULL,  -- Nombre de usuario
    contrasena VARCHAR(255) NOT NULL,     -- Contraseña
    fecha_creacion TIMESTAMP DEFAULT CURRENT_TIMESTAMP,  -- Fecha de creación del usuario
);
