#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

int getopt(int argc, char *const argv[], const char *optstring);

char *default_message(char message[], char message2[]) {
    int k, l = 0;
    for (k=0;k<=1003;k++) {
        if (isdigit(message[k]) == 0 & ispunct(message[k]) == 0) {
            message2[l] = message[k];
            l++;
        }
    }
    return message2;
}

char *digit(char message[], char message2[]) {
    int k, l = 0;
    for (k=0;k<=1003;k++) {
        if (ispunct(message[k]) == 0) {
            message2[l] = message[k];
            l++;
        }
    }
    return message2;
}

char *punct(char message[], char message2[]) {
    int k, l = 0;
    for (k=0;k<=1003;k++) {
        if (isdigit(message[k]) == 0) {
            message2[l] = message[k];
            l++;
        }
    }
    return message2;
}

char *replace_sensitive(char message[],char message2[], int optarg_length, char optarg_in_argv[]) {
    int y, x = 0, alnum_count = 0, start = 0, replace_len;
    while (x <= 1003) {
        if (isalnum(message[x])) {
            start = x;
            alnum_count = 0;
            while (isalnum(message[x])) {
                alnum_count++;
                x++;
            }
        } else {
            x++;
        }
        if (alnum_count <= optarg_length) {
            replace_len = alnum_count;
        } else {
            replace_len = optarg_length;
        }
        y = 0;
        while (y < replace_len) {
            message2[start + y] = optarg_in_argv[y];
            y++;
        }
    }
    return message2;
}

char *insensitive(char message[],char message2[]) {
    int z = 0;
    while (message[z] != '\0') {
        if (isupper(message[z])) {
            message2[z] = toupper(message2[z]);
        } else if (islower(message[z])) {
            message2[z] = tolower(message2[z]);
        }
        z++;
    }
    return message2;
}

char *Replace_insensitive(char message[],char message2[], int optarg_length, char optarg_in_argv[]) {
    replace_sensitive(message, message2, optarg_length, optarg_in_argv);
    insensitive(message, message2);
    return message2;
}


int main(int argc, char *argv[]) {
    int opt, d_option = 0, p_option = 0, u_option = 0, l_option = 0, sensitive_replace = 0, insensitive_Replace = 0, j;
    int non_option_count = 0, optarg_length;
    char *optarg_in_argv;
    char message[1003];
    char* optstring = "-:dpulr:R:";

    while ((opt = getopt(argc, argv, optstring)) != -1) {
        switch (opt) {
            case 'd':
                d_option = 1;
                break;
            case 'p':
                p_option = 1;
                break;
            case 'u':
                u_option = 1;
                break;
            case 'l':
                l_option = 1;
                break;
            case 'r':
                sensitive_replace = 1;
                optarg_in_argv = optarg;
                break;
            case 'R':
                insensitive_Replace = 1;
                optarg_in_argv = optarg;
                break;
            case '?':
                return 1;
            case ':':
                return 2;
        }
    }
    if (optind < argc) {
        for (j=optind;j<argc;j++) {
            non_option_count++;
            printf("Non-option CMD argument: %s\n", argv[j]);
        }
    }

    //optarg
    int optarg_position = argc - 1 - non_option_count;
    optarg_length = strlen(argv[optarg_position]);

    int o,p;
    char message2[1003];

    while (strcmp(fgets(message, 1003, stdin), "\n") != 0) {
        if (u_option == 1) {
            for (o=0;o<=1003;o++) {
                message[o] = toupper(message[o]);
            }
        } else if (l_option == 1) {
            for (p=0;p<=1003;p++) {
                message[p] = tolower(message[p]);
            }
        }

        strcpy(message2, message);

        if (d_option == 0 & p_option == 0) {
            default_message(message,message2);
            strcpy(message, message2);
        } else if (d_option == 1 & p_option == 1) {
            int i = 1;
        } else {
            if (d_option == 1) {
                digit(message, message2);
            }
            if (p_option == 1) {
                punct(message, message2);
            }
        }
        strcpy(message, message2);
        if (sensitive_replace == 1) {
            replace_sensitive(message, message2, optarg_length, optarg_in_argv);
        } else if (insensitive_Replace == 1) {
            Replace_insensitive(message, message2, optarg_length, optarg_in_argv);
        }
        strcpy(message, message2);
        printf("%s", message);
    }
    return 0;
}