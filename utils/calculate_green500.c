#include <stdio.h>
#include <string.h>

/* Calculates GFLOPS/W */
/* Note, wattsup only has 1s precision */
/* The time stamps don't necessarily line up */
/* In that case we count a little extra in the first interval */
/* And drop the trailing interval */

int main(int argc, char **argv) {

	FILE *fff;
	double idle_start;
	double start_time,stop_time,gigaflops,total_time;
	char temp_string[BUFSIZ];
	char *result;
	int tv_count=0;

	if (argc<3) {
		fprintf(stderr,"Usage: %s hpl_file power_file\n\n",argv[0]);
		return -1;
	}

	/* Read HPL file */
	printf("Reading HPL file:\n");

	fff=fopen(argv[1],"r");
	if (fff==NULL) {
		fprintf(fff,"Error opening %s\n",argv[1]);
		return -1;
	}

	fgets(temp_string,BUFSIZ,fff);
	sscanf(temp_string,"%lf",&idle_start);
	fgets(temp_string,BUFSIZ,fff);
	sscanf(temp_string,"%lf",&start_time);

	printf("\tHPL start time: %lf\n",start_time);

	while(1) {
		result=fgets(temp_string,BUFSIZ,fff);
		if (result==NULL) break;

		if (!strncmp(temp_string,"T/V",3)) {
			tv_count++;
		}
		if (tv_count==2) break;
	}
	/* ---- */
	fgets(temp_string,BUFSIZ,fff);
	fgets(temp_string,BUFSIZ,fff);
	sscanf(temp_string,"%*s %*d %*d %*d %*d %*f %lf",&gigaflops);

	while(1) {
		result=fgets(temp_string,BUFSIZ,fff);
		if (result==NULL) break;

		if (!strncmp(temp_string,"End of Tests.",13)) break;
	}

	/* ========= */
	fgets(temp_string,BUFSIZ,fff);
	fgets(temp_string,BUFSIZ,fff);
	sscanf(temp_string,"%lf",&stop_time);
	fclose(fff);

	printf("\tHPL gigaflops: %lf\n",gigaflops);
	printf("\tHPL stop time: %lf\n",stop_time);
	total_time=stop_time-start_time;
	printf("\tTotal time: %lf\n",total_time);
	printf("\n");

	/* Read the power output */

	double first_time=0.0,last_time=0.0;
	double current_time=0.0,prev_time=0.0;
	double wup_start=0.0,wup_stop=0.0;
	double temp_joules,total_joules=0,average_watts,current_watts;
	double max_power=0.0;
	int ret;
	int accumulating=0,accum_idle=0;
	double idle_joules=0.0;

	printf("Reading power file\n");

	fff=fopen(argv[2],"r");
	if (fff==NULL) {
		fprintf(stderr,"Error opening %s\n",argv[2]);
		return -1;
	}

	while(1) {

		if (accum_idle) {
			if (current_time-prev_time > 1.1) {
				printf("ERRROR!  FIXME: missed sample\n");
			}
			temp_joules=current_watts/(current_time-prev_time);
			idle_joules+=temp_joules;
		}

		prev_time=current_time;

		ret=fscanf(fff,"%lf %lf",&current_time,&current_watts);
		if (ret<1) break;

		if (first_time==0.0) {
			first_time=current_time;
			printf("\tFirst time in file: %lf\n",first_time);
		}

		if ((accum_idle==0) && (current_time>idle_start)) {
			printf("\tStarting Idle at %lf\n",current_time);
			accum_idle=1;
		}

		if (current_time>start_time) {
			printf("\tStarting Energy at %lf\n",current_time);
			wup_start=current_time;
			accumulating=1;
			break;
		}



	}

	while(1) {

		if (accumulating) {
			if (current_time-prev_time > 1.1) {
				printf("ERRROR!  FIXME: missed sample\n");
			}
			temp_joules=current_watts/(current_time-prev_time);
			total_joules+=temp_joules;
//			printf("Accumulating: %lf, %lf %lf\n",
//				current_watts,total_joules,current_time-prev_time);

		}

		prev_time=current_time;

		ret=fscanf(fff,"%lf %lf",&current_time,&current_watts);
		if (ret<1) break;

		if (current_watts>max_power) max_power=current_watts;

		if ((accumulating) && (current_time>stop_time)) {
			printf("\tStopping Energy at %lf\n",current_time);
			wup_stop=current_time;
			accumulating=0;
		}


	}
	last_time=prev_time;

	fclose(fff);

	average_watts=total_joules/(wup_stop-wup_start);

	printf("\tLast time in file: %lf\n",last_time);

	printf("\tTotal Energy: %lf J\n",total_joules);
	printf("\tAverage Power: %lf/%lf = %lf W\n",
			total_joules,(wup_stop-wup_start),average_watts);

	printf("\tAverage Idle Power: %lf/%lf = %lf W\n",
		idle_joules,(wup_start-idle_start),
		idle_joules/(wup_start-idle_start));
	printf("\tMax Power: %lf W\n",max_power);
	printf("\n\n");
	printf("GIGAFLOPS/W = %lf\n",gigaflops/average_watts);

	return 0;
}
