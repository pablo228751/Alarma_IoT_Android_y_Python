import socket
import sys
from datetime import datetime
import time
import os

server_sockets = []

def close_client_connections(s):
    for c in server_sockets:
        try:
            #print("Cerrando conexion de: ", str(c))
            c.close()
        except:
            pass
    del server_sockets[:]


#Verificar si el archivo existe
if os.path.exists("est.txt"):
    #Leer el archivo y guardar el valor en la variable estado_alarma
    with open("est.txt", "r") as f:
        estado_alarma = f.read()
        if estado_alarma == "True":
            estado_alarma = True
        else:
            estado_alarma = False
else:
    #Si el archivo no existe, por defecto la variable inicia en False
    estado_alarma = False

# Creacion del socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Asignacion de una direccion IP y un puerto al socket
server_address = ('', 2105)
sock.bind(server_address)

# Escuchar conexiones entrantes
sock.listen(100)


##Variables
sonar_sirena=False
alarma_automatico=False
msj_enviado=False
vueltas= 5
contador = vueltas


    


while True:
    try:
        connection, client_address = sock.accept()
        server_sockets.append(connection)
        #print('Cliente conectado desde: ', client_address)
        start_time = time.time()

        while True:            
            data = connection.recv(200)
            #print('Recibido:  {!r}'.format(data))
            if data:
                if data == b'cerrar':
                    connection.close()
                    sys.exit()

                ### Placa ####
                if data == b'dispara=SI':                    
                    alarma_automatico=True
                    data=None

                if data == b'estado2':
                    if estado_alarma ==True:
                        data=b'AlarmActivada=SI'
                        connection.sendall(data)
                    else:
                        data=b'AlarmActivada=NO'
                        connection.sendall(data)
                    data=None

                    #Guardar el valor de estado_alarma en el archivo est.txt 
                    with open("est.txt", "w") as f:  #Abrir el archivo en modo escritura para guardar el valor de la variable estado_alarma 
                        f.write(str(estado_alarma))

                    if sonar_sirena ==True:
                        data=b'SirenaActivada=SI'
                        connection.sendall(data)
                    else:
                        data=b'SirenaActivada=NO'
                        connection.sendall(data)
                    data=None

                ### FINA Placa ###

                #### APP Android ####
                if data == b'activarA':
                    estado_alarma=True

                    #Guardar el valor de estado_alarma en el archivo est.txt 
                    with open("est.txt", "w") as f:  #Abrir el archivo en modo escritura para guardar el valor de la variable estado_alarma 
                        f.write(str(estado_alarma))

                    data=None                    
                if data == b'desactivarA':
                    estado_alarma=False

                    #Guardar el valor de estado_alarma en el archivo est.txt 
                    with open("est.txt", "w") as f:  #Abrir el archivo en modo escritura para guardar el valor de la variable estado_alarma 
                        f.write(str(estado_alarma))

                    alarma_automatico=False                    
                    contador = vueltas                    
                    msj_enviado=False                    

                    if sonar_sirena == True:                        
                        sonar_sirena=False                    

                    data=None                                        

                if data == b'sonarSI':                    

                    sonar_sirena=True                    

                    data=b'Sirena=SI'                    

                    connection.sendall(data)                    

                    data=None                                        

                if data == b'sonarNO':                    

                    sonar_sirena=False                    

                    data=None                                        

                if data == b'estado':                    

                    if alarma_automatico == True:                        

                        data=b'DisparoAlarma'                        

                        connection.sendall(data)

                    elif estado_alarma ==True:                        

                        data=b'AlarmActivada'                        

                        connection.sendall(data)                    

                    else:                        

                        data=b'AlarmDesActivada'                        

                        connection.sendall(data)                    

                    data=None                    

                    ##Si suena alarma desde placa                    

                    if alarma_automatico == True:                        

                        if msj_enviado == False:                            

                            data=b'Alarma=SI'                            

                            connection.sendall(data)                        

                            data=None                        

                        contador -= 1                        

                        if contador <= 0:                            

                            contador=0                            

                            if msj_enviado == False:                                

                                msj_enviado=True

##Si se apaga alarma desde placa se vuelve a resetear msj a False
                if alarma_automatico == False and msj_enviado ==True:
                    msj_enviado=False

###### FIN APP Android ####

# Cortar conexion si supera los 30 segundos
                if (time.time() - start_time) > 30:
                    connection.close()
                    break

                data=None

            if len(server_sockets) > 0:
                # Si hay conexiones abiertas, cerramos todas
                close_client_connections(sock)

            if data == None:
                hora = datetime.now()
                close_client_connections(sock)
                break
            

    except Exception as e:
        #print("Error: ",e)
        close_client_connections(sock)
        sock.close()
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.bind(server_address)
        sock.listen(100)
