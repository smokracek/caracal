project('Caracal', 'cpp', version : '0.1.0', 
    default_options : [
        'cpp_std=c++17',
        'buildtype=debug'])

libtorrent_dep = dependency('libtorrent-rasterbar')

src = [
    'src/caracal.cpp',
    'src/libtorrent_session.cpp',
    'src/torrent_status.cpp',
    'src/torrent_handle.cpp',
    'src/torrent_pool.cpp',
    'src/magnet_pool.cpp',
    'src/post_bundle.cpp']

caracal_lib = library('caracal', 
                        src, 
                        include_directories : ['src', 'include'],
                        dependencies : [libtorrent_dep],
                        install : true)

client_demo = executable('client-demo',
                            ['demos/client_demo.cpp'],
                            include_directories : ['src', 'include'],
                            link_with : caracal_lib)

dht_demo = executable('dht-demo',
                            ['demos/dht_demo.cpp'],
                            include_directories : ['src', 'include'],
                            link_with : caracal_lib)

seed_demo = executable('seed-demo',
                            ['demos/seed_demo.cpp'],
                            include_directories : ['src', 'include'],
                            link_with : caracal_lib)