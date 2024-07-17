# project('bodhilib', 'c', version : '0.1')

inc = include_directories('.')
bodhilib = library(
	'bodhilib',
	['storage.c'],
	include_directories: inc
)

bodhilib_dep = declare_dependency(
	link_with: bodhilib,
	include_directories: inc
)

if get_option('enable-tests')
  subdir('tests')
endif
