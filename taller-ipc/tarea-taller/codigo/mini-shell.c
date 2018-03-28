#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define NELEMS(a) (sizeof(a) / sizeof((a)[0]))

static int
run(const char ***progs, size_t count)
{
	int (*pipes)[2];
	pid_t *children;
	size_t i, j;
	pid_t cur;
	int r, status;

	if (!(children = malloc(sizeof(*children) * count))) {
		fputs("out of memory\n", stderr);
		goto end;
	}

	// TODO: crear pipes ANTES de crear los procesos
	// Pensar cuantos pipes necesito.

	for (i = 0; i < count; i++) {

		//TODO: Crea *count* procesos

		//TODO: Guardar el PID del proceso hijo en children[i]

		//TODO: Para cada proceso hijo:
			//1. Redireccionar los file descriptors adecuados al proceso
			//2- Ejecutar el programa correspondiente
	}

	//El padre espera a que terminen todos los procesos hijos que ejecutan los programas
	for (i = 0; i < count; i++) {
		if (waitpid(children[i], &status, 0) == -1) {
			perror("waitpid");
			return -1;
		}
		if (!WIFEXITED(status)) {
			fprintf(stderr, "proceso %d no terminó correctamente [%d]: ",
			    (int)children[i], WIFSIGNALED(status));
			perror("");
			return -1;
		}
	}
	r = 0;

	free(children);
	free(pipes);

	return r;
}

int
main(int argc, char **argv)
{
	char *lscmd[] = { "ls", "-al", NULL };
	char *wccmd[] = { "wc", NULL };
	char *awkcmd[] = { "awk", "{ print $2 }", NULL };
	char **progs[] = { lscmd, wccmd, awkcmd };

	printf("status: %d\n", run(progs, NELEMS(progs)));
	fflush(stdout);
	fflush(stderr);

	return 0;
}
