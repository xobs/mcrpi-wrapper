#include <stdio.h>
#include <stdint.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <dlfcn.h>
#include <SDL.h>
#include <SDL_syswm.h>

void link_libglesv1_cm(void) __attribute__((constructor));

typedef void* DISPMANX_ELEMENT_HANDLE_T;
typedef void* DISPMANX_DISPLAY_HANDLE_T;
typedef void* DISPMANX_UPDATE_HANDLE_T;
typedef void* DISPMANX_RESOURCE_HANDLE_T;
typedef void* DISPMANX_PROTECTION_T;
typedef void* VC_DISPMANX_ALPHA_T;
typedef void* DISPMANX_CLAMP_T;
typedef void* DISPMANX_TRANSFORM_T;
typedef int VC_RECT_T;

typedef struct {
   DISPMANX_ELEMENT_HANDLE_T element;
   int width;   /* This is necessary because dispmanx elements are not queriable. */
   int height;
 } EGL_DISPMANX_WINDOW_T;

void *libXext;
void *libGLESv1_CM;


/*
int gcoOS_DestroyContext() {
	printf("gcoOS_DestroyContext()\n");
	return NULL;
}
*/

static void *dlsym_assert(void *handle, const char *symbol) {
	void *ret;
	ret = dlsym(handle, symbol);
	if (!ret) {
		char err[2048];
		snprintf(err, sizeof(err)-1, "Unable to get %s", symbol);
		perror(err);
		exit(1);
	}
	return ret;
}

static void *get_from_libEGL(const char *symbol) {
	static void *libEGL; 
	if (!libEGL) {
		libEGL = dlopen("libEGL.so", RTLD_LAZY);
		if (!libEGL) {
			perror("Unable to open libEGL.so");
			exit(1);
		}
	}
	return dlsym_assert(libEGL, symbol);
}

/*
void *XextAddDisplay (void *extinfo, void *dpy, char *ext_name,
			        void *hooks, int nevents, void *data) {
	void *result;
	static void *last_result;
	static void *(*XextAddDisplayReal) (void *extinfo, void *dpy, char *ext_name,
			        void *hooks, int nevents, void *data);
	static int counter = 0;
	static int total_counter = 0;
	printf("XextAddDisplay(%p, %p, %s, %p, %d, %p) %d\n", extinfo, dpy, ext_name, hooks, nevents, data, total_counter++);

	if (!XextAddDisplayReal) {
		if (!libXext) {
			libXext = dlopen("libXext.so.6", RTLD_LAZY);
			if (!libXext) {
				perror("Unable to open libXext.so");
				exit(1);
			}
		}
		XextAddDisplayReal = dlsym_assert(libXext, "XextAddDisplay");
	}

	if (!strcmp(ext_name, "XFree86-DRI")) {
		counter++;
		if (counter == 1) {
			printf("Skipping!\n");
			return last_result;
		}
	}
	result = XextAddDisplayReal(extinfo, dpy, ext_name, hooks, nevents, data);
	if (!strcmp(ext_name, "XFree86-DRI"))
		last_result = result;
	printf("Result of XextAddDisplayReal(): %p\n", result);
	return result;
}
*/

EGLAPI EGLBoolean EGLAPIENTRY eglBindAPI(EGLenum api)
{
	static EGLBoolean (*eglBindAPIReal)(EGLenum api);
	static EGLBoolean ret;
	static int counter = 0;

	fprintf(stderr, "eglBindAPI(%d): ", api);
	if (!eglBindAPIReal)
		eglBindAPIReal = get_from_libEGL("eglBindAPI");

	switch(counter++) {
	default:
		ret = eglBindAPIReal(api);
		fprintf(stderr, "%d\n", ret);
		return ret;
	}
}

EGLAPI EGLBoolean EGLAPIENTRY eglTerminate(EGLDisplay dpy)
{
	static EGLBoolean (*eglTerminateReal)(EGLDisplay dpy);
	static EGLBoolean ret;
	static int counter = 0;

	fprintf(stderr, "eglTerminateReal(%p): ", dpy);
	if (!eglTerminateReal)
		eglTerminateReal = get_from_libEGL("eglTerminate");

	switch(counter++) {
	case 0:
		fprintf(stderr, "1 (lying)\n");
		return 1;

	default:
		ret = eglTerminateReal(dpy);
		fprintf(stderr, "%d\n", ret);
		return ret;
	}
}

EGLAPI EGLBoolean EGLAPIENTRY eglDestroySurface(EGLDisplay dpy, EGLSurface surface)
{
	static EGLBoolean (*eglDestroySurfaceReal)(EGLDisplay dpy, EGLSurface surface);
	static EGLBoolean ret;
	static int counter = 0;

	fprintf(stderr, "eglDestroySurface(%d, %d): ", dpy, surface);
	if (!eglDestroySurfaceReal)
		eglDestroySurfaceReal = get_from_libEGL("eglDestroySurface");

	switch(counter++) {
	case 0:
	case 1:
		fprintf(stderr, "1 (lying)\n");
		return 1;

	default:
		ret = eglDestroySurfaceReal(dpy, surface);
		fprintf(stderr, "%d\n", ret);
		return ret;
	}
}

EGLAPI EGLBoolean EGLAPIENTRY eglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
{
	static EGLBoolean (*eglSwapBuffersReal)(EGLDisplay dpy, EGLSurface surface);
	static EGLBoolean ret;
	static int counter = 0;

//	fprintf(stderr, "eglSwapBuffers(%p, %p): ", dpy, surface);
	if (!eglSwapBuffersReal)
		eglSwapBuffersReal = get_from_libEGL("eglSwapBuffers");

	switch(counter++) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
		fprintf(stderr, "1 (guessed)\n");
		return 1;

	default:
		ret = eglSwapBuffersReal(dpy, surface);
		//fprintf(stderr, "%d\n", ret);
		return ret;
	}
}


EGLAPI EGLContext EGLAPIENTRY eglCreateContext(EGLDisplay dpy, EGLConfig config,
		                            EGLContext share_context,
						const EGLint *attrib_list)
{
	static EGLContext (*eglCreateContextReal)(EGLDisplay dpy, EGLConfig config,
		                            EGLContext share_context,
						const EGLint *attrib_list);
	static EGLContext *ret;
	static int counter = 0;

	fprintf(stderr, "eglCreateContext(%p, %p, %p, %p): ", dpy, config, share_context, attrib_list);
	if (!eglCreateContextReal)
		eglCreateContextReal = get_from_libEGL("eglCreateContext");

	switch(counter++) {
	case 0:
		fprintf(stderr, "NULL (guessed)\n");
		return NULL;

	case 1:
		ret = eglCreateContextReal(dpy, config, share_context, attrib_list);
		fprintf(stderr, "%p\n", ret);
		return ret;

	default:
		fprintf(stderr, "%p (cached)\n", ret);
		return ret;
	}
}

EGLAPI EGLBoolean EGLAPIENTRY eglChooseConfig(EGLDisplay dpy, const EGLint *attrib_list,
		                           EGLConfig *configs, EGLint config_size,
					      EGLint *num_config)
{
	static EGLBoolean (*eglChooseConfigReal)(EGLDisplay dpy, const EGLint *attrib_list,
			                           EGLConfig *configs, EGLint config_size,
					                              EGLint *num_config);
	static EGLBoolean ret;
	static int counter = 0;

	fprintf(stderr, "eglChooseConfig(%p, %p, %p, %d, %p): ", dpy, attrib_list, configs, config_size, num_config);
	if (!eglChooseConfigReal)
		eglChooseConfigReal = get_from_libEGL("eglChooseConfig");

	switch(counter++) {
	case 0:
		fprintf(stderr, "1 (guessed)\n");
		return 1;

	case 1:
		ret = eglChooseConfigReal(dpy, attrib_list, configs, config_size, num_config);
		fprintf(stderr, "%d\n", ret);
		return ret;

	default:
		fprintf(stderr, "%d (cached)\n", ret);
		return ret;
	}
}


EGLAPI EGLBoolean EGLAPIENTRY eglInitialize(EGLDisplay dpy, EGLint *major, EGLint *minor)
{
	static EGLBoolean (*eglInitializeReal)(EGLDisplay dpy, EGLint *major, EGLint *minor);
	static EGLBoolean ret;
	static int counter = 0;
	fprintf(stderr, "eglInitialize(%p, %p, %p): ", dpy, major, minor);
	if (!eglInitializeReal)
		eglInitializeReal = get_from_libEGL("eglInitialize");

	switch(counter++) {
	case 0:
		fprintf(stderr, "1 (guessed)\n");
		return 1;

	case 1:
		ret = eglInitializeReal(dpy, major, minor);
		fprintf(stderr, "%d\n", ret);
		return ret;

	default:
		fprintf(stderr, "%d (cached)\n", ret);
		return ret;
	}
}

EGLAPI EGLBoolean EGLAPIENTRY eglMakeCurrent(EGLDisplay dpy, EGLSurface draw,
		                          EGLSurface read, EGLContext ctx)
{
	static EGLBoolean (*eglMakeCurrentReal)(EGLDisplay dpy, EGLSurface draw,
		                          EGLSurface read, EGLContext ctx);
	static EGLBoolean ret;
	static int counter = 0;

	fprintf(stderr, "eglMakeCurrent(%p, %p, %p, %p): ", dpy, draw, read, ctx);
	if (!eglMakeCurrentReal)
		eglMakeCurrentReal = get_from_libEGL("eglMakeCurrent");

	switch(counter++) {
	case 0:
	case 1:
		fprintf(stderr, "1 (guessed)\n");
		return 1;

	case 2:
		ret = eglMakeCurrentReal(dpy, draw, read, ctx);
		fprintf(stderr, "%d\n", ret);
		return ret;

	default:
		fprintf(stderr, "%d (cached)\n", ret);
		return ret;
	}
}

EGLAPI EGLDisplay EGLAPIENTRY eglGetDisplay(EGLNativeDisplayType display_id)
{
	static EGLDisplay (*eglGetDisplayReal)(EGLNativeDisplayType display_id);
	static EGLDisplay ret;
	static int counter = 0;

	fprintf(stderr, "eglGetDisplay(%d): ", display_id);
	if (!eglGetDisplayReal)
		eglGetDisplayReal = get_from_libEGL("eglGetDisplay");

	switch(counter++) {
	case 0:
		fprintf(stderr, "4 (lying)\n");
		return (void *)4;

	case 1:
		ret = eglGetDisplayReal(display_id);
		fprintf(stderr, "%p\n", ret);
		return ret;

	default:
		fprintf(stderr, "%p (cached)\n", ret);
		return ret;
	}
}


EGLAPI EGLSurface EGLAPIENTRY eglCreateWindowSurface(EGLDisplay dpy, EGLConfig config,
                                  EGLNativeWindowType win,
                                  const EGLint *attrib_list) {
	static int counter = 0;
	static EGLSurface (*eglCreateWindowSurfaceReal)(EGLDisplay dpy, EGLConfig config,
                                  EGLNativeWindowType win,
                                  const EGLint *attrib_list);
	static EGLSurface *ret;
	SDL_SysWMinfo sysInfo; //Will hold our Window information

	fprintf(stderr, "eglCreateWindowSurface(%p, %p, %p, %p): ", dpy, config, win, attrib_list);

	if (!eglCreateWindowSurfaceReal)
		eglCreateWindowSurfaceReal = get_from_libEGL("eglCreateWindowSurface");

	switch(counter++) {
	case 0:
		fprintf(stderr, "0x4 (guessed)\n");
		return (EGLSurface)0x4;

	case 1:
		SDL_VERSION(&sysInfo.version); //Set SDL version
		if(SDL_GetWMInfo(&sysInfo) <= 0)
		{
			fprintf(stderr, "Unable to get window handle");
			return 0;
		}

		ret = eglCreateWindowSurfaceReal(dpy, config, (EGLNativeWindowType)sysInfo.info.x11.window, attrib_list);
		fprintf(stderr, "%p\n", ret);
		return ret;

	default:
		fprintf(stderr, "%p (cached)\n", ret);
		return ret;
	}
}


int32_t graphics_get_display_size( const uint16_t display_number, uint32_t *width, uint32_t *height) {
	printf("graphics_get_display_size(%d, %p, %p)\n", display_number, width, height);
	*width = 2560;
	*height = 1700;

	SDL_SetVideoMode(*width, *height, 32, SDL_FULLSCREEN);// | SDL_RESIZABLE);
	return 0;
}

int vc_dispmanx_display_open(int device) {
	printf("vc_dispmanx_display_open(%d)\n", device);
	return 0;
}

DISPMANX_ELEMENT_HANDLE_T vc_dispmanx_element_add ( DISPMANX_UPDATE_HANDLE_T update,
		 DISPMANX_DISPLAY_HANDLE_T display,
		int32_t layer, const VC_RECT_T *dest_rect, DISPMANX_RESOURCE_HANDLE_T src,
		const VC_RECT_T *src_rect, DISPMANX_PROTECTION_T protection,
		VC_DISPMANX_ALPHA_T *alpha,
		DISPMANX_CLAMP_T *clamp, DISPMANX_TRANSFORM_T transform )  {
	printf("vc_dispmanx_element_add(%p, %p, %d, %p, %p, %p, %p, %p, %p, %p)\n",
			update, display, layer, dest_rect, src, src_rect, protection,
					      alpha, clamp, transform);
	return NULL;
}

int vc_dispmanx_update_submit_sync( DISPMANX_UPDATE_HANDLE_T update ) {
	printf("vc_dispmanx_update_submit_sync(%p)\n", update);
	return 0;
}

void bcm_host_deinit(void) {
	printf("bcm_host_deinit()\n");
	return;
}

int vc_dispmanx_update_start( int32_t priority ) {
	printf("vc_dispmanx_update_start(%d)\n", priority);
	return 0;
}

void bcm_host_init(void) {
	printf("bcm_host_init()\n");
	return;
}
