# Eco-Perf

## Purpose

The `eco-perf` package contains pure command-line tools to monitor
diverse ressource consumption metrics on Unix systems.

The first available tool is `etop`, an equivalent of `htop` allowing
users to have an information about processes that consumed,
cumulatively, a lot of resources during a long time (and not only a
snap-shot of resource consumption such as in `htop`).

Applications are built using `term-widgets`, a library providing a simple
interface to create and manipulate widgets inside a terminal.

## Setup instructions

Clone the repository
``` console
    git clone https://github.com/GregoireUhlrich/eco-perf.git
```

Build from sources
``` console
    cd eco-perf
    mkdir build
    cd build
    cmake ..
    make
    cd ..
```

Finally, execute the programs
``` console
    build/test_terminal  # test colors and formats for the terminal
    build/etop  # etop -- htop for eco-perf
```

Quit the `etop` program using `Ctrl+C` in the terminal.
