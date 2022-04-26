# Eco-Perf

## Purpose

The `eco-perf` package contains pure command-line tools to monitor
diverse ressource consumption metrics on Unix systems.

The first available tool is `etop`, an equivalent of `htop` allowing
users to have an information about processes that consumed,
cumulatively, a lot of resources during a long time (and not only a
snap-shot of resource consumption such as in `htop`).

Applications are built using the `cute_terminal` library providing a
simple interface to create and manipulate widgets inside a terminal.

## Package structure

Here is the tree representation of the sub-packages of `eco-perf`.
Individual files are not represented, only directories.
```
eco-perf
|_________________
|
|-- eco_perf      # Core library used in applications
|  |
|  |-- cute_terminal  # All utilities for terminal display
|  |  |-- definitions  # Several constant definitions and error handling
|  |  |-- io           # io and string utilities for the terminal-I/O
|  |  |-- terminal     # Utilities to manipulate the terminal window
|  |  |-- tools        # Tools to use in widgets
|  |  |-- widgets      # Built-in widgets of eco-perf
|  |     |-- layouts  # Layouts applied to widgets
|  |
|  |-- metrics             # System metrics (CPU, memory etc)
|_________________
|
|-- applications  # Terminal applications
|  |-- etop  # Equivalent of htop with different metrics
|_________________
|
|-- tests         # Test programs
```

## Setup instructions

Clone the repository
``` console
    git clone git@github.com:GregoireUhlrich/eco-perf.git
    # or
    # git clone https://github.com/GregoireUhlrich/eco-perf.git
```

Go in the `eco-perf` directory and prepare for build:
``` console
    cd eco-perf
    mkdir build
    cd build
```
Using cmake, Makefiles are generated automatically and the whole project
can then be built from sources using `make`:
``` console
    cmake ..
    make
```

Finally, from the `build/` directory, binaries can be directly launched:
``` console
    tests/test_terminal  # test colors and formats for the terminal
    applications/etop/etop  # etop -- htop for eco-perf
```

*Note:*
For now the `etop` program has no keyboard input so one must quit the
program using `Ctrl+C` in the terminal.
