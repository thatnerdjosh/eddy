unit_test_files = [
    'storage'
]

foreach name : unit_test_files
	# FIXME: See if interpolation works.
	exe = executable(
		name,
		'test_'+ name +'.c',
		include_directories: inc,
		dependencies: bodhilib_dep,
	)
    test(name, exe)
endforeach
