# vbrfix

A command-line tool to repair MP3 Variable Bit Rate (VBR) headers.
It regenerates the Xing/LAME tag to correct duration and seekability issues in VBR files.

## Usage

```sh
vbrfix [options] <input.mp3> <output.mp3>

```

### Options

| Flag        | Description                                 |
|-------------|---------------------------------------------|
| `-makevbr`  | Force writing of the VBR tag (Recommended)  |
| `-always`   | Process the file even if it appears correct |
| `-lameinfo` | Keep existing LAME info if possible         |

**Example:**

```sh
vbrfix -makevbr podcast.mp3 podcast_fixed.mp3
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
