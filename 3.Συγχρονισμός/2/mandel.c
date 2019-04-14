/*
 * mandel.c
 *
 * A program to draw the Mandelbrot Set on a 256-color xterm.
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
//added the next four
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>


#include "mandel-lib.h"

/*
 * POSIX thread functions do not return error numbers in errno,
 * but in the actual return value of the function call instead.
 * This macro helps with error reporting in this case.
 */
#define perror_pthread(ret, msg) \
	do { errno = ret; perror(msg); } while (0)

#define MANDEL_MAX_ITERATION 100000

/* Signal handler for SIGINT */
void user_pressed_Ctrl_C (int signum)
{
	reset_xterm_color(1);
	if (signal (SIGINT, SIG_DFL) == SIG_ERR) {
		perror("SIGNAL_DEFAULT\n");
		exit(1);
	}
	if (raise (SIGINT)) {
		printf("Could not raise SIGINT");
		exit(1);
	}
}

int safe_atoi(char *s, int *val)
{
	long l;
	char *endp;

	l = strtol(s, &endp, 10);
	if (s != endp && *endp == '\0') {
		*val = l;
		return 0;
	} else
		return -1;
}

void *safe_malloc(size_t size)
{
	void *p;

	if ((p = malloc(size)) == NULL) {
		fprintf(stderr, "Out of memory, failed to allocate %zd bytes\n",
			size);
		exit(1);
	}

	return p;
}

/*
 * A (distinct) instance of this structure
 * is passed to each thread
 */
struct thread_info_struct {
	int num;				//number of this thread
	int Number_Of_Threads;	//how many threads in total
	pthread_t tid;			/* POSIX thread id, as returned by the library */

	sem_t *mysem, *nextsem; /* Pointer to the semaphores I care about */
							// mysem -> myturn
							// nextsem -> next thread ready
};


/***************************
 * Compile-time parameters *
 ***************************/

/*
 * Output at the terminal is is x_chars wide by y_chars long
*/
int y_chars = 50;
int x_chars = 90;

/*
 * The part of the complex plane to be drawn:
 * upper left corner is (xmin, ymax), lower right corner is (xmax, ymin)
*/
double xmin = -1.8, xmax = 1.0;
double ymin = -1.0, ymax = 1.0;

/*
 * Every character in the final output is
 * xstep x ystep units wide on the complex plane.
 */
double xstep;
double ystep;

/*
 * This function computes a line of output
 * as an array of x_char color values.
 */
void compute_mandel_line(int line, int color_val[])
{
	/*
	 * x and y traverse the complex plane.
	 */
	double x, y;

	int n;
	int val;

	/* Find out the y value corresponding to this line */
	y = ymax - ystep * line;

	/* and iterate for all points on this line */
	for (x = xmin, n = 0; n < x_chars; x+= xstep, n++) {

		/* Compute the point's color value */
		val = mandel_iterations_at_point(x, y, MANDEL_MAX_ITERATION);
		if (val > 255)
			val = 255;

		/* And store it in the color_val[] array */
		val = xterm_color(val);
		color_val[n] = val;
	}
}

/*
 * This function outputs an array of x_char color values
 * to a 256-color xterm.
 */
void output_mandel_line(int fd, int color_val[])
{
	int i;

	char point ='@';
	char newline='\n';

	for (i = 0; i < x_chars; i++) {
		/* Set the current color, then output the point */
		set_xterm_color(fd, color_val[i]);
		if (write(fd, &point, 1) != 1) {
			perror("compute_and_output_mandel_line: write point");
			exit(1);
		}
	}

	/* Now that the line is done, output a newline character */
	if (write(fd, &newline, 1) != 1) {
		perror("compute_and_output_mandel_line: write newline");
		exit(1);
	}
}

void *thread_start_fn (void *arg)
{
	/* We know arg points to an instance of thread_info_struct */
	struct thread_info_struct *thr = arg;
	int i;

	for (i=thr->num; i<y_chars; i+=thr->Number_Of_Threads) {

		/*
		 * A temporary array, used to hold color values for the line being drawn
		 */
		int color_val[x_chars];
		compute_mandel_line(i, color_val);

		/* Wait for my turn */
		if (sem_wait(thr->mysem)) {
			perror("sem_wait");
			exit(1);
		}

		output_mandel_line(1, color_val);

		/* Next thread is ready */
		if (sem_post(thr->nextsem)) {
			perror("sem_post");
			exit(1);
		}
	}

	/* Thread is exiting */
	return NULL;
}


int main(int argc, char *argv[])
{
	int NTHREADS, i, ret;
	struct thread_info_struct *thr;

	/* Parse the command line */
	if (argc != 2) {
		fprintf(stderr, "Usage: %s NTHREADS\n\n"
		"Exactly one argument required:\n"
		"    NTHREADS: The number of threads to create.\n",
		argv[0]);
		exit(1);
	}

	if (safe_atoi(argv[1], &NTHREADS) < 0 || NTHREADS <= 0) {
		fprintf(stderr, "`%s' is not valid for `NTHREADS'\n", argv[1]);
		exit(1);
	}
	/* Done with parsing */

	xstep = (xmax - xmin) / x_chars;
	ystep = (ymax - ymin) / y_chars;

	sem_t sem[NTHREADS];		//create multiple semaphores & initialise
	if (sem_init(&sem[0], 0, 1)) {
		perror("Semaphore not initialised correctly\n");
		exit(1);
	}
	for (i=1; i<NTHREADS; i++) {
		if (sem_init(&sem[i], 0, 0)) {
			perror("Semaphore not initialised correctly\n");
			exit(1);
		}
	}

	thr = safe_malloc(NTHREADS * sizeof (struct thread_info_struct));

	if (signal (SIGINT, user_pressed_Ctrl_C) == SIG_ERR) {
		perror("SIGNAL_HANDLER\n");
		exit(1);
	}

	for (i=0; i<NTHREADS; i++) {
		/* Spawn new thread */
		thr[i].num = i;
		thr[i].Number_Of_Threads = NTHREADS;
		thr[i].mysem = &sem[i];
		thr[i].nextsem = &sem[(i+1) % NTHREADS];
		ret = pthread_create(&thr[i].tid, NULL, thread_start_fn, &thr[i]);
		if (ret) {
			perror_pthread(ret, "pthread_create");
			exit(1);
		}
	}


	/* Wait for all threads to terminate */
	for (i=0; i<NTHREADS; i++) {
		ret = pthread_join(thr[i].tid, NULL);
		if (ret) {
			perror_pthread(ret, "pthread_join");
			exit(1);
		}
	}

	/* Destroy all semaphores and free */
	for (i=0; i<NTHREADS; i++) {
		if (sem_destroy(&sem[i])) {
			perror("sem_destroy");
			exit(1);
		}
	}
	free(thr);
	reset_xterm_color(1);
	return 0;
}
