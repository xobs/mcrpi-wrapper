Minecraft: Pi Edition wrapper
=============================

This is a wrapper for Minecraft: Pi Edition's OpenGL ES drivers.  It makes
several assumptions about running on a Raspberry Pi, some of which are not
valid on our system (Kosagi Novena).  Namely, it suffers from the following:

* Requires libbcm, which does not exist outside Broadcom chips
* Does not pass the window object to eglContextCreate()
* Calls init()/destroy() multiple times, which crashes our vendor's GL drivers
* Links against libGLESv2.so but uses GLES 1.0

This library and runtime environment aim to solve these problems.

Usage
-----

To compile our libbcm replacement, simply run "make".

Then download Minecraft from http://pi.minecraft.net/ and extract it so
that minecraft-pi is in the current directory.

Copy /usr/lib/libGLESv1_CM.so or /usr/lib/arm-linux-gnueabihf/libGLESv1_CM.so
to this directory and rename it to libGLESv2.so

Finally, run minecraft-pi with the following invokation:

    LD_PRELOAD=./libbcm_host.so LD_LIBRARY_PATH=$PWD ./minecraft-pi

Gotchas
-------

Minecraft is run at full-screen for performance reasons.  Unfortunately,
the Minecraft program doesn't have an "Exit" function, so you'll have to
come up with your own way of killing it.

Design
------

There are counters that monitor how many times various functions are
called.  Minecraft seems to initialize EGL and then tear it down again
right away, which causes our vendor's driver to crash.  To get around
this, we count how many times each driver is invoked, and only call
the real egl functions after a certain number of invocations.
