{ pkgs, ... }:
{
  # Used to find the project root
  projectRootFile = "flake.nix";

  # Formats *.nix
  programs.nixfmt.enable = true;

  # Formats *.c, *.h
  programs.clang-format.enable = true;

  # Files to exclude from formatting.
  settings.global.excludes = [ ];
}
