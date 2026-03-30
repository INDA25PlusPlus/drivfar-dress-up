{ pkgs, ... }:
{
  # Used to find the project root
  projectRootFile = "flake.nix";

  # Formats *.nix
  programs.nixfmt.enable = true;
  # Formats *.json, etc.
  programs.prettier.enable = true;

  # Formats *.c, *.h
  programs.clang-format.enable = true;

  # Files to exclude from formatting.
  settings.global.excludes = [
    # This file is created and edited by the CSpell VSCode extension, which
    # formats it in its own way.
    "cspell.json"
  ];
}
