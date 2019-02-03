{
  pkgs ? import <nixpkgs> {},
  stdenv ? pkgs.clangStdenv
}:

stdenv.mkDerivation {
  name = "spa-env";
  buildInputs = with pkgs; [
    qt5.qtbase
    cmake

    # utilities
    gdb
    ccls
  ];
}
