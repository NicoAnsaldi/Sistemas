#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char* argv[]) {
	int status;
	pid_t child;

	if (argc <= 1) {
		fprintf(stderr, "Uso: %s commando [argumentos ...]\n", argv[0]);
		exit(1);
	}

	/* Fork en dos procesos */
	child = fork();
	if (child == -1) { perror("ERROR fork"); return 1; }
	if (child == 0) {
		/* S'olo se ejecuta en el Hijo */
		if (ptrace(PTRACE_TRACEME, 0, NULL, NULL)) {
			perror("ERROR child ptrace(PTRACE TRACEME, ...)"); exit(1);
		}
		execvp(argv[1], argv+1);
		/* Si vuelve de exec() hubo un error */
		perror("ERROR child exec(...)"); exit(1);
	} else {
		/* S'olo se ejecuta en el Padre */
		while(1) {
			if (wait(&status) < 0) { /*perror("waitpid");*/ break; }
			if (WIFEXITED(status)) break; /* Proceso terminado */
			if(ptrace(PTRACE_PEEKUSER, child, 8 * RAX, NULL) < 0){
				if(ptrace(PTRACE_PEEKUSER, child, 8*ORIG_RAX, NULL) == SYS_kill){
					printf("Se ha hecho justicia!\n");
					kill(child, SIGKILL);
				}else{
					ptrace(PTRACE_SYSCALL, child, NULL, NULL);
				}
			}else{
				ptrace(PTRACE_SYSCALL, child, NULL, NULL);
			}
		}
	}
	ptrace(PTRACE_DETACH, child, NULL, NULL);
	return 0;
}
