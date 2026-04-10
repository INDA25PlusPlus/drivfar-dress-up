default:
    just --list

# Remove all build files.
clean:
    @make clean

# Build the program's executable.
build:
    @make program

# Build and run the program.
run *args: build
    ./program {{args}}

test *args:
    @make build/tester
    ./build/tester {{args}}

# Format the project.
fmt:
    @make fmt

check_fmt:
    @make check_fmt
