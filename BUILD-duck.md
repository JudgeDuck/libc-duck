To configure and build for duck target:

```bash
$ ./configure CFLAGS='-m32 -O2 -U_FORTIFY_SOURCE -fno-PIC' CXXFLAGS='-m32 -O2 -U_FORTIFY_SOURCE -fno-PIC' LDFLAGS=-m32 --disable-shared duck
$ make
```

Or run `build.sh`.

