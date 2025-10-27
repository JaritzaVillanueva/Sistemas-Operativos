# 🧠 Laboratorios de Sistemas Operativos

Este repositorio reúne todos los **laboratorios prácticos desarrollados durante el curso de Sistemas Operativos**, con el propósito de aplicar y reforzar los conceptos teóricos mediante ejercicios de programación en **Bash**, **C** y **Go**.  
Cada laboratorio aborda un tema fundamental del funcionamiento interno del sistema operativo: desde la automatización de tareas hasta la gestión de procesos, memoria y sistemas de archivos.

---

## 🎯 Objetivo general

Comprender los principales mecanismos internos de un sistema operativo mediante prácticas guiadas que permiten observar su comportamiento a nivel de procesos, concurrencia y comunicación entre componentes.

---

## 📚 Contenido de los laboratorios

| Nº | Tema principal | Descripción |
|----|----------------|-------------|
| **Laboratorio 1 – Shell Scripts en Linux** | Introducción al entorno de Linux y scripting en **Bash**. Se crean y ejecutan scripts para automatizar tareas comunes del sistema. |
| **Laboratorio 2 – Procesos y comunicación entre procesos (padre-hijo)** | Programación en **C**. Se implementa la creación de procesos con `fork()`, comunicación mediante **pipes**, y el uso de descriptores de archivos y funciones del sistema (`exec`, `dup`, `getpid`, etc.). |
| **Laboratorio 3 – Pthreads y Sockets en Linux** | Uso de **hilos (pthreads)** para paralelismo y **sockets** para comunicación entre procesos en red. Enfatiza la sincronización y la coordinación entre hilos. |
| **Laboratorio 4 – Concurrencia en Go (Golang)** | Aplicación de **goroutines**, **canales** y el uso de la instrucción `select` para controlar múltiples tareas concurrentes. Incluye sincronización de procesos concurrentes. |
| **Laboratorio 5 – Gestión de memoria** | Análisis de la administración de memoria: asignación dinámica, uso de punteros y estructuras de datos en C. Se proporciona código base para experimentar con modelos de asignación. |
| **Laboratorio 6 – Sistema de archivos exFAT** | Exploración del sistema de archivos **exFAT**, su estructura interna, manejo de directorios, bloques y metadatos. Se trabaja con imágenes de dispositivos y análisis de bajo nivel. |

---

## ⚙️ Tecnologías utilizadas

- 🐧 **Linux / WSL** – entorno principal de trabajo  
- 🧮 **C** – manipulación de procesos, memoria y archivos  
- 🐚 **Bash** – scripting y automatización  
- 🚀 **Go (Golang)** – concurrencia y sincronización moderna  

---
