# This is a nix-shell for use with the nix package manager.
# If you have nix installed, you may simply run `nix-shell`
# in this repo, and have all dependencies ready in the new shell.

{ pkgs ? import <nixpkgs> {} }:
let
  xls = pkgs.stdenv.mkDerivation rec {
    name = "xls";
    version = "v0.0.0-6661-g9dfd0b42e";
    src = builtins.fetchurl {
      url = "https://github.com/google/xls/releases/download/v0.0.0-6661-g9dfd0b42e/${name}-${version}-linux-x64.tar.gz";
    };
    installPhase = ''
        mkdir -p $out/bin
        for f in *_main ; do cp $f $out/bin/$(echo xls-$f | sed 's/_main//' | sed 's/_/-/g'); done
        cp -r xls/ $out/bin
    '';

    outputHashAlgo = "sha256";
    outputHashMode = "recursive";
    outputHash = "sha256-NQ8eQbhJ58XiLKY4rArB0yZr3rJhLBkbiuQ+imI9KXA=";
  };
in pkgs.mkShell {
  buildInputs = with pkgs;
    [
      valgrind
      yosys
      surelog
      uhdm
      yosys-synlig
      xls

      nextpnr
      icestorm
      verilator


      verible
    ];
}
