# Virtual Address Translation Simulator

Este proyecto es una simulación en C que modela la traducción de direcciones virtuales paginadas para un proceso cargado en memoria. El simulador se encarga de separar una dirección virtual de 16-bits en su **Virtual Page Number (VPN)** y su **Offset**, mantener una tabla de páginas lineal, asignar marcos de memoria física a partir de una memoria RAM simulada de tamaño fijo, y traducir un lote de direcciones virtuales hacia direcciones físicas o emitir errores claros si la traducción no es posible.

## Funcionamiento Interno

1. **Memoria Física (RAM):**
   Se simulan 100 marcos de memoria física de 256 bytes cada uno. Antes de la ejecución del proceso, una porción aleatoria de estos marcos se marcan como "Ocupados" (`OCCUPIED`), mientras que el resto quedan "Libres" (`FREE`).

2. **Carga del Proceso:**
   El simulador reserva cierta cantidad de marcos de la RAM libre (especificada como número de páginas virtuales) y las asigna a la tabla de páginas del proceso.

3. **Traducción (Page Table):**
   El programa lee un archivo con direcciones virtuales. Para cada dirección:
   - Extrae el VPN (8 bits más significativos) y el Offset (8 bits menos significativos).
   - Valida que la dirección y el VPN estén dentro del rango permitido (`VA_OUT_OF_RANGE`, `VPN_OUT_OF_RANGE`).
   - Revisa en la tabla de páginas si el VPN tiene asignado un Physical Frame Number (PFN).
   - En caso afirmativo, calcula la **Dirección Física (PA)** usando la fórmula `PA = (PFN * 256) + Offset`.

## Compilación

El proyecto incluye un `Makefile` para automatizar la compilación mediante GCC.

Para compilar el programa, abre una terminal en el directorio donde se encuentran los archivos fuente y ejecuta:

```bash
make
```

Esto generará un archivo ejecutable llamado `lab9`.

Si deseas eliminar el binario generado para limpiar el entorno, puedes usar:

```bash
make clean
```

## Modo de Uso

Para correr el simulador, necesitas ejecutar el programa pasándole por línea de comandos los parámetros necesarios.

**Sintaxis:**
```bash
./lab9 <NUM_VIRTUAL_PAGES> <archivo_de_direcciones> [semilla_opcional]
```

### Parámetros:
1. `<NUM_VIRTUAL_PAGES>`: (Obligatorio) Un entero entre 1 y 256 que define cuántas páginas virtuales tendrá el proceso. 
2. `<archivo_de_direcciones>`: (Obligatorio) Ruta al archivo de texto que contiene las direcciones virtuales a traducir (una por línea, pueden estar en formato decimal o hexadecimal).
3. `[semilla_opcional]`: (Opcional) Un número entero que sirve como semilla para el generador pseudoaleatorio. Al usar una misma semilla, la disposición de la memoria RAM inicial se mantendrá idéntica en distintas ejecuciones (ideal para depurar resultados). Si se omite, la RAM será diferente en cada ejecución.

### Ejemplo de Ejecución:

Se provee un archivo `example_addresses.txt` con algunas direcciones de prueba.

Ejecutar con 8 páginas virtuales y una semilla aleatoria ligada al reloj:
```bash
./lab9 8 example_addresses.txt
```

Ejecutar con 8 páginas virtuales y una semilla fija (ej. `12345`) para obtener resultados reproducibles:
```bash
./lab9 8 example_addresses.txt 12345
```

### Interpretación de la Salida

La salida en consola constará de 3 bloques principales:
1. **Mapa de RAM:** Un resumen visual enumerando los marcos de 0 a 99 con el estado `F` (Libre) o `X` (Ocupado).
2. **Resumen de Carga:** Lista de qué PFN (marcos) le fueron asignados a cada VPN cargado.
3. **Traducciones:** Línea por línea con el resultado de la dirección virtual evaluada, desglosando la información de la operación o explicando por qué falló.
