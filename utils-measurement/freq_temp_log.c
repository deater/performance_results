/* Get freq and temp from a Raspberry Pi and log it */

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <signal.h>

static int done_running=0;

static void ctrlc_handler(int sig, siginfo_t *foo, void *bar) {

	done_running=1;
}


static double get_cpu_freq(void) {

	FILE *fff;
	double freq=0.0;

	fff=fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq","r");
	if (fff==NULL) return -1;

	fscanf(fff,"%lf",&freq);

	fclose(fff);

	return freq;
}

static double get_cpu_temp(void) {
	FILE *fff;
	double temp=-10000.0;

	fff=fopen("/sys/devices/virtual/thermal/thermal_zone0/temp","r");
	if (fff==NULL) return -10000;

	fscanf(fff,"%lf",&temp);

	fclose(fff);

	temp/=1000;

	return temp;
}

int main(int argc, char **argv) {

	struct timeval sample_time;
	double double_time=0.0;
	struct sigaction sa;

	/* setup control-c handler */
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_sigaction = ctrlc_handler;
	sa.sa_flags = SA_SIGINFO;

	if (sigaction( SIGINT, &sa, NULL) < 0) {
		fprintf(stderr,"Error setting up signal handler\n");
		return -1;
	}

	while(1) {

		gettimeofday(&sample_time,NULL);
		double_time=(double)sample_time.tv_sec+
                	((double)sample_time.tv_usec)/1000000.0;

		printf("%lf %lf %lf\n",
			double_time,get_cpu_freq(),get_cpu_temp());
		sleep(1);

		if (done_running) break;
	}

	return 0;
}
