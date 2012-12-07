

#ifndef __ADD_LIB_OPENAL_CONFIG_H__
#define __ADD_LIB_OPENAL_CONFIG_H__


// NOTE: 
//      here you can define or undefine params for openal.library
//      may also include platform specific config header file
// 

#if ( defined(__RE_OS_ANDROID__) )

#warning "add search user path to project: -I/addlibs/openal_soft/include/"


#ifndef AL_BUILD_LIBRARY
#define AL_BUILD_LIBRARY
#endif

#ifndef AL_ALEXT_PROTOTYPES
#define AL_ALEXT_PROTOTYPES
#endif


/* Define to the library version */
#define ALSOFT_VERSION "1.13"

/* Define if we have the Android backend */
#define HAVE_ANDROID 1

/* Define if we have the ALSA backend */
/* #cmakedefine HAVE_ALSA */

/* Define if we have the OSS backend */
/* #cmakedefine HAVE_OSS */

/* Define if we have the Solaris backend */
/* #cmakedefine HAVE_SOLARIS */

/* Define if we have the DSound backend */
/* #cmakedefine HAVE_DSOUND */

/* Define if we have the Wave Writer backend */
/* #cmakedefine HAVE_WAVE */

/* Define if we have the Windows Multimedia backend */
/* #cmakedefine HAVE_WINMM */

/* Define if we have the PortAudio backend */
/* #cmakedefine HAVE_PORTAUDIO */

/* Define if we have the PulseAudio backend */
/* #cmakedefine HAVE_PULSEAUDIO */

/* Define if we have dlfcn.h */
#define HAVE_DLFCN_H 1

/* Define if we have the stat function */
#define HAVE_STAT 1

/* Define if we have the powf function */
#define HAVE_POWF 1

/* Define if we have the sqrtf function */
#define HAVE_SQRTF 1

/* Define if we have the acosf function */
#define HAVE_ACOSF 1

/* Define if we have the atanf function */
#define HAVE_ATANF 1

/* Define if we have the fabsf function */
#define HAVE_FABSF 1

/* Define if we have the strtof function */
#define HAVE_STRTOF 1

/* Define if we have stdint.h */
#define HAVE_STDINT_H 1

/* Define if we have the __int64 type */
/* #cmakedefine HAVE___INT64 */

/* Define to the size of a long int type */
#define SIZEOF_LONG 4

/* Define to the size of a long long int type */
#define SIZEOF_LONG_LONG 8

/* Define to the size of an unsigned int type */
#define SIZEOF_UINT 4

/* Define to the size of a void pointer type */
#define SIZEOF_VOIDP 4

/* Define if we have GCC's destructor attribute */
#define HAVE_GCC_DESTRUCTOR 1

/* Define if we have GCC's format attribute */
#define HAVE_GCC_FORMAT 1

/* Define if we have pthread_np.h */
/* #cmakedefine HAVE_PTHREAD_NP_H */

/* Define if we have float.h */
/* #cmakedefine HAVE_FLOAT_H */

/* Define if we have fenv.h */
#define HAVE_FENV_H 1

/* Define if we have fesetround() */
/* #cmakedefine HAVE_FESETROUND */

/* Define if we have _controlfp() */
/* #cmakedefine HAVE__CONTROLFP */

/* Define if we have pthread_setschedparam() */
#define HAVE_PTHREAD_SETSCHEDPARAM 1


#endif



#endif

