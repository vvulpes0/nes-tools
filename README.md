# NES Tools

This repository is a collection of tools created to aid in development
for the 8-bit home console known as the NES.

* [mk2bpp](#mk2bpp)

## mk2bpp

Convert a four-color image into a 2bpp image.  One can enable culling
of duplicate tiles with the `-c` option, or disable this culling with
`-C`.  Usage:

```
$ mk2bpp [-c|-C] file ... > out.chr
```

## Third-Party Software

The `stb_image.h` file comes from the [stb][1] suite
and is licensed as indicated in its own footer,
either the MIT license or the Unlicense, at your option.

[1]: https://github.com/nothings/stb
