#ifndef OS_H
#define OS_H

void OS_WRITE(const char *s);
void OS_READ(char *buffer, int max_length);

//PARTE DE INTS
int OS_ATOI(const char *s);
void OS_ITOA(int num, char *buffer);

#endif
