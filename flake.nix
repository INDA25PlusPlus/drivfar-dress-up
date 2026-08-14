{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
    treefmt-nix.url = "github:numtide/treefmt-nix";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      treefmt-nix,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        name = "drivfar-dress-up";
        pkgs = nixpkgs.legacyPackages.${system};
        treefmtEval = treefmt-nix.lib.evalModule pkgs ./treefmt.nix;
      in
      {
        packages.default = pkgs.stdenv.mkDerivation {
          pname = name;
          version = self.shortRev or self.dirtyShortRev;
          src = ./.;

          nativeBuildInputs = [
            pkgs.csfml
            pkgs.pkg-config
          ];

          buildPhase = ''
            make program
          '';
          installPhase = ''
            mkdir -p $out/bin
            cp program $out/bin/${name}
          '';

          meta.mainProgram = name;
        };

        devShells.default = pkgs.mkShell {
          packages = [
            pkgs.csfml
            pkgs.pkg-config
            # For clang-format
            pkgs.clang-tools
            pkgs.just
          ];
        };

        formatter = treefmtEval.config.build.wrapper;

        checks = {
          formatting = treefmtEval.config.build.check self;
        };
      }
    )
    // {
      # For debugging
      inherit self;
    };
}
