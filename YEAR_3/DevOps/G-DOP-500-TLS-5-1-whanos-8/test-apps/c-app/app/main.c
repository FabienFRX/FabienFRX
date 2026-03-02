#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("Whanos C Test Application\n");
    printf("==========================\n\n");

    while (1) {
        printf("Hello from Whanos C app! Running...\n");
        sleep(5);
    }

    return 0;
}
