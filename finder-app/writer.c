//
// Created by joey on 12/13/23.
//
#include <syslog.h>
#include <stdio.h>


int main(int argc, char **argv){
    openlog(NULL, 0, LOG_USER);
    if (argc-1 != 2) {
        syslog(LOG_ERR, "Invalid Number of arguments: %d", argc-1);
        return 1;
    }
    char* filename = argv[1];
    char* text = argv[2];
    FILE *file  = fopen(filename, "w+");
    if (file == NULL){
        syslog(LOG_ERR, "Failed to open/create file");
        return 1;
    }
    syslog(LOG_DEBUG,"Writing %s to %s", text, filename);
    int rv = fputs(text, file);
    if (rv == EOF) {
        syslog(LOG_ERR, "Failed to write to file");
        return 1;
    }
}
