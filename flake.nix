{
  description = "C++ implementation of lox from Crafting Interpreters";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (
      system:
        let
          pkgs = import nixpkgs { inherit system; };
          llvm = pkgs.llvmPackages_latest;
          lib = nixpkgs.lib;

        in
          {
            devShell = pkgs.stdenvNoCC.mkDerivation {
              name = "shell";
              nativeBuildInputs = [
                # builder
                # p.gnumake
                # p.bear
                pkgs.cmake  # for discovering libraries
                pkgs.pkg-config
                pkgs.meson
                pkgs.ninja
                # debugger
                llvm.lldb

                # XXX: the order of include matters
                pkgs.clang-tools
                llvm.clang # clangd
                llvm.libcxxabi

                pkgs.gtest
                pkgs.fmt
                pkgs.tl-expected
              ] ++ lib.optional pkgs.stdenv.isLinux [ llvm.lld ]
              ;

              CPATH = builtins.concatStringsSep ":" [
                (lib.makeSearchPathOutput "dev" "include" [ llvm.libcxx ])
                (lib.makeSearchPath "resource-root/include" [ llvm.clang ])
              ];
            };
          }
    );
}
