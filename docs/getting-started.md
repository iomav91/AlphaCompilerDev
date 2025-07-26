## II. Getting Started

This section provides instructions on how to build the Alpha compiler from source and run Alpha programs.

### Prerequisites

Before you can build the Alpha compiler, you will need the following software installed on your system:

*   **C++ Compiler:** A C++ compiler that supports C++11 or later is required.  Recommended compilers:
    *   **GCC:** (GNU Compiler Collection) version 5.0 or later.  Available on most Linux distributions.  To check your GCC version, run `g++ --version`.
    *   **Clang:** (Clang Compiler Infrastructure) version 3.5 or later.  Available on macOS and Linux.  To check your Clang version, run `clang++ --version`.
    *   **Microsoft Visual C++:** (MSVC) version 14.0 (Visual Studio 2015) or later.  Available on Windows.

*   **Git (Optional):**  Git is a version control system used to clone the Alpha compiler source code from GitHub.  You only need Git if you plan to download the source code from the repository.  Download it from [git-scm.com](https://git-scm.com/downloads) or install it using your system's package manager.

### Building the Compiler

These instructions assume you are using a Unix-like operating system (Linux, macOS, or WSL on Windows).  The steps may vary slightly on Windows with MSVC.

1.  **Clone the Repository (Optional):**

    If you haven't already, clone the Alpha compiler repository from GitHub:

    ```bash
    git clone https://github.com/iomav91/AlphaCompilerDev.git
    cd AlphaCompilerDev
    ```

2.  **Build the Compiler:**

    Use `make` (or your system's equivalent build tool) to compile the Alpha compiler.

    ```bash
    make
    ```

    This will create the `al` (Alpha Compiler) and `avm` (Alpha Virtual Machine) executables in the `build` directory.

### Running Alpha Programs

Once you have built the Alpha compiler, you can use it to compile and run Alpha programs.

1.  **Create an Alpha Source File:**

    Create a text file with the `.alpha` extension (e.g., `hello.alpha`) and write your Alpha code in it.  For example:

    ```alpha
    // hello.alpha
    program {
        print("Hello, world!");
    }
    ```

2.  **Compile the Alpha Program:**

    Use the `al` executable to compile the Alpha source file into bytecode.

    ```bash
    ./al hello.alpha
    ```

    This will generate a bytecode file named `tcode.abc` (Alpha Bytecode).  If there are syntax or semantic errors in your Alpha code, the compiler will report them at this stage.

3.  **Run the Bytecode:**

    Use the `avm` executable to execute the bytecode file.

    ```bash
    ./avm tcode.abc
    ```

    This will run the Alpha program and print the output to the console.  In this example, it will print "Hello, world!".

### Example: Building and Running "Hello, World!"

Here's a complete example demonstrating how to build the compiler and run a simple "Hello, world!" program:

```bash
# Clone the repository
git clone https://github.com/iomav91/AlphaCompilerDev.git
cd AlphaCompilerDev

# Run CMake
cmake ..

# Build the compiler
make

# Create a hello.alpha file
cat <<EOF > hello.alpha
program {
    print("Hello, world!");
}
EOF

# Compile the Alpha program
./al hello.alpha

# Run the bytecode
./avm hello.abc
