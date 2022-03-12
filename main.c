#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libpq-fe.h>

int main(int argc, char **argv) {
    char passwd[BUFSIZ] = {0};
    /* parse arguments */
    int n = 0;
    int trim = 1;
    if(argc == 2) {
        if(!strcmp(argv[1], "--no-trim")) {
            trim = 0;
        } else {
            n = snprintf(passwd, BUFSIZ, "%s", argv[1]);
        }
    }
    if(!n && !isatty(fileno(stdin))) {
        n = fread(passwd, sizeof(char), BUFSIZ, stdin);
        if(trim && passwd[n-1] == '\n') {
            passwd[n-1] = '\0';
        }
    }
    if(n <= 0 || n >= BUFSIZ || argc < 1 || argc > 2) {
        return EXIT_FAILURE;
    }
    /* encrypt password */
    PGconn *conn = PQconnectdb("");
    char *pgpass = PQencryptPasswordConn(conn, passwd, "postgres", "scram-sha-256");
    printf("%s", pgpass);
    PQfreemem(pgpass);
    PQfinish(conn);
    return EXIT_SUCCESS;
}
