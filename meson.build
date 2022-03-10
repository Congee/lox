project(
  'lox',
  'c',
  'cpp',
  version: '0.1.0',
  license: 'MIT',
  meson_version: '>=0.57.1',
  default_options: [
    'cpp_std=c++20',
    'c_std=c11',
    'warning_level=3',
    'werror=false',
    'cpp_args=-Og',
  ],
)

fmt = dependency('fmt')  # it is too nice having have flake.nix
expected = dependency('tl-expected', method : 'cmake', modules : ['tl::expected'])
cxxabi = meson.get_compiler('cpp').find_library('c++abi')  # required on darwin

executable(
  'lox',
  'main.cpp',
  install: false,
  dependencies: [fmt, expected, cxxabi]
)
