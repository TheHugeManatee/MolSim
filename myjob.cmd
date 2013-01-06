#!/bin/bash
#SBATCH -o /home/hpc/t1221/t1221an/MolSim/OutputFiles/myjob.%j.%N.out
#SBATCH -D /home/hpc/t1221/t1221an/MolSim
#SBATCH -J nukular
#SBATCH --clusters=ice1
#SBATCH --get-user-env
#SBATCH --mail-type=end
#SBATCH --mail-user=alexander.winkler@mytum.de
#SBATCH --time=10:00:00

cd MolSim
./MolSim -configFile sampleConfigs/sheet5_rayleigh_taylor.xml
