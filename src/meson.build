inc = include_directories('.')
executable('eddy', [
    'eddy.c', 'eddy.h'
  ],
  include_directories: inc,
  dependencies       : [ deps, eeze_dep, dep_intl ] ,
  gui_app            : true,
  install            : true)
