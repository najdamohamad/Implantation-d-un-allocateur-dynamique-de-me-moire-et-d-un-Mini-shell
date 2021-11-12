#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <fcntl.h>

#include "variante.h"
#include "readcmd.h"

#ifndef VARIANTE
#error "Variante non défini !!"
#endif

//gestion des jobs

typedef struct _list_of_jobs{
	pid_t pid;
	char* cmd;
	struct _list_of_jobs* next;
} *list_of_jobs;

list_of_jobs list = NULL;

list_of_jobs last_list_of_jobs() {
    if (list == NULL) {
        list = malloc(sizeof(list_of_jobs));
        list->next = NULL;
        return list;
    }

    list_of_jobs courant = list;

    while(courant->next != NULL) {
        courant = courant->next;
    }

    courant->next = malloc(sizeof(list_of_jobs));
    courant->next->next = NULL;

    return courant->next;
}

void gestion_des_jobs(list_of_jobs list){
	printf("liste des jobs \n");

	list_of_jobs courant = list;
	list_of_jobs ancien = NULL;

	while(courant != NULL) {
			int status;

			/* If the process is running */
			if(((bool) waitpid(courant->pid, &status, WNOHANG))==false) {
					printf("%d : %s\n", courant->pid, courant->cmd);
			} else {
					if(ancien == NULL) {
							list = courant->next;
					} else {
							ancien->next = courant->next;
					}
			}

			ancien = courant;
			courant = courant->next;
	}
}

//traitement de la commande
int tuyau[2];

void traiter_cmd(struct cmdline* ptr_l,char** cmd,int i){
	pid_t pid;
	int status;

	if(l->seq[1] != NULL{
		pipe(tuyau);
	}

	switch(pid=fork()){

		case -1:
			perror("fork:") ;
			break ;

		case 0:

			if(i>0) {
					dup2(tuyau[1],1);
			}
			if(ptr_l->seq[i+1]!=NULL) {
					dup2(tuyau[0],0);
			}

			int file;

			if((ptr_l->in!=NULL) && (i==0)){
				if ((file = open(ptr_l->in, O_RDONLY)) < 0) {
						printf("erreur: ouverture impossible %s\n", ptr_l->in);
				}
				dup2(file, STDIN_FILENO);
				close(file);
			}
			if((ptr_l->out!=NULL) && (i==0)){
				if ((file = open(ptr_l->out, O_WRONLY|O_TRUNC|O_CREAT,0666)) < 0) {
						printf("erreur: ouverture impossible %s\n", ptr_l->out);
				}

				dup2(file, STDOUT_FILENO);
				close(file);
			}
			if ((ptr_l->in != NULL) && (ptr_l->seq[i+1] != 0)) {
					if ((file = open(ptr_l->in, O_RDONLY)) < 0) {
							printf("erreur: ouverture impossible %s\n", ptr_l->in);
					}

					dup2(file, STDIN_FILENO);
					close(file);
			}
			if ((ptr_l->out != NULL) && (i>0)) {
					if ((file = open(ptr_l->out, O_WRONLY|O_TRUNC|O_CREAT,0666)) < 0) {
							printf("erreur: ouverture impossible %s\n", ptr_l->out);
					}

					dup2(file, STDOUT_FILENO);
					close(file);
			}


			close(tuyau[1]);
			execvp(cmd[0],cmd);
			break;

		default:
			if(!ptr_l->bg){
				waitpid(pid,&status,0);
				break;
			}
			else{
				list_of_jobs last = last_list_of_jobs();
				last->pid = pid;
				last->cmd = malloc(strlen(cmd[0]) * sizeof(char));
				strcpy(last->cmd, cmd[0]);
				break;
			}
	}
}


/* Guile (1.8 and 2.0) is auto-detected by cmake */
/* To disable Scheme interpreter (Guile support), comment the
 * following lines.  You may also have to comment related pkg-config
 * lines in CMakeLists.txt.
 */

#if USE_GUILE == 1
#include <libguile.h>

int question6_executer(char *line)
{
	/* Question 6: Insert your code to execute the command line
	 * identically to the standard execution scheme:
	 * parsecmd, then fork+execvp, for a single command.
	 * pipe and i/o redirection are not required.
	 */
	printf("Not implemented yet: can not execute %s\n", line);

	/* Remove this line when using parsecmd as it will free it */
	free(line);

	return 0;
}

SCM executer_wrapper(SCM x)
{
        return scm_from_int(question6_executer(scm_to_locale_stringn(x, 0)));
}
#endif


void terminate(char *line) {
#if USE_GNU_READLINE == 1
	/* rl_clear_history() does not exist yet in centOS 6 */
	clear_history();
#endif
	if (line)
	  free(line);
	printf("exit\n");
	exit(0);
}


int main() {
        printf("Variante %d: %s\n", VARIANTE, VARIANTE_STRING);

#if USE_GUILE == 1
        scm_init_guile();
        /* register "executer" function in scheme */
        scm_c_define_gsubr("executer", 1, 0, 0, executer_wrapper);
#endif

	while (1) {
		struct cmdline *l;
		char *line=0;
		int i, j;
		char *prompt = "ensishell>";

		/* Readline use some internal memory structure that
		   can not be cleaned at the end of the program. Thus
		   one memory leak per command seems unavoidable yet */
		line = readline(prompt);
		if (line == 0 || ! strncmp(line,"exit", 4)) {
			terminate(line);
		}

#if USE_GNU_READLINE == 1
		add_history(line);
#endif


#if USE_GUILE == 1
		/* The line is a scheme command */
		if (line[0] == '(') {
			char catchligne[strlen(line) + 256];
			sprintf(catchligne, "(catch #t (lambda () %s) (lambda (key . parameters) (display \"mauvaise expression/bug en scheme\n\")))", line);
			scm_eval_string(scm_from_locale_string(catchligne));
			free(line);
                        continue;
                }
#endif

		/* parsecmd free line and set it up to 0 */
		l = parsecmd( & line);

		/* If input stream closed, normal termination */
		if (!l) {

			terminate(0);
		}



		if (l->err) {
			/* Syntax error, read another command */
			printf("error: %s\n", l->err);
			continue;
		}

		if (l->in) printf("in: %s\n", l->in);
		if (l->out) printf("out: %s\n", l->out);
		if (l->bg) printf("background (&)\n");

		/* Display each command of the pipe */

		for (i=0; l->seq[i]!=0; i++) {
			char **cmd = l->seq[i];
			printf("seq[%d]: ", i);
                        for (j=0; cmd[j]!=0; j++) {
                                printf("'%s' ", cmd[j]);
                        }
			printf("\n");

			if(!strcmp(cmd[0],"jobs")){
				gestion_des_jobs(list);
			}
			else{
				traiter_cmd(l,cmd,i);
			}


		}
	}

}
