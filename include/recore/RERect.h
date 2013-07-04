/*
 *   Copyright 2012 - 2013 Kulykov Oleh
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


#ifndef __RERECT_H__
#define __RERECT_H__

#include "RECommonHeader.h"
#include "REPoint2.h"
#include "RESize.h"
#include "REMath.h"
#include "REMem.h"

#if defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

/// Structure of rectangle.
typedef struct _reRectStruct
{
	union 
	{
		struct
		{
			/// Rectangle X coordinate.
			REFloat32 x;
			
			/// Rectangle Y coordinate.
			REFloat32 y;
			
			/// Rectangle width.
			REFloat32 width;
			
			/// Rectangle height.
			REFloat32 height;
		};
#if defined(__ARM_NEON__)
		/// arm neon vector with x, y, width and height.
		float32x4_t armNeonVector;
#endif
		/// array with x, y, width and height.
		REFloat32 xywh[4];
	};
}
/// Structure of rectangle.
RERectStruct;

/// Class of rectangle.
class __RE_PUBLIC_CLASS_API__ RERect
{
public:
	union 
	{
		struct
		{
			/// Rectangle X coordinate.
			REFloat32 x;
			
			/// Rectangle Y coordinate.
			REFloat32 y;
			
			/// Rectangle width.
			REFloat32 width;
			
			/// Rectangle height.
			REFloat32 height;
		};
#if defined(__ARM_NEON__)		
		/// arm neon vector with x, y, width and height.
		float32x4_t armNeonVector;
#endif	
		/// array with x, y, width and height.
		REFloat32 xywh[4];
	};
	
	/// Sets center coordinates of rectangle.
	RERect & setCenter(const REFloat32 centerX, const REFloat32 centerY)
	{
		x = (centerX - (width / 2.0f));
		y = (centerY - (height / 2.0f));
		return (*this);
	}
	
	/// Sets center point of rectangle.
	RERect & setCenter(const REPoint2 & centerPoint)
	{
		x = (centerPoint.x - (width / 2.0f));
		y = (centerPoint.y - (height / 2.0f));
		return (*this);
	}
	
	/// Returns center point of rectangle.
	REPoint2 getCenter() const
	{
		return REPoint2((x + (width / 2)), (y + (height / 2)));
	}
	
	/// Returns top center point of rectangle.
	REPoint2 getTopCenter() const
	{
		return REPoint2((x + (width / 2)), y);
	}
	
	/// Sets top center point of rectangle.
	RERect & setTopCenter(const REPoint2 & topCenter)
	{
		x = topCenter.x - (width / 2);
		y = topCenter.y;
		return (*this);
	}
	
	/// Sets top center coordinates of rectangle
	RERect & setTopCenter(const REFloat32 topCenterX, const REFloat32 topCenterY)
	{
		x = topCenterX - (width / 2);
		y = topCenterY;
		return (*this);
	}
	
	/// Returns bottom center point of rectangle.
	REPoint2 setBottomCenter() const
	{
		return REPoint2(x + (width / 2), y + height);
	}
	
	/// Sets bottom center point of rectangle.
	RERect & setBottomCenter(const REPoint2 & bottomCenter)
	{
		x = bottomCenter.x - (width / 2);
		y = bottomCenter.y - height;
		return (*this);
	}
	
	/// Sets bottom center coordinates of rectangle.
	RERect & setBottomCenter(const REFloat32 bottomCenterX, const REFloat32 bottomCenterY)
	{
		x = bottomCenterX - (width / 2);
		y = bottomCenterY - height;
		return (*this);
	}
	
	/// Returns left center point of rectangle.
	REPoint2 setLeftCenter() const
	{
		return REPoint2(x, y + (height / 2));
	}
	
	/// Sets left center point of rectangle.
	RERect & setLeftCenter(const REPoint2 & leftCenter)
	{
		x = leftCenter.x;
		y = leftCenter.y - (height / 2);
		return (*this);
	}
	
	/// Sets left center coordinates of rectangle.
	RERect & setLeftCenter(const REFloat32 leftCenterX, const REFloat32 leftCenterY)
	{
		x = leftCenterX;
		y = leftCenterY - (height / 2);
		return (*this);
	}
	
	/// Returns right center point of rectangle.
	REPoint2 getRightCenter() const
	{
		return REPoint2(x + width, y + (height / 2));
	}
	
	/// Sets right center point of rectangle.
	RERect & setRightCenter(const REPoint2 & rightCenter)
	{
		x = rightCenter.x - width;
		y = rightCenter.y - (height / 2);
		return (*this);
	}
	
	/// Sets right center coordinates of rectangle.
	RERect & setRightCenter(const REFloat32 rightCenterX, const REFloat32 rightCenterY)
	{
		x = rightCenterX - width;
		y = rightCenterY - (height / 2);
		return (*this);
	}
	
	/// Returns top right point.
	REPoint2 getTopRight() const
	{
		return REPoint2(x + width, y);
	}
	
	/// Sets top right point of rectangle.
	RERect & setTopRight(const REPoint2 & topRight)
	{
		x = topRight.x - width;
		y = topRight.y;
		return (*this);
	}
	
	/// Sets top right coordinates of rectangle.
	RERect & setTopRight(const REFloat32 topRightX, const REFloat32 topRightY)
	{
		x = topRightX - width;
		y = topRightY;
		return (*this);
	}
	
	/// Returns bottom right point of rectangle.
	REPoint2 getBottomRight() const
	{
		return REPoint2(x + width, y + height);
	}
	
	/// Sets bottom right point of rectangle.
	RERect & setBottomRight(const REPoint2 & bottomRight)
	{
		x = bottomRight.x - width;
		y = bottomRight.y - height;
		return (*this);
	}
	
	/// Sets bottom right coordinates of rectangle.
	RERect & setBottomRight(const REFloat32 bottomRightX, const REFloat32 bottomRightY)
	{
		x = bottomRightX - width;
		y = bottomRightY - height;
		return (*this);
	}
	
	/// Returns bottom left point.
	REPoint2 getBottomLeft() const
	{
		return REPoint2(x, y + height);
	}
	
	/// Sets bottom left point of rectangle.
	RERect & setBottomLeft(const REPoint2 & bottomLeft)
	{
		x = bottomLeft.x;
		y = bottomLeft.y - height;
		return (*this);
	}
	
	/// Sets bottom left coordinates of rectangle.
	RERect & setBottomLeft(const REFloat32 bottomLeftX, const REFloat32 bottomLeftY)
	{
		x = bottomLeftX;
		y = bottomLeftY - height;
		return (*this);
	}
	
	/// Clear rectangle. Sets all values to zeros.
	RERect & clear()
	{
		REMem::Memset(xywh, 0, sizeof(REFloat32) * 4);
		return (*this);
	}
	
	/// Sets size of rectangle.
	RERect & setSize(const RESize & newSize)
	{
		width = newSize.width;
		height = newSize.height;
		return (*this);
	}
	
	/// Sets size values of rectangle.
	RERect & setSize(const REFloat32 newSizeWidth, const REFloat32 newSizeHeight)
	{
		width = newSizeWidth;
		height = newSizeHeight;
		return (*this);
	}
	
	/// Returns size of rectangle.
	RESize getSize() const
	{
		return RESize(width, height);
	}
	
	/// Sets origin point of rectangle. Means sets top left coordinates.
	RERect & setOrigin(const REPoint2 & newOrigin)
	{
		x = newOrigin.x;
		y = newOrigin.y;
		return (*this);
	}
	
	/// Sets origin coordinates of rectangle. Means sets top left coordinates.
	RERect & setOrigin(const REFloat32 newOriginX, const REFloat32 newOriginY)
	{
		x = newOriginX;
		y = newOriginY;
		return (*this);
	}
	
	/// Checks if point coordinates inside rectangle.
	REBOOL isPointInRect(const REFloat32 pointX, const REFloat32 pointY) const
	{
		return ((pointX > x) && (pointX < (x + width)) && (pointY > y) && (pointY < (y + height)));
	}
	
	/// Checks if point inside rectangle.
	REBOOL isPointInRect(const REPoint2 & point) const
	{
		return ((point.x > x) && (point.x < (x + width)) && (point.y > y) && (point.y < (y + height)));
	}
	
	/// Checks if rectangle intersepts with another rectangle.
	REBOOL isIntersects(const RERect & b) const
	{
		return !( ((x + width) <= b.x) || (x >= (b.x + b.width)) || ((y + height) <= b.y) || (y >= (b.y + b.height)) );
	}
	
	/// Checks is rectangle containes another rectangle.
	REBOOL isContainsRect(const RERect & b) const
	{
		return ((x < b.x) && (y < b.y) && ((x + width) > (b.x + b.width)) && ((y + height) > (b.y + b.height)));
	}
	
	/// Returns intersected rectangle with another rectangle.
	RERect getIntersection(const RERect & b) const
	{
		const REFloat32 bottomX = (x + width);
		const REFloat32 bottomY = (y + height);
		const REFloat32 b_bottomX = (b.x + b.width);
		const REFloat32 b_bottomY = (b.y + b.height);
		
		if ( (bottomX <= b.x) || (x >= b_bottomX) || (bottomY <= b.y) || (y >= b_bottomY) )
		{
			return RERect(0.0f, 0.0f, 0.0f, 0.0f);
		}
		
		const REFloat32 newX = MAX(x, b.x);
		const REFloat32 newY = MAX(y, b.y);
		
		const REFloat32 newWidth = MIN(bottomX,  b_bottomX) - newX;
		const REFloat32 newHeight = MIN(bottomY, b_bottomY) - newY;
		
		return RERect(newX, newY, newWidth, newHeight);
	}
	
	/// Returns union rectangle with another rectangle.
	RERect getUnion(const RERect & b) const
	{
		const REFloat32 newX = MIN(x, b.x);
		const REFloat32 newY = MIN(y, b.y);
		
		const REFloat32 bottomX = (x + width);
		const REFloat32 bottomY = (y + height);
		const REFloat32 b_bottomX = (b.x + b.width);
		const REFloat32 b_bottomY = (b.y + b.height);
		
		const REFloat32 newWidth = MAX(bottomX, b_bottomX) - newX;
		const REFloat32 newHeight = MAX(bottomY, b_bottomY) - newY;
		
		return RERect(newX, newY, newWidth, newHeight);
	}
	
	/// Basic assignment operator.
	RERect & operator=(const RERect & anotherRect) 
	{
#if defined(__ARM_NEON__)
		armNeonVector = anotherRect.armNeonVector;
#else
		REMem::Memcpy(xywh, anotherRect.xywh, sizeof(REFloat32) * 4);
#endif
		return (*this);
	}
	
	/// Basic assignment operator.
	RERect & operator=(const RERectStruct & anotherRect) 
	{
#if defined(__ARM_NEON__)
		armNeonVector = anotherRect.armNeonVector;
#else
		REMem::Memcpy(xywh, anotherRect.xywh, sizeof(REFloat32) * 4);
#endif
		return (*this);
	}
	
#if defined(__ARM_NEON__)
	/// Constructs rectangle with arm neon vector.
	RERect(const float32x4_t & armNVec) : armNeonVector(armNVec) { }
#endif
	
	/// Constructs rectangle with x, y, width, height values.
	RERect(const REFloat32 newX, const REFloat32 newY, const REFloat32 newWidth, const REFloat32 newHeight) : 
		x(newX), y(newY), width(newWidth), height(newHeight) { }
	
	/// Constructs rectangle with rect struct.
	RERect(const RERectStruct & anotherRect) : 
		x(anotherRect.x), y(anotherRect.y), width(anotherRect.width), height(anotherRect.height) { }
	
	/// Constructs rectangle with another rectangle.
	RERect(const RERect & anotherRect) : 
		x(anotherRect.x), y(anotherRect.y), width(anotherRect.width), height(anotherRect.height) { }
	
	/// Constructs rectangle with origin and size
	RERect(const REPoint2 & newOrigin, const RESize & newSize) :
		x(newOrigin.x), y(newOrigin.y), width(newSize.width), height(newSize.height) { }
	
	/// Constructs zero value rectangle.
	RERect() : x(0.0f), y(0.0f), width(0.0f), height(0.0f) { }
	
	~RERect() { }
	
	
	/// Objective-c additions
#if (defined(CG_EXTERN) || defined(CG_INLINE)) && defined(CGFLOAT_TYPE)

	CGRect getCGRect() const 
	{
		CGRect r;
		r.origin.x = (CGFloat)x; 
		r.origin.y = (CGFloat)y;
		r.size.width = (CGFloat)width;
		r.size.height = (CGFloat)height;
		return r;
	}
	
	RERect & operator=(const CGRect & anotherRect)
	{
		x = (REFloat32)anotherRect.origin.x; 
		y = (REFloat32)anotherRect.origin.y;
		width = (REFloat32)anotherRect.size.width;
		height = (REFloat32)anotherRect.size.height;
		return (*this);
	}
	
	RERect(const CGRect & anotherRect) : 
		x((REFloat32)anotherRect.origin.x), 
		y((REFloat32)anotherRect.origin.y), 
		width((REFloat32)anotherRect.size.width),
		height((REFloat32)anotherRect.size.height) 
	{
		
	}
	
#endif	
	
};


#endif /* __RERECT_H__ */

