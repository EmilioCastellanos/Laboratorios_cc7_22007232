# Log Analyzer - Laboratorio 005Threads
### Emilio Castellanos - 22007232

## Descripción
Este proyecto es un analizador de archivos de log tipo Apache, implementado en C, que permite comparar el procesamiento single-thread y multi-thread usando hilos POSIX (`pthread`).

## Archivos principales
- **main_linux.c**: Programa principal. Lee el archivo de log, lanza los hilos (o ejecuta en modo single-thread), recolecta estadísticas y mide el tiempo de ejecución.
- **log_processor.c / log_processor.h**: Lógica de procesamiento de líneas de log por hilo o en modo único.
- **access_log_file_generator.py**: Script para generar un archivo de log de prueba (`access.log`).
- **run.sh**: Script para compilar y ejecutar el programa fácilmente.
- **Makefile**: Compilación del ejecutable `log_analyzer`.

## Uso
### 1. Generar archivo de log de prueba
```bash
python3 access_log_file_generator.py
```
Esto crea un archivo `access.log` con 5000 entradas.

### 2. Compilar
```bash
make
```

### 3. Ejecutar
Puedes ejecutar el programa en modo single-thread o multi-thread:

- **Single-thread:**
  ```bash
  ./log_analyzer single
  # o
  ./run.sh single
  ```
- **Multi-thread:**
  ```bash
  ./log_analyzer multi
  # o
  ./run.sh multi
  ```

Si no se especifica argumento, por defecto usa multi-thread.

## Salida
El programa imprime:
- Total de IPs únicas
- Total de errores HTTP (códigos 4xx y 5xx)
- URL más visitada y su cantidad
- Tiempo de ejecución en milisegundos

Ejemplo:
```
Thread 0 -> start 0 end 1250
Thread 1 -> start 1250 end 2500
Thread 2 -> start 2500 end 3750
Thread 3 -> start 3750 end 5000
Total Unique IPs: 5
HTTP Errors: 2500
Most Visited URL: /index.html (850 times)
Execution time: 3.12 ms
```