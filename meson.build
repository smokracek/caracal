project('Caracal', 'cpp', version : '0.1.0', 
    default_options : [
        'cpp_std=c++17',
        'buildtype=debug'])

libtorrent_dep = dependency('libtorrent-rasterbar')

client_demo = executable('client-demo', ['client_demo.cpp'], dependencies: [libtorrent_dep])

