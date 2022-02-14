project(
  'pgs-chroma',
  'c',
  version: '0.1.0',
  license: 'GPL-3.0-or-later',
  default_options: [
    'c_std=c17',
    'warning_level=3',
  ]
)

add_project_arguments(
  [
    '-D_POSIX_C_SOURCE=200809',
    '-DPROJECT_VERSION="@0@"'.format(meson.project_version()),
  ],
  language: 'c'
)

src = [
  'src/main.c',
  'src/pgs.c',
  'src/util.c',
]

executable(
  'pgs-chroma',
  src,
  install: true
)

install_man('pgs-chroma.1')
