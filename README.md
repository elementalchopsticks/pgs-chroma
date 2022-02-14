<div align="center">

# pgs-chroma

`pgs-chroma` is a tool for modifying the palette of Presentation Graphics
Stream (PGS) files, the image-based subtitle format used for Blu-ray media.

[![License](https://img.shields.io/github/license/elementalchopsticks/pgs-chroma)](https://www.gnu.org/licenses/gpl-3.0.en.html)
[![Latest Release](https://img.shields.io/github/v/release/elementalchopsticks/pgs-chroma)](https://github.com/elementalchopsticks/pgs-chroma/releases/latest)
[![Release Date](https://img.shields.io/github/release-date/elementalchopsticks/pgs-chroma)](#)

</div>

### Rationale

Yellow subtitles are (subjectively) ugly.

### Example Usage

```shell
# Convert a file using defaults
$ pgs-chroma input.sup output.sup

# Convert a file using the "black and white" filter
$ pgs-chroma -f bw input.sup output.sup
```

### Dependencies

- [Meson](https://mesonbuild.com/) [^1]

[^1]: Build dependency, not required at runtime.
