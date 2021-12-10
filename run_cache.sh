#!/bin/bash



declare -a l3size=("512kB" "1MB" "4MB")
declare -a repl=("lru" "liprp" "biprp" "diprp" "rriprp" "brriprp" "drriprp")
declare -a benchmark=("bzip2" "cactusADM" "hmmer" "mcf")

for bench in "${benchmark[@]}"; do
	for pol in "${repl[@]}"; do
		for size in "${l3size[@]}"; do
			./build/X86/gem5.opt -d output_results/$bench/$pol/$size configs/spec2k6/run.py -b $bench --maxinsts=1000000000 --cpu-type=DerivO3CPU --fast-forward=1000000000 --warmup-insts=50000000 --standard-switch=50000000 --caches --l2cache --l3cache --l3_repl=$pol --l3_size=$size
		done
	done
done

wait 
