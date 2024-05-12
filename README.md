# NES Tools

This repository is a collection of tools created to aid in development
for the 8-bit home console known as the NES.

* [mk2bpp](#mk2bpp)
* [nes2gb](#nes2gb)
* [unchr](#unchr)

## mk2bpp

Convert a four-color image into a 2bpp image.  One can enable culling
of duplicate tiles with the `-c` option, or disable this culling with
`-C`.  Usage:

```
$ mk2bpp [-c|-C] file ... >out.chr
```

## nes2gb

The `mk2bpp` program outputs in NES format.
The Game Boy uses a different 2bpp format.
So `nes2gb` translates the former into the latter.
Usage:

```
$ nes2gb <file.chr >file.bin
```

## unchr

Mainly for previewing purposes.
Convert a NES pattern table into a PGM image.
Usage:

```
# unchr <tiles.chr >tiles.pgm
```

## Third-Party Software

The `stb_image.h` file comes from the [stb][1] suite
and is licensed as indicated in its own footer,
either the MIT license or the Unlicense, at your option.

[1]: https://github.com/nothings/stb
