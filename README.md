# Practica Sistemas Operativos #2 2021-01

Este repositorio hace parte de la solución a la practica #2 del curso de Sistemas Operativos de la Universidad Nacional de Colombia en el primer semestre de 2021

## Funcionamiento del programa

El proposito principal del programa es poder hacer uso de la información de tiempos de viaje de las bases de datos de Uber Colombia proporsionada por el docente, donde se debe poder ingresar por parte del usuario la siguiente información:

- Id de origen del viaje
- Id de destino del viaje
- Hora del día de inicio del viaje

Con esta información el programa debe ser capaz de ir rapidamente al archivo que contiene la información y extraer el tiempo medio de viaje para que el usuario lo pueda usar.

## Restricciones de la practica

La solución debe ser capaz de:

- Hallar el tiempo medio de viaje en menos de 2 segundos
- Usar menos de 1Mb de memoria en el proceso servidor
- Hacer uso de punteros y memoria dinamica (malloc() y free())
- Hacer uso de procesos comunicados mediante red, uno (clientes) para mantener la interfaz de usuario y otro (servidor) para la busqueda de la información
- Implementar una hash table que facilite la busqueda en archivos

## Objetivos de la practica

 - Afianzar el uso de estructuras y memoria dinamica en C, además de estructuras de datos como linked list y hash tables
 - Demostrar conocimeintos en el manejo de comunicación entre procesos vistos en clase
 - Explorar formas de acceder a información almacenada en archivos y llevarla al cliente que la solicite
 - Generar una solución modular, rapida, eficiente y confiable que haga uso de buenas practicas de programación

 ## Explicación de la solución desarrollada

 Se implementó una Hash Table dinamica que usa Linked List en caso de alguna colisión que permite asegurar un tiempo continuo en la busqueda de información que almacena un puntero al documento que almacena los datos, mapeado con el id de origen que se desea consultar, esta estructura de datos ayuda al proceso de busqueda a llegar de forma más rapida a la primera posición donde empiezan los datos que tienen el id de origen especificado para hacer una busqueda más optimizada y veloz.

 La interfaz de consola tiene las restricciones propias para detectar cuando un dato ingresado no está dentro de los datos que se podrán encontrar en el archivo de datos y permitira generar multiples busquedas hasta que el usuario decida salir del programa.

 Los dos procesos se comunican mediante la red local para permitir intercambio de información bidireccional, tanto para enviar los datos ingresados por el usuario en la interfaz hacia el servidor de busqueda como regresar el tiempo medio medio hacia el cliente que solicitó la información, almacenando cada consulta en un documento del lado del servidor par amanterner un regitro de acciones preciso.

## Participantes del desarrollo

Gabriel Santana Paredes (gsantana@unal.edu.co)

Diego Esteban Morales (dimorales@unal.edu.co)

Daniel Alejandro Melo (dmelo@unal.edu.co)
