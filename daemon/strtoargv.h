#ifndef __STRTOARGV_H
#define __STRTOARGV_H

#ifdef __cplusplus
extern "C" {
#endif

char **parsedargs(char *args, int *argc);
void freeparsedargs(char **argv);

#ifdef __cplusplus
}
#endif
#endif