##### project
project('eddy', 'c',
        version        : '0.0.1',
        license        : 'GPLv2',
        default_options: [ 'buildtype=plain', 'c_std=gnu99' ],
        meson_version  : '>= 0.40.0')
base_url = 'https://github.com/Deepspeed/eddy'

##### convenience variables for later
proj     = meson.project_name()
ver      = meson.project_version()

##### dependencies
efl_version = '>= 1.20.0'
deps = dependency('elementary', version: efl_version)
eeze_dep = dependency('eeze', required: true)

##### dir locations
dir_prefix = get_option('prefix')
dir_bin    = join_paths(dir_prefix, get_option('bindir'))
dir_data   = join_paths(dir_prefix, get_option('datadir'))

install_data([ 'AUTHORS',
               'LICENSE'
             ],
             install_dir: join_paths(dir_data, 'eddy'))

##### config.h
cfg = configuration_data()
cfg.set_quoted('PACKAGE'                 , proj)
cfg.set_quoted('PACKAGE_NAME'            , proj)
cfg.set_quoted('PACKAGE_VERSION'         , ver)
cfg.set_quoted('PACKAGE_STRING'          , proj + ' ' + ver)
cfg.set_quoted('PACKAGE_URL'             , base_url + proj)
cfg.set_quoted('PACKAGE_BIN_DIR'         , dir_bin)
cfg.set_quoted('PACKAGE_DATA_DIR'        , join_paths(dir_data, proj))

configure_file(output: 'config.h', configuration: cfg)

##### subdirs
subdir('src')
