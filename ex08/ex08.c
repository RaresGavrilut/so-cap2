/*
 * Problema 8
 * Realizati o aplicatie care sorteaza continutul unui fisier folosind
 * algoritmul Bubble Sort. Nu se accepta folosirea unui tablou,
 * ci numai folosirea functiei lseek.
 *
 * Utilizare: ex08 fisier
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Utilizare: ex08 fisier\n");
        return 1;
    }

    int fd = _open(argv[1], _O_RDWR | _O_BINARY);
    if (fd == -1)
    {
        printf("Nu pot deschide fisierul %s!\n", argv[1]);
        return 1;
    }

    long n = _lseek(fd, 0, SEEK_END);
    if (n <= 1)
    {
        _close(fd);
        return 0;
    }

    int schimbat;
    char c1, c2;

    do
    {
        schimbat = 0;
        for (long i = 0; i < n - 1; i++)
        {
            _lseek(fd, i, SEEK_SET);
            _read(fd, &c1, 1);
            _read(fd, &c2, 1);

            if ((unsigned char)c1 > (unsigned char)c2)
            {
                _lseek(fd, i, SEEK_SET);
                _write(fd, &c2, 1);
                _write(fd, &c1, 1);
                schimbat = 1;
            }
        }
    } while (schimbat);

    _close(fd);

    return 0;
}
