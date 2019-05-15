cmake_minimum_required(VERSION 3.8)
project(f2fs)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        acl.c
        acl.h
        checkpoint.c
        data.c
        debug.c
        dir.c
        extent_cache.c
        f2fs.h
        file.c
        gc.c
        gc.h
        gc_nvm.c
        hash.c
        inline.c
        inode.c
        namei.c
        node.c
        node.h
        nvm.c
        nvm.h
        recovery.c
        segment.c
        segment.h
        shrinker.c
        super.c
        sysfs.c
        trace.c
        trace.h
        xattr.c
        xattr.h)

add_executable(f2fs ${SOURCE_FILES})