#!/bin/bash

#SBATCH -p general	# partition (queue)
#SBATCH -N 24		# number of nodes
#SBATCH --ntasks-per-node=1
#SBATCH -o slurm.xhpl.%N.%j.out # STDOUT
#SBATCH -e slurm.xhpl.%N.%j.err # STDERR

time mpirun ./xhpl

