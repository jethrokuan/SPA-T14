{ pkgs ? import <nixpkgs> {} }:

with pkgs; dockerTools.buildImage {
  name = "jethrokuan/spa-clang-cmake";
  contents = [ clang cmake bash ];

  config = {
    Cmd = [ "bash" ];
  };
}
