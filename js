#! /bin/bash
#PBS -o output
#PBS -e error
#PBS -l cput=800:00:00
mkdir -p $HOME/jobs 
tpdir=`echo $PBS_JOBID | cut -f 1 -d .`  
tempdir=$HOME/work/job$tpdir  
mkdir -p $tempdir  
cd $tempdir  
cp -R $PBS_O_WORKDIR/* .  
make clean
make
./test_aco_cu < input
mv ../job$tpdir $HOME/jobs/.

 
