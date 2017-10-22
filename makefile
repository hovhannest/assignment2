COMPILER = "cc"

all: shell
	echo "done"

shell: HovnanShell.c
	$(COMPILER) HovnanShell.c -o shell
