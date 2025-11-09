project('wibbly-wobbly', ['c', 'cpp'],
    default_options : [
        'cpp_std=c++23',
        'c_std=c23',
        'warning_level=2',
        'buildtype=release',
        'b_lto=true'
    ]
)

add_project_arguments('-Wall', '-Wextra', '-Wshadow', language : ['c', 'cpp'])
add_project_arguments('-DUNICODE', '-D_UNICODE', language : ['c', 'cpp'])

inc = include_directories('src/shared', '.')

qt5_dep = dependency('qt5', modules : ['Core', 'Gui', 'Widgets'])
vsscript_dep = dependency('vapoursynth-script', required : true)

if host_machine.system() == 'windows'
  add_project_link_arguments('-mwindows', language : ['c', 'cpp'])
endif

qt5 = import('qt5')

shared_moc_files = qt5.preprocess(
  moc_headers : [
    'src/shared/BookmarksModel.h',
    'src/shared/CombedFramesModel.h',
    'src/shared/CustomListsModel.h',
    'src/shared/DockWidget.h',
    'src/shared/FrameRangesModel.h',
    'src/shared/FrozenFramesModel.h',
    'src/shared/ListWidget.h',
    'src/shared/PresetsModel.h',
    'src/shared/ProgressDialog.h',
    'src/shared/OrphanFieldsModel.h',
    'src/shared/ScrollArea.h',
    'src/shared/SectionsModel.h',
    'src/shared/WobblyProject.h'
  ]
)

wibbly_moc_files = qt5.preprocess(
  moc_headers : [
    'src/wibbly/WibblyWindow.h'
  ]
)

wobbly_moc_files = qt5.preprocess(
  moc_headers : [
    'src/wobbly/CombedFramesCollector.h',
    'src/wobbly/FrameLabel.h',
    'src/wobbly/ImportWindow.h',
    'src/wobbly/OverlayLabel.h',
    'src/wobbly/PresetTextEdit.h',
    'src/wobbly/SectionsProxyModel.h',
    'src/wobbly/SpinBox.h',
    'src/wobbly/TableView.h',
    'src/wobbly/TableWidget.h',
    'src/wobbly/WobblyWindow.h'
  ]
)

rapidjson_inc = include_directories('.')

shared_sources = [
  'src/shared/BookmarksModel.cpp',
  'src/shared/CombedFramesModel.cpp',
  'src/shared/CustomListsModel.cpp',
  'src/shared/DockWidget.cpp',
  'src/shared/FrameRangesModel.cpp',
  'src/shared/FrozenFramesModel.cpp',
  'src/shared/ListWidget.cpp',
  'src/shared/PresetsModel.cpp',
  'src/shared/ProgressDialog.cpp',
  'src/shared/OrphanFieldsModel.cpp',
  'src/shared/ScrollArea.cpp',
  'src/shared/SectionsModel.cpp',
  'src/shared/WobblyProject.cpp',
  'src/shared/WobblyShared.cpp'
]

wobbly_sources = [
  'src/wobbly/CombedFramesCollector.cpp',
  'src/wobbly/FrameLabel.cpp',
  'src/wobbly/ImportWindow.cpp',
  'src/wobbly/OverlayLabel.cpp',
  'src/wobbly/PresetTextEdit.cpp',
  'src/wobbly/SectionsProxyModel.cpp',
  'src/wobbly/SpinBox.cpp',
  'src/wobbly/TableView.cpp',
  'src/wobbly/TableWidget.cpp',
  'src/wobbly/Wobbly.cpp',
  'src/wobbly/WobblyWindow.cpp'
]

wibbly_sources = [
  'src/wibbly/Wibbly.cpp',
  'src/wibbly/WibblyJob.cpp',
  'src/wibbly/WibblyWindow.cpp'
]

executable('wobbly',
  wobbly_sources + shared_sources + shared_moc_files + wobbly_moc_files,
  include_directories : [inc, rapidjson_inc],
  dependencies : [qt5_dep, vsscript_dep],
  install : true
)

executable('wibbly',
  wibbly_sources + shared_sources + shared_moc_files + wibbly_moc_files,
  include_directories : [inc, rapidjson_inc],
  dependencies : [qt5_dep, vsscript_dep],
  install : true
)