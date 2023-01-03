# My fork

[Original here](https://github.com/paroj/gltut), this is my own fork. 
I've adapted it and the `README.md` file to work with my Arch Linux
machine, which only officially supports [OpenGL](https://wiki.archlinux.org/title/OpenGL)
up to 3.1, but uses [Mesa](https://archlinux.org/packages/extra/x86_64/mesa/) for OpenGL 3.3 support.

My own stuff will be on the `srp` branch.

# OpenGL Tutorials

This is a series of tutorials on using OpenGL to do graphical rendering.
The code for each tutorial is found in the `Tut*` directories. The code
alone is not enough information to understand what is going on. The actual
documentation is available in several forms.

The HTML form of the documentation can be found on: https://paroj.github.io/gltut/

The [License.txt](License.txt) file contains the licensing information for the materials distributed in these tutorials.

In case you prefer Java to C++, Giuseppe Barbieri [created a Java port here](https://github.com/elect86/modern-jogl-examples).

## Building the tutorials

- `sudo pacman -S cmake`
- `mkdir build && cd build`
- `cmake ..`
- `cmake --build . --config Release`

### Software

- [CMake](https://archlinux.org/packages/extra/x86_64/cmake/)

## Rebuilding the HTML Documentation

- make sure you have [lua](https://wiki.archlinux.org/title/Lua) installed (`sudo pacman -S lua`)
- run `sudo pacman -S libxslt` or install [libxslt-git](https://aur.archlinux.org/packages/libxslt-git) from the AUR
- run `sudo pacman -S lua-filesystem docbook-xsl`
- run `./build-webdoc`

NOTE: I changed the file at `Documents/Build/_buildConfig.lua`, in particular
the `docbookXSLBasepath` property. It depends on the fact that after installing
`libxslt` there is a file at 
`/usr/share/xml/docbook/xsl-stylesheets-1.79.2/html/chunkfast.xsl`.

### Software

- [lua](https://archlinux.org/packages/extra/x86_64/lua/)
- [libxslt](https://archlinux.org/packages/extra/x86_64/libxslt/)
- [lua-filesystem](https://archlinux.org/packages/community/x86_64/lua-filesystem/)
- [docbook-xsl](https://archlinux.org/packages/extra/any/docbook-xsl/)

## Viewing the HTML Documentation

REMINDER: don't sandbox your browser when trying to view local files or it won't find them.

### Method 1: Bash script

- make sure the HTML documentation is built
- the first time using it, edit `build-webdoc` and set it to your preferred browser
- run `./webdoc` whenever you want to see it

### Method 2: Manual

- make sure the HTML documentation is built
- open the file at `website/index.html` on your preferred browser directly

## Rebuilding the PDF Documentation

- install the packages needed for HTML
- `apt-get install fop libxerces2-java libsaxon-java libfop-java libjeuclid-fop-java`
- add the line `find_jars jeuclid-core jeuclid-fop` to `/usr/bin/fop`
- inside `Documents/Build` run `lua BuildComputerFO.lua`

## Known issues

### Exception in thread "main" java.lang.NoClassDefFoundError: org/w3c/dom/events/CustomEvent
Some versions of `fop` will not work correctly.
Installing `2.6` from here https://xmlgraphics.apache.org/fop/download.html fixes it.
