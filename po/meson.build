dep_intl = []

if get_option('nls')
  i18n = import('i18n')
  i18n.gettext('eddy',
    args: [
      '--keyword=_',
      '--keyword=d_:1',
      '--keyword=P_:1,2',
      '--keyword=dP_:1,2',
      '--keyword=N_',
      '--keyword=NP_:1,2',
      '--from-code=UTF-8',
      '--foreign-user'
    ])
  cfg.set('ENABLE_NLS', '1')
  cc = meson.get_compiler('c')
  dep_intl = cc.find_library('intl', required : false)
endif
