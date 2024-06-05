Juego TUI de Batalla Naval programado en c++

Existen 3 programes principales en este proyecto.
	1 - Player: este inicia el juego creado.
	2 - Receiver: este espera infinitamente por mensajes a que lleguen a su bandeja.
	3 - Sender: este envía un solo mensaje n cantidad de veces.
Estos estan ubicados en la carpeta bin luego de compilar el proyecto.

Para compilar el proyecto ubicarse en la ruta del Makefile y ejecutar el comando "make"

Existen los siguientes comandos en el Makefile, que pueden ser utilizados para efectos de revisión:
	make run_receiver
	make run_sender
	make run_player1
	make run_player2
	
Estos inician los programas mencionados anteriormente, con argumentos predeterminados visibles en el Makefile.

Estos deben de ser ejecutados en parejas, "Receiver" con "Sender" y "Player1" con "Player2".

En caso de no querer utilizar los argumentos predeterminados, es posible optar por correr los programas con cero argumentos por consola "./bin/<programa>", estos informarán que argumentos son necesarios para poder ejecutarlos correctamente.

Los programas no cuentan con chequeo de argumentos por lo que si se ingresan datos inválidos pueden suceder cosas raras.

Por alguna extraña razón el juego puede llegar a colgarse al inicio luego de correr los otros dos programas, en ese caso se puede optar por ejecutar el comando "make clean", para luego compilar e intentar volver a jugar.
