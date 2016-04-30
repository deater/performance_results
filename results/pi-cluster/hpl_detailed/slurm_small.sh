#!/bin/bash

#SBATCH -p general	# partition (queue)
#SBATCH -N 12		# number of nodes
#SBATCH --ntasks-per-node=1
#SBATCH -o slurm.xhpl.%N.%j.out # STDOUT
#SBATCH -e slurm.xhpl.%N.%j.err # STDERR

date +"%s.%N"
sleep 5
date +"%s.%N"
mpirun ./xhpl
date +"%s.%N"
sleep 5
date +"%s.%N"

