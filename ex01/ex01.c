/*
 * Problema 1
 * Realizati un program care primeste in linia de comanda un nume de
 * director pe care trebuie sa il stearga, impreuna cu tot continutul
 * (fisiere si subdirectoare). Parcurgerea se face cu o functie recursiva.
 * Fisierele read-only se fac mai intai normale, apoi se sterg.
 *
 * Utilizare: ex01 director
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Utilizare: ex01 director\n");
        return 1;
    }

    sterge_tot(argv[1]);
    void sterge_tot(const char *cale)
{
    char cautare[260];
    struct _finddata_t fd;
    intptr_t h;

    snprintf(cautare, sizeof(cautare), "%s\\*", cale);
    h = _findfirst(cautare, &fd);
    if (h != -1)
    {
        do
        {
            if (strcmp(fd.name, ".") == 0 || strcmp(fd.name, "..") == 0)
                continue;

            char subcale[260];
            snprintf(subcale, sizeof(subcale), "%s\\%s", cale, fd.name);

            if (fd.attrib & _A_SUBDIR)
            {
                sterge_tot(subcale);
            }
            else
            {
                if (fd.attrib & _A_RDONLY)
                {
                    _chmod(subcale, _S_IREAD | _S_IWRITE);
                }
                remove(subcale);
            }
        } while (_findnext(h, &fd) == 0);
        _findclose(h);
    }

    _rmdir(cale);
}

    return 0;
}
