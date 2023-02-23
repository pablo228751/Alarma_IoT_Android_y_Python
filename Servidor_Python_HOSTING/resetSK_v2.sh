#!/bin/bash
echo "___________Entro a Obtener PID"
nroPiD=$(ps -ef | grep "sockt.py" | grep -v "grep" | awk '{print $2}')
echo "___________nro PID ${nroPiD}"
echo "___________PID: "
echo $nroPiD

kill -9 $(ps -ef | grep "sockt.py" | grep -v "grep" | awk '{print $2}')
sudo rm -f nohup.out

contador=0
while true; do 
    echo "Intento #$contador de ejecutar el comando 'nohup python sockt.py &'..."
    nohup python sockt.py &
    nroPiD=$(ps -ef | grep "sockt.py" | grep -v "grep" | awk '{print $2}')
    if [ -z "$nroPiD" ]; then
        if [ $contador -eq 10 ]; then
            break
        else
            contador=$((contador+1))
            continue
        fi
    else
        echo ".....Inicio script del socket........"
        echo ".......Obtengo NUEVO PID .................."
        echo $nroPiD
        echo ".......Finalizo......................"
        break
    fi
done
counter=0
while [ $counter -lt 20 ]
do
  if rm nohup.out; then
    break
  fi
  ((counter++))
  sleep 1
done
if [ $counter -eq 20 ]; then
  echo "No se pudo eliminar el archivo nohup.out después de 20 intentos."
else
  echo "El archivo nohup.out se ha eliminado con éxito."
fi
