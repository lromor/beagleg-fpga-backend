# This is a nix-shell for use with the nix package manager.
# If you have nix installed, you may simply run `nix-shell`
# in this repo, and have all dependencies ready in the new shell.

{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  buildInputs = with pkgs;
    [
      valgrind
      yosys
      surelog
      uhdm
      yosys-symbiflow.systemverilog

      nextpnr
      icestorm
      verilator


      verible
    ];
}
