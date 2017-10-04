writing a shell
===============

You are supposed to write a program, which does the following

* When started, it shows user a prompt. 
** it could be '$' sign, for example.
* It expects the user to enter a command.
** The "command" could have arguments. It could look like "ls -al", or "cc -o smth smth.c"
* When the command have been entered and the "Enter" key have been pressed, the program uses functions
** fork()
** wait()
** exec() family functions (evecvp, execle, etc)
in order to execute a given program.
```

				 Prompt <---------------------------------|
					|                                     |
				 Input received                           |
					|                                     |
				  fork()                                  |
					|                                     |
			  is this program a child?                    |
	   yes  |                         | no                |
			|                         |                   |
		execute the                wait for the child     |
		given program              process to end         |
		with arguments             with wait()            |
	by using execvp or                |                   |
	 its friends.                     --------------------| 
			|
			|
			|
		was exec family function
		able to run the other
		process?
 yes |                       | no
   	 |                       |
 	 |                print an error
	 |                       |
		 child exits.

```
