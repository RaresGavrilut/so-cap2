/*
 * Problema 7
 * Realizati programul cauta care cauta, recursiv, fisierele cu extensia
 * ext care contin sirul text (in continutul fisierului, nu in nume).
 *   -cx  textul trebuie sa apara de cel putin x ori in fisier
 *
 * Utilizare: cauta [fanion] ext text
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>
#include <fcntl.h>
int aparitii_min = 1;

int numara_aparitii(const char *cale, const char *text)
{
    FILE *f = fopen(cale, "rb");
    if (!f)
        return 0;

    char linie[1024];
    int total = 0;
    size_t len = strlen(text);

    if (len == 0)
    {
        fclose(f);
        return 0;
    }

    while (fgets(linie, sizeof(linie), f) != NULL)
    {
        char *p = linie;
        while ((p = strstr(p, text)) != NULL)
        {
            total++;
            p += len;
        }
    }

    fclose(f);
    return total;
}

void cauta_in_dir(const char *dir, const char *ext, const char *text)
{
    char cautare[260];
    struct _finddata_t fd;
    intptr_t h;

    /* Cauta fisierele cu extensia data in directorul curent */
    if (ext[0] == '.')
        snprintf(cautare, sizeof(cautare), "%s\\*%s", dir, ext);
    else
        snprintf(cautare, sizeof(cautare), "%s\\*.%s", dir, ext);

    h = _findfirst(cautare, &fd);
    if (h != -1)
    {
        do
        {
            if (strcmp(fd.name, ".") == 0 || strcmp(fd.name, "..") == 0)
                continue;

            if (!(fd.attrib & _A_SUBDIR))
            {
                char cale[260];
                snprintf(cale, sizeof(cale), "%s\\%s", dir, fd.name);

                if (numara_aparitii(cale, text) >= aparitii_min)
                {
                    printf("%s\n", cale);
                }
            }
        } while (_findnext(h, &fd) == 0);
        _findclose(h);
    }

    /* Parcurge recursiv subdirectoarele */
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
                cauta_in_dir(subcale, ext, text);
            }
        } while (_findnext(h, &fd) == 0);
        _findclose(h);
    }
}

int main(int argc, char *argv[])
{
   if (argc < 3)
    {
        printf("Utilizare: cauta [fanion] ext text\n");
        return 1;
    }

    int idx = 1;
    if (strncmp(argv[idx], "-c", 2) == 0)
    {
        aparitii_min = atoi(argv[idx] + 2);
        idx++;
    }

    if (argc - idx < 2)
    {
        printf("Utilizare: cauta [fanion] ext text\n");
        return 1;
    }

    const char *ext = argv[idx];
    const char *text = argv[idx + 1];

    char dir_curent[260];
    if (_getcwd(dir_curent, sizeof(dir_curent)) == NULL)
        return 1;

    cauta_in_dir(dir_curent, ext, text);

    return 0;
}
