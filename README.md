# 42sh

42sh is a Unix Shell implementation in C.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

To build and run the 42sh shell, you will need:

* A Unix-based system
* The GCC C compiler (version 4.2 or later)

### Installing

To install and run the 42sh shell, follow these steps:

1. Clone the repository to your local machine:
git clone https://github.com/schmittnicolas/42sh.git


2. Navigate into the cloned directory:
cd 42sh

3. Create a build directory: meson setup builddir

4. Build the project with Ninja: ninja -C builddir

5. Run the 42sh shell: ./builddir/42sh script.sh
