#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <assert.h>

#include <sys/wait.h>
#include <sys/types.h>

#include "proc-common.h"
#include "request.h"

#include <string.h>

/* Compile-time parameters. */
#define SCHED_TQ_SEC 2                /* time quantum */
#define TASK_NAME_SZ 60               /* maximum size for a task's name */

struct Process_List
{
	int id;
	pid_t pid;
	char name[TASK_NAME_SZ];
	struct Process_List *next;
};

struct Process_List *head, *current;

void child (char *name)
{
	if (raise (SIGSTOP)) {
		perror("raise");
		exit (1);
	}
	/* Wait for the creation of the Process_List */

	char *newenviron[]	={ NULL };
	char *newargv[]		={ name, NULL/*, NULL, NULL*/ };
	execve (name, newargv, newenviron);

	/* execve() only returns on error */
	perror("execve");
	exit(1);
}

void activate_current ()
{
	printf("Scheduler: Process with ID = %d, Pid = %ld, Name = %s, CONTINUES.\n", current->id, (long) current->pid, current->name);

	/* Start counting tq */
	if (alarm (SCHED_TQ_SEC) < 0) {
		perror("alarm");
		exit(1);
	}
	if (kill (current->pid, SIGCONT)) {
		perror("kill");
		exit (1);
	}
}

/*
 * Removes from the list the now dead child.
 * Running shows if we change current.
 */
void child_died (pid_t p)
{
	int running = 0;
	if (current->pid == p) running = 1;

	struct Process_List *temp = head, *prev = NULL;

	while (temp != NULL) {
		if (temp->pid == p) {
			printf("Scheduler: Process with ID = %d, Pid = %ld, Name = %s, HAS DIED.\n", temp->id, (long) temp->pid, temp->name);

			if (prev != NULL) {	// child that died is not the head
				prev->next = temp->next;
				free (temp);
			} else {			// child that died is the head
				if (head->next == NULL) {	// last child died
					free (head);
					printf("Scheduler: No tasks remaining. Exiting...\n");
					exit (1);
				}
				temp = head->next;
				free (head);
				head = temp;
			}

			if (running) {
				if (prev == NULL || prev->next == NULL) current = head;
				else current = prev->next;
			}

			return;
		}
		prev = temp;
		temp = temp->next;
	}

	fprintf(stderr, "Internal error: Child not found\n");
	exit(1);
}


/*
 * SIGALRM handler
 */
static void
sigalrm_handler(int signum)
{
	if (signum != SIGALRM) {
		fprintf(stderr, "Internal error: Called for signum %d, not SIGALRM\n", signum);
		exit(1);
	}

	/* Stop current running child */
	printf("Scheduler: Process with ID = %d, Pid = %ld, Name = %s, STOPS.\n", current->id, (long) current->pid, current->name);
	if (kill (current->pid, SIGSTOP)) {
		perror("kill");
		exit (1);
	}
}

/*
 * SIGCHLD handler
 */
static void
sigchld_handler(int signum)
{
	if (signum != SIGCHLD) {
		fprintf(stderr, "Internal error: Called for signum %d, not SIGCHLD\n", signum);
		exit(1);
	}

	int status, activate = 0;
	pid_t p;

	/*
	 * Something has happened to one of the children.
	 * We use waitpid() with the WUNTRACED flag, instead of wait(), because
	 * SIGCHLD may have been received for a stopped, not dead child.
	 *
	 * A single SIGCHLD may be received if many processes die at the same time.
	 * We use waitpid() with the WNOHANG flag in a loop, to make sure all
	 * children are taken care of before leaving the handler.
	 */
	for (;;) {
		p = waitpid (-1, &status, WUNTRACED | WNOHANG);
		if (p < 0) {
			perror("waitpid");
			exit(1);
		}

		if (p == 0) break;

		if (WIFEXITED(status) || WIFSIGNALED(status)) {
			/* A child has died, remove from list, if current died activate next in line */
			if (current->pid == p) activate = 1;
			child_died (p);
		}
		if (WIFSTOPPED(status) && (current->pid == p)) {
			/* The active child has stopped due to SIGSTOP/SIGTSTP, etc... */
			printf("Scheduler: Process with ID = %d, Pid = %ld, Name = %s, HAS STOPPED.\n", current->id, (long) current->pid, current->name);
			activate = 1;
			if (current->next == NULL) current = head;
			else current = current->next;
		}
	}
	if (activate) activate_current();
}

/* Install two signal handlers.
 * One for SIGCHLD, one for SIGALRM.
 * Make sure both signals are masked when one of them is running.
 */
static void
install_signal_handlers(void)
{
	sigset_t sigset;
	struct sigaction sa;

	sa.sa_handler = sigchld_handler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGCHLD);
	sigaddset(&sigset, SIGALRM);
	sa.sa_mask = sigset;
	if (sigaction(SIGCHLD, &sa, NULL) < 0) {
		perror("sigaction: sigchld");
		exit(1);
	}

	sa.sa_handler = sigalrm_handler;
	if (sigaction(SIGALRM, &sa, NULL) < 0) {
		perror("sigaction: sigalrm");
		exit(1);
	}

	/*
	 * Ignore SIGPIPE, so that write()s to pipes
	 * with no reader do not result in us being killed,
	 * and write() returns EPIPE instead.
	 */
	if (signal(SIGPIPE, SIG_IGN) < 0) {
		perror("signal: sigpipe");
		exit(1);
	}
}

int main(int argc, char *argv[])
{
	int nproc = argc - 1;	/* number of proccesses goes here */

	if (nproc == 0) {
		fprintf(stderr, "Scheduler: No tasks. Exiting...\n");
		exit(1);
	}

	/*
	 * For each of argv[1] to argv[argc - 1],
	 * create a new child process, add it to the process list.
	 */
	pid_t p;

	int i;
	for (i = 1; i <= nproc; i++) {
		if (i == 1) {
			head = (struct Process_List *) malloc (sizeof (struct Process_List));
			if (head == NULL) {
				printf ("malloc");
				exit (1);
			}
			current = head;
		}
		else {
			current->next = (struct Process_List *) malloc (sizeof (struct Process_List));
			if (current->next == NULL) {
				printf ("malloc");
				exit (1);
			}
			current = current->next;
		}

		current->next = NULL;
		current->id = i;
		strcpy (current->name, argv[i]);
		p = fork ();
		if (p == -1) {
			perror("fork");
			exit (1);
		}
		else if (p == 0) {
			/* child code */
			child (argv[i]);
			/* Should not reach this point */
			assert (0);
		}
		/* parent code */
		current->pid = p;
	}

	/* head points to the 1st node */

	/* Wait for all children to raise SIGSTOP before exec()ing. */
	wait_for_ready_children(nproc);

	/* Install SIGALRM and SIGCHLD handlers. */
	install_signal_handlers();

	printf("\n\n\n\nSTART SCHEDULING\n\n\n\n");
	/* Process_List ready with head -> start | tail -> end */

	current = head;

	/* Waking MSG */
	printf("Scheduler: Process with ID = %d, Pid = %ld, Name = %s, CONTINUES.\n", head->id, (long) head->pid, head->name);

	/* Start counting tq */
	if (alarm (SCHED_TQ_SEC) < 0) {
		perror("alarm");
		exit(1);
	}

	/* Start 1st process */
	if (kill (head->pid, SIGCONT)) {
		perror("kill");
		exit (1);
	}

	/* loop forever  until we exit from inside a signal handler. */
	while (pause())
		;

	/* Unreachable */
	fprintf(stderr, "Internal error: Reached unreachable point\n");
	return 1;
}
