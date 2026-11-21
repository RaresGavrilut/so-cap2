/*
 * Problema 5
 * Se considera un fisier existent, "test.tst". Adaugati la sfarsitul lui
 * toate cifrele care ii apartin.
 *   initial: abc123de6fghi12jk
 *   final:   abc123de6fghi12jk123612
 * Cifrele se memoreaza intr-un buffer. Cand bufferul e plin: se retine
 * pozitia curenta, se sare la EOF, se scrie bufferul, se revine.
 * Citirile nu se fac dincolo de EOF-ul initial.
 *
 * Utilizare: ex05 test.tst
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    const char *nume = (argc >= 2) ? argv[1] : "test.tst";

    int fd = _open(nume, _O_RDWR | _O_BINARY);
    if (fd == -1)
    {
        printf("Nu pot deschide fisierul %s!\n", nume);
        return 1;
    }

    long eof_initial = _lseek(fd, 0, SEEK_END);
    _lseek(fd, 0, SEEK_SET);

    char buf[256];
    char cifre[16];
    int nr_cifre = 0;
    long cititi = 0;

    while (cititi < eof_initial)
    {
        int de_citit = sizeof(buf);
        if (eof_initial - cititi < de_citit)
            de_citit = (int)(eof_initial - cititi);

        int n = _read(fd, buf, de_citit);
        if (n <= 0)
            break;

        cititi += n;

        for (int i = 0; i < n; i++)
        {
            if (buf[i] >= '0' && buf[i] <= '9')
            {
                cifre[nr_cifre++] = buf[i];

                if (nr_cifre == sizeof(cifre))
                {
                    long poz_curenta = _lseek(fd, 0, SEEK_CUR);
                    _lseek(fd, 0, SEEK_END);
                    _write(fd, cifre, nr_cifre);
                    _lseek(fd, poz_curenta, SEEK_SET);
                    nr_cifre = 0;
                }
            }
        }
    }

    if (nr_cifre > 0)
    {
        _lseek(fd, 0, SEEK_END);
        _write(fd, cifre, nr_cifre);
    }

    _close(fd);

    return 0;
}
