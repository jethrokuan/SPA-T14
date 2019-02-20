{ pkgs ? import <nixpkgs> {} }:

with pkgs; dockerTools.buildImage {
  name = "jethrokuan/spa-clang-cmake";
  contents = [ clang clang-tools cmake bash ];

  config = {
    Cmd = [ "bash" ];
  };
}
