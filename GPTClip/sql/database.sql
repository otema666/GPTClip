-- MySQL database creation script
CREATE DATABASE gptClip;
USE gptClip;

CREATE TABLE `usuarios` (
  `id` int NOT NULL AUTO_INCREMENT,                                   -- PK
  `username` varchar(50) NOT NULL UNIQUE,                             -- Nombre de usuario
  `password` varchar(255) NOT NULL,                                   -- Contraseña
  `is_logged` tinyint(1) NOT NULL DEFAULT 0,                          -- ¿Está logueado?
  `creation_date` timestamp NULL DEFAULT CURRENT_TIMESTAMP,           -- Fecha de creación
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;   -- Unicode
