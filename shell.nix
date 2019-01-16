with import <nixpkgs> {};

pkgs.mkShell {
  buildInputs = with pkgs; [
    qt511.qtbase
    cmake
    clang

    # utilities
    cquery
  ];

  shellHook = ''
    export CXX=${clang}/bin/clang++
  '';
}
