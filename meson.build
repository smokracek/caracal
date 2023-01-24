project('Caracal', 'cpp', version : '0.1.0', default_options : ['cpp_std=c++17'])

src_dir = 'src'
test_dir = 'test'

libtorrent_dep = dependency('libtorrent-rasterbar')

# libcaracal = declare_dependency(link_args : ['-L/usr/local/lib', '-llibcaracal'])

src = [
    'src/caracal.cpp',
    'src/client_session.cpp']

caracal_lib = library('caracal', 
                        src, 
                        include_directories : ['src', 'include'],
                        dependencies : [libtorrent_dep],
                        install : true)

test_src = ['tests/SessionTest.cpp']

session_test = executable('session-test', 
                            test_src, 
                            include_directories : ['tests'],
                            link_with : caracal_lib)

test('session test', session_test)