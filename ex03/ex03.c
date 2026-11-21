/*
 * Problema 3
 * Realizati un program de cautare de fisier, specificat printr-un nume
 * extins (poate contine ?, *), pornind de la directorul curent.
 * Pentru fiecare fisier gasit se afiseaza calea completa si numele.
 *
 * Utilizare: ex03 tipar   (ex: ex03 *.txt)
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>
void cauta_fisiere(const char *dir, const char *tipar)
{
    char cautare[260];
    struct _finddata_t fd;
    intptr_t h;

    snprintf(cautare, sizeof(cautare), "%s\\%s", dir, tipar);
    h = _findfirst(cautare, &fd);
    if (h != -1)
    {
        do
        {
            if (strcmp(fd.name, ".") == 0 || strcmp(fd.name, "..") == 0)
                continue;

            if (!(fd.attrib & _A_SUBDIR))
            {
                printf("%s\\%s\n", dir, fd.name);
            }
        } while (_findnext(h, &fd) == 0);
        _findclose(h);
    }

    snprintf(cautare, sizeof(cautare), "%s\\*", dir);
    h = _findfirst(cautare, &fd);
    if (h != -1)
    {
        do
        {
            if (strcmp(fd.name, ".") == 0 || strcmp(fd.name, "..") == 0)
                continue;

            if (fd.attrib & _A_SUBDIR)
            {
                char subcale[260];
                snprintf(subcale, sizeof(subcale), "%s\\%s", dir, fd.name);
                cauta_fisiere(subcale, tipar);
            }
        } while (_findnext(h, &fd) == 0);
        _findclose(h);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Utilizare: ex03 tipar\n");
        return 1;
    }

    char dir_curent[260];
    if (_getcwd(dir_curent, sizeof(dir_curent)) == NULL)
    {
        return 1;
    }

    cauta_fisiere(dir_curent, argv[1]);

    return 0;
}
