echo "Compilando para qemu..."

gcc main.c parking.c -o parking -lpthread

if [ $? -ne 0 ]; then
echo "Error en la compilación"
exit 1
fi

echo "Compilación exitosa"

echo "Ejecutando simulación..."
echo "-----------------------------------"


./parking

