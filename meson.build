project('Caracal', 'cpp', version : '0.1.0', default_options : ['cpp_std=c++17'])

libtorrent_dep = dependency('libtorrent-rasterbar')

src = [
    'src/caracal.cpp',
    'src/libtorrent_session.cpp']

caracal_lib = library('caracal', 
                        src, 
                        include_directories : ['src', 'include'],
                        dependencies : [libtorrent_dep],
                        install : true)

client_demo = executable('client-demo',
                            ['client_demo.cpp'],
                            include_directories : ['src', 'include'])

test_src = ['tests/SessionTest.cpp']

session_test = executable('session-test', 
                            test_src, 
                            include_directories : ['tests'],
                            link_with : caracal_lib)

test('session test', session_test)