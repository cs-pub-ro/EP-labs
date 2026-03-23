#include <stdlib.h>
#include <string.h>
 
void leaky_function() {
    char *buf = malloc(256);
    strcpy(buf, "this memory will never be freed");
    /* buf is never passed to free() */
}
 
int main() {
    for (int i = 0; i < 10; i++)
        leaky_function();
    return 0;
}
