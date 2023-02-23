#!/bin/bash
nohup python sockt.py &
echo ".....Inicio script del socket........"
nroPiD=$(ps -ef | grep "sockt.py" | grep -v "grep" | awk '{print $2}')
echo ".......Obtengo PID .................."
echo $nroPiD
echo ".......Finalizo uno_2......................"