#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int fd;
    int i;
    char in;

    if ((fd = open("/dev/rgb", O_RDWR)) == -1) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    // Read commands until failure
    while (1) {
        scanf("%c", &in);
        getchar();

        printf("Read '%c'\n",in);
        
        if (in == ' ') {
            break;
        } else {
            if (write(fd, &in, 1) < 0) {
                perror("Error while writing the file.\n");
            }
        }

    }

    close(fd);
    
    return 0;
}