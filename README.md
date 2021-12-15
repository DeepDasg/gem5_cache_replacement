# gem5_cache_replacement
Combined Setup- DIP implemented by Abhishek Nayak, DRRIP implemented by Deepayan Dasgupta

Output results (for K=32) in the folder - output_results/K32/...
Output results (for K=64) in the folder - output_results/K64/...

Benchmarks run -
1. bzip2
2. cactusADM
3. hmmer
4. mcf
5. namd
6. sjeng
7. sphinx3

Command for building gem5 library:
scons-3 -j 4 build/X86/gem5.opt

Command line to run simulation:
./build/X86/gem5.opt -d output_results/K32/<benchmark>/<policy>/<LLC_size> configs/spec2k6/run.py -b <benchmark> --maxinsts=1000000000 --cpu-type=DerivO3CPU --fast-forward=1000000000 --warmup-insts=50000000 --standard-switch=50000000 --caches --l2cache --l3cache --l3_repl=<policy> --l3_size=<LLC_size>
