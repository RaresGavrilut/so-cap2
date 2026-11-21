/*
 * Problema 6
 * Realizati programul cpd care copiaza arborele de fisiere din sursa in dest.
 *   -a   se copiaza si fisierele ascunse
 *   -o   nu se suprascriu fisierele existente in destinatie
 *   -u   se sterg fisierele din sursa
 *   -dx  se copiaza fisierele pana la nivelul x de imbricare
 * Daca dest este in arborele de subdirectoare al lui sursa, programul
 * se opreste cu mesaj de eroare.
 *
 * Utilizare: cpd [fanion...] sursa dest
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

int main(int argc, char *argv[])
{
    int copiaza_ascunse = 0;
int nu_suprascrie = 0;
int sterge_sursa = 0;
int adancime_max = -1;

int este_in_sursa(const char *sursa, const char *dest)
{
    char s[260], d[260];
    snprintf(s, sizeof(s), "%s\\", sursa);
    snprintf(d, sizeof(d), "%s\\", dest);

    if (_strnicmp(s, d, strlen(s)) == 0)
        return 1;
    if (_stricmp(sursa, dest) == 0)
        return 1;
    return 0;
}

void copiaza_fisier(const char *cale_s, const char *cale_d)
{
    if (nu_suprascrie && _access(cale_d, 0) == 0)
        return;

    int in = _open(cale_s, _O_RDONLY | _O_BINARY);
    if (in == -1)
        return;

    int out = _open(cale_d, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (out == -1)
    {
        _close(in);
        return;
    }

    char buf[4096];
    int n;
    while ((n = _read(in, buf, sizeof(buf))) > 0)
    {
        _write(out, buf, n);
    }

    _close(in);
    _close(out);

    if (sterge_sursa)
    {
        _chmod(cale_s, _S_IREAD | _S_IWRITE);
        remove(cale_s);
    }
}

void copiaza_dir(const char *sursa, const char *dest, int nivel)
{
    char cautare[260];
    struct _finddata_t fd;
    intptr_t h;

    _mkdir(dest);

    snprintf(cautare, sizeof(cautare), "%s\\*", sursa);
    h = _findfirst(cautare, &fd);
    if (h != -1)
    {
        do
        {
            if (strcmp(fd.name, ".") == 0 || strcmp(fd.name, "..") == 0)
                continue;

            if (!copiaza_ascunse && (fd.attrib & _A_HIDDEN))
                continue;

            char cale_s[260], cale_d[260];
            snprintf(cale_s, sizeof(cale_s), "%s\\%s", sursa, fd.name);
            snprintf(cale_d, sizeof(cale_d), "%s\\%s", dest, fd.name);

            if (fd.attrib & _A_SUBDIR)
            {
                if (adancime_max == -1 || nivel < adancime_max)
                {
                    copiaza_dir(cale_s, cale_d, nivel + 1);
                    if (sterge_sursa)
                        _rmdir(cale_s);
                }
            }
            else
            {
                copiaza_fisier(cale_s, cale_d);
            }
        } while (_findnext(h, &fd) == 0);
        _findclose(h);
    }
}

    return 0;
}
