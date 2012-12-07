/*
 *   Copyright 2012 Kulykov Oleh
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */


#ifndef __RECOREINCLUDES_H__
#define __RECOREINCLUDES_H__

#include "RECoreOSSelector.h"
#include "RECoreSettings.h"
#include "RECorePlatformSelector.h"




#ifdef __RE_OS_WINDOWS__ 

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <wchar.h>
#include <limits.h>
#include <float.h>

/*
#ifdef __RE_USING_OPENGL__ 
#include <gl/gl.h>
#include <gl/glu.h>
#endif
*/

/*
#ifdef __RE_USING_OPENAL__ 
#include <al/al.h>
#include <al/alc.h>
#endif
*/

#endif /* __RE_OS_WINDOWS__ */









#ifdef __RE_OS_IPHONE__ 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <wchar.h>
#include <limits.h>
#include <float.h>

#include <sys/types.h>
#include <sys/errno.h>

/*
#ifdef __RE_USING_OPENGL_ES__ 
#if defined(__RE_USING_OPENGL_ES_1__)
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#elif defined(__RE_USING_OPENGL_ES_2__)
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h> 
#endif
#endif
*/

/*
#ifdef __RE_USING_OPENAL__ 
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif
*/


#endif /* __RE_OS_IPHONE__ */









#ifdef __RE_OS_MACOSX__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <wchar.h>
#include <limits.h>
#include <float.h>

#include <sys/types.h>
#include <sys/errno.h>

/*
#ifdef __RE_USING_OPENGL__ 
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif
*/

/*
#ifdef __RE_USING_OPENAL__ 
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <OpenAL/MacOSX_OALExtensions.h>
#endif
*/

#endif /* __RE_OS_MACOSX__ */










#ifdef __RE_OS_LINUX__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <wchar.h>
#include <limits.h>
#include <float.h>

#include <sys/types.h>
#include <sys/errno.h>

/*
#ifdef __RE_USING_OPENGL__ 
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
*/

#endif /* __RE_OS_LINUX__ */ 









#ifdef __RE_OS_ANDROID__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <wchar.h>
#include <limits.h>
#include <float.h>
#include <setjmp.h>
#include <string.h>
#include <ctype.h>
#include <cerrno>
#include <cstddef>

#include <sys/types.h>
#include <sys/errno.h>

#include <jni.h>
#include <android/log.h>

/*
#ifdef __RE_USING_OPENGL_ES__ 
#include <GLES/gl.h> 
#include <GLES/glext.h> 
#endif
*/

#endif /* __RE_OS_ANDROID__ */










#ifdef __RE_OS_BADA__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <wchar.h>
#include <limits.h>
#include <float.h>
#include <setjmp.h>
#include <string.h>
#include <ctype.h>
#include <cerrno>
#include <cstddef>

#include <sys/types.h>
#include <sys/errno.h>

/*
#ifdef __RE_USING_OPENGL_ES__
#include <gl.h>
#include <glext.h>
#include <glmacro.h>
#endif
*/

#ifdef __RE_USING_OPENAL__
#include <AL/al.h>
#include <AL/alc.h>
#endif

#endif /* __RE_OS_BADA__ */








#endif /* __RECOREINCLUDES_H__ */

