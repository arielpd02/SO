#Linea para que se pueda agregar como comando.
#! bin/bash

comandos = ("verInformacionUsuario"  "agregarInformacionUsuario")

read comando

for item in ${comandos[@]} do
    if [ $comando == $item ]
    then 
        gcc -o agregarInfo agregarInfoUser.c
        ./agregarInfo
    else 
        echo "Accion no existente."        
