/*
 * Problema 4
 * Realizati un program care primeste in linia de comanda numele a doua
 * fisiere, sursa si dest, si copiaza sursa in dest.
 * Se verifica sa nu coincida sursa si dest.
 * Se lucreaza fara redirectari si duplicari.
 *
 * Utilizare: ex04 sursa dest
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
if (argc < 3)
    {
        printf("Utilizare: ex04 sursa dest\n");
        return 1;
    }

    if (_stricmp(argv[1], argv[2]) == 0)
    {
        printf("Fisierul sursa si destinatia nu pot fi identice!\n");
        return 1;
    }

    int in = _open(argv[1], _O_RDONLY | _O_BINARY);
    if (in == -1)
    {
        printf("Nu pot deschide fisierul sursa!\n");
        return 1;
    }

    int out = _open(argv[2], _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (out == -1)
    {
        printf("Nu pot crea fisierul destinatie!\n");
        _close(in);
        return 1;
    }

    char buf[4096];
    int n;
    while ((n = _read(in, buf, sizeof(buf))) > 0)
    {
        _write(out, buf, n);
    }

    _close(in);
    _close(out);

    return 0;
}
