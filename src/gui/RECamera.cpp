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


#include "../../include/regui/RECamera.h"
#include "../../include/regui/IRETexture.h"

void RECamera::Apply()
{
#if (defined(__RE_USING_OPENGL_ES__) || defined(__RE_USING_OPENGL__))
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projection.line);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(modelView.line);
#endif	
}

REMatrix4 RECamera::GetAppliedProjectionMatrix()
{
	REMatrix4 m;
#if (defined(__RE_USING_OPENGL_ES__) || defined(__RE_USING_OPENGL__))
	glGetFloatv(GL_PROJECTION_MATRIX, m.line);
#endif	
	return m;
}

REMatrix4 RECamera::GetAppliedModelViewMatrix()
{
	REMatrix4 m;
#if (defined(__RE_USING_OPENGL_ES__) || defined(__RE_USING_OPENGL__))
	glGetFloatv(GL_MODELVIEW_MATRIX, m.line);
#endif	
	return m;
}

RECamera::RECamera() : REObject()
{
	projection = this->GetAppliedProjectionMatrix();
	modelView = this->GetAppliedModelViewMatrix();
}

RECamera::~RECamera()
{
	
}





