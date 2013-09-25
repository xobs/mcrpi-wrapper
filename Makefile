all: bcm_host.c
	gcc -c bcm_host.c -o bcm_host.o -fPIC `sdl-config --cflags`
	gcc -fPIC -shared -lEGL -lGLESv1_CM -Wl,-soname,libbcm_host.so.1 bcm_host.o -o libbcm_host.so.1.0
