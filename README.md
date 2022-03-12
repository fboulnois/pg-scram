# `pg-scram`: Postgres `scram-sha-256` passwords

A tiny C program to create valid [Postgres `scram-sha-256`](https://www.postgresql.org/docs/14/auth-password.html)
passwords.

These can be used to automatically set and update database passwords without
accidentally logging the passwords in plaintext:

```sh
echo "my_super_secret_password" > secret.txt
echo "CREATE USER joe WITH PASSWORD :'PGPW';" | psql --set PGPW=$(cat secret.txt | pg-scram) -f -
```

`joe` now has the password `my_super_secret_password`, but the database logs
include this statement instead:

```sql
CREATE USER joe WITH PASSWORD 'SCRAM-SHA-256$4096:Hamvc7BOELb9OnEgS1dL/g==$jjR4ODiN5vqzbNUas2xSG08+1iZLWnghzjfXDx9p6yg=:/sRVoDORHCaojcWGg5ifOFx3NN7bzT/xgamQ7CSDpiE='
```

## How to build:

### Build locally

`pg-scram` only requires the `libpq` headers to build the code. Depending on
your distribution, these headers are included in the `libpq-dev`, `libpq-devel`,
or `postgresql-devel` packages.

To build the code run:

```sh
cc -I/usr/include/postgresql main.c -o pg-scram -lpq
```

The include paths and library paths might need to be adjusted to match your
distribution.

### Build using Docker

Two `Dockerfile`s are also available to build the code:

* [Dockerfile-debian](Dockerfile-debian) builds a dynamically linked version of
`pg-scram` for Debian 11.
* [Dockerfile-alpine](Dockerfile-alpine) builds a fully statically linked and
portable version of `pg-scram`.

## How to use:

Although `pg-scram` depends on `libpq`, it does not need a running Postgres
instance.

The password can be specified directly as a command line argument:

```sh
$ pg-scram "hello"
SCRAM-SHA-256$4096:if2MpZnsBy6B/TbMKd+siw==$xXmOAWEPkeDuJqT+N3wVVVY/eJL0vkpGQStEpoIKRw4=:KxdUo1wXlH5O4MCKzTX5E7iZbLxY1p9RX4o7WWbg89I=
```

The password can also be passed using a pipe:

```sh
$ echo "hello" | pg-scram
SCRAM-SHA-256$4096:1FZKq8e9WsX+p6fFN4pVQg==$Tn9TsEDc4v7UVpb7rCgvPVi2lCOck4ip1rQwKEg3coI=:X3ugMHIx12rIxYg2/6n/iaICOLl9dL6n0iyKxaQlZno=
```

For convenience with most shell utilities, the final newline is discarded by
default when using pipes. However, this behavior may be undesirable when piping
in files. To avoid this behavior, pass the `--no-trim` option:

```sh
$ cat secret.txt | pg-scram --no-trim
SCRAM-SHA-256$4096:BBsdc8M0Ir9WsCXXMYW3iw==$5rdGQRbwufOlEsrz1dlU1WYA32t5mAMz1Vv57rEP6Ww=:abldSFOQtYuZv5bNfJywrp6V/UtKkptS+DJ0nzx1sq8=
```
