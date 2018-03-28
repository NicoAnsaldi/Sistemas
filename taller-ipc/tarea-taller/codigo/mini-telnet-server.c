#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "config.h"

int
main(int argc, char **argv)
{
	char buf[MSGLEN + 1];
	struct sockaddr_in local, remote;
	int s,r,a;

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons(PORT);

	if (bind(s, (struct sockaddr *)&local, sizeof(local)) == -1) {
		perror("bind");
		exit(1);
	}

	if (listen(s, 1) == -1){
		perror("listen vieja");
		exit(1);
	}
	//TODO: Completar conexión del socket para TCP

	int longitud = sizeof(remote);
	if((a = accept(s, (struct sockaddr *)&remote, &longitud)) == -1){
			perror ("se rompe todoooo (accept)");
			exit(1);
	}
	//recv(a, buf, MSGLEN+1, 0);
	for (;;) {
		memset(&buf, 0, MSGLEN+1);

		//TODO: Recibir mensaje en buf
		
		printf("Se conecto\n");
		//Imprimir comando
		
		r = recv(a, buf, sizeof(buf) - 1, 0);
		if (r == -1) {
			perror("read");
			continue;
		}
		/* transforma en string de C */
		buf[r] = '\0';
		
		printf("comando: %s\n", buf);
		if (strncmp(buf, ENDMSG, sizeof(ENDMSG)) == 0)
			break;
		//Ejectuar comando

		if(system(buf) == -1){
			perror("error ejecución comando");
			break;
		}
	}

	close(s);
	return 0;
}
