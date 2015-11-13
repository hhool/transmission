#include <stdio.h>
#include "strtoargv.h"

static int setargs(char *args, char **argv)
{
   int count = 0;

   while (isspace(*args)) ++args;
   while (*args) {
	 if (argv) argv[count] = args;
	 while (*args && !isspace(*args)) ++args;
	 if (argv && *args) *args++ = '\0';
	 while (isspace(*args)) ++args;
	 count++;
   }
   return count;
}

char **parsedargs(char *args, int *argc)
{
   char **argv = NULL;
   int    argn = 0;

   if (args && *args
	&& (args = strdup(args))
	&& (argn = setargs(args,NULL))
	&& (argv = malloc((argn+1) * sizeof(char *)))) {
	  *argv++ = args;
	  argn = setargs(args,argv);
   }

   if (args && !argv) free(args);

   *argc = argn;
   return argv;
}

void freeparsedargs(char **argv)
{
  if (argv) {
	free(argv[-1]);
	free(argv-1);
  } 
}