# vbrfix

A command-line tool to repair MP3 Variable Bit Rate (VBR) headers.
It regenerates the Xing/LAME tag to correct duration and seekability issues in VBR files.

## Usage

```sh
vbrfix [options] <input.mp3> <output.mp3>
```

The output file is only written when the file needs fixing. If the existing Xing header already looks correct,
or the file is not VBR, nothing is written and the exit code is still 0. Input and output must be different files.

### Options

Case sensitive, accepted with one or two leading dashes (`-removeId3v2` and `--removeId3v2` are the same).
Giving any option disables the "Xing header already looks good, skip" shortcut.

| Option                       | Description                                              |
|------------------------------|----------------------------------------------------------|
| `--removeId3v1`              | Remove the ID3v1 tag                                     |
| `--removeId3v2`              | Remove the ID3v2 tag                                     |
| `--removeUnknown`            | Remove data that is not recognised as MP3 or a known tag |
| `--removeLame`               | Drop the LAME info from the new Xing frame               |
| `--keepLame`                 | Copy the LAME info as is (default)                       |
| `--keepLameUpdateCrc`        | Copy the LAME info and recompute its CRC                 |
| `--XingFrameCrcProtectIfCan` | Keep CRC protection on the Xing frame when possible      |
| `--version`, `--help`, `-h`  | Print version / usage and exit                           |

**Example:**

```sh
vbrfix --keepLameUpdateCrc podcast.mp3 podcast_fixed.mp3
```

## Build

**Requirements:**

* CMake 3.20+
* C++17 compliant compiler (GCC, Clang, MSVC)

**Compilation:**

```sh
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

**Installation (Optional):**

```sh
sudo cmake --install .
```

## License

GPL-2.0-or-later. See [LICENSE](LICENSE) for details.
Original code by William Pye (2006). Modernized fork.
