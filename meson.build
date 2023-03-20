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
    'src/post_bundle.cpp',
    'src/dht_item.cpp']

caracal_lib = library('caracal', 
                        src, 
                        include_directories : ['src', 'include'],
                        dependencies : [libtorrent_dep],
                        install : true)