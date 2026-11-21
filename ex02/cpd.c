/*
- Problema 2
- Realizati programul cpd care muta cu acelasi nume toate fisierele din
- director_sursa in director_destinatie, impreuna cu structura arborescenta.
-   -a   se muta si fisierele ascunse
-   -dx  se muta fisiere pana la nivelul x de imbricare (1 = fara subdirectoare)
-   -lx  se muta doar fisierele cu lungimea <= x
- Se verifica sa nu fie destinatia in arborele directorului sursa.
 *
- Utilizare: cpd [fanion...] director_sursa director_destinatie
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>

int muta_ascunse = 0;
int adancime_max = -1;
long lungime_max = -1;

int este_in_sursa(const char *sursa, const char *dest)
{
    char s[260], d[260];
    snprintf(s, sizeof(s), "%s\\", sursa);
    snprintf(d, sizeof(d), "%s\\", dest);

    size_t ls = strlen(s);
    if (_strnicmp(s, d, ls) == 0)
        return 1;
    if (_stricmp(sursa, dest) == 0)
        return 1;
    return 0;
}

void muta_dir(const char *sursa, const char *dest, int nivel)
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

            if (!muta_ascunse && (fd.attrib & _A_HIDDEN))
                continue;

            char cale_s[260], cale_d[260];
            snprintf(cale_s, sizeof(cale_s), "%s\\%s", sursa, fd.name);
            snprintf(cale_d, sizeof(cale_d), "%s\\%s", dest, fd.name);

            if (fd.attrib & _A_SUBDIR)
            {
                if (adancime_max == -1 || nivel < adancime_max)
                {
                    muta_dir(cale_s, cale_d, nivel + 1);
                    _rmdir(cale_s);
                }
            }
            else
            {
                if (lungime_max == -1 || fd.size <= lungime_max)
                {
                    if (fd.attrib & _A_RDONLY)
                        _chmod(cale_s, _S_IREAD | _S_IWRITE);

                    if (rename(cale_s, cale_d) != 0)
                    {
                        FILE *in = fopen(cale_s, "rb");
                        FILE *out = fopen(cale_d, "wb");
                        if (in && out)
                        {
                            char buf[4096];
                            size_t n;
                            while ((n = fread(buf, 1, sizeof(buf), in)) > 0)
                                fwrite(buf, 1, n, out);
                            fclose(in);
                            fclose(out);
                            remove(cale_s);
                        }
                        else
                        {
                            if (in) fclose(in);
                            if (out) fclose(out);
                        }
                    }
                }
            }
        } while (_findnext(h, &fd) == 0);
        _findclose(h);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Utilizare: cpd [fanion...] director_sursa director_destinatie\n");
        return 1;
    }

    int idx = 1;
    while (idx < argc - 2)
    {
        if (strcmp(argv[idx], "-a") == 0)
        {
            muta_ascunse = 1;
        }
        else if (strncmp(argv[idx], "-d", 2) == 0)
        {
            adancime_max = atoi(argv[idx] + 2);
        }
        else if (strncmp(argv[idx], "-l", 2) == 0)
        {
            lungime_max = atol(argv[idx] + 2);
        }
        idx++;
    }

    const char *sursa = argv[argc - 2];
    const char *dest = argv[argc - 1];

    if (este_in_sursa(sursa, dest))
    {
        printf("Destinatia nu poate fi in interiorul directorului sursa!\n");
        return 1;
    }

    muta_dir(sursa, dest, 1);

    return 0;
}
