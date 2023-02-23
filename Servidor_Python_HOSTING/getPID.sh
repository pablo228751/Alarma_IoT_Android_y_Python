#!/bin/bash
echo "___________Entro a Obtener PID"
nroPiD=$(ps -ef | grep "sockt.py" | grep -v "grep" | awk '{print $2}')
echo "___________nro PID ${nroPiD}"
echo "___________Finalizo"
echo $nroPiD