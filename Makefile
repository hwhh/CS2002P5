shellsplit: main_parser.c
	gcc -o shellsplit main_parser.c

runcmds: main_run.c
	gcc -o runcmds main_run.c

runparallelcmds: run_parallel_cmds.c
	gcc -o runparallelcmds run_parallel_cmds.c

clean:
	rm *.o runcmds