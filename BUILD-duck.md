To configure and build for duck target:

```bash
$ ./configure CFLAGS='-m32 -O2 -U_FORTIFY_SOURCE' CXXFLAGS='-m32 -O2 -U_FORTIFY_SOURCE' LDFLAGS=-m32 --disable-shared duck
$ make
```

