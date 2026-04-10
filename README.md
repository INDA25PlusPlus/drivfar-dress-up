# Drivfar Dress-up

## Development

The project uses Make for building, but also provides a Just file with recipes for building and running the project:
```shell
just run
```

Run `just` to list all available recipes.

Before commiting, please run the following to make sure that formatting is automatically checked:
```shell
git config core.hooksPath .githooks
```

### Dependencies
Dependencies: You must have CSFML, `clang-format` and Just installed.

How to install the dependencies depending on your package manager:

> **Pacman**
> ```shell
> sudo pacman -S csfml clang just
> ```

> **Brew**
> ```shell
> brew install csfml clang-format just
> ```

> **Nix**  
> Just enter into the development shell!
> ```shell
> nix develop
> ```

> **Windows**  
> TODO: We should figure out how to do this

## Testing
This project uses the utest.h header-only testing framework. The tests are ran
using `just test`.

To add new tests, create a new C file under `tests/` to hold your new tests. See [tests/example.c](./tests/example.c) for how to format the file.
