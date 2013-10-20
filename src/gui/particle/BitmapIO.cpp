/*
 * Copyright (c) 2002-2007 Fenomen Games.  All Rights Reserved.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 * PURPOSE.
 *
 * REPRODUCTION IN WHOLE OR IN PART IS PROHIBITED WITHOUT THE WRITTEN
 * CONSENT OF THE COPYRIGHT OWNER.
 *
 */

/* $ Revision 1.5 $ */

//#include "StdAfx.H"

#ifndef ENGINE_FLASH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <setjmp.h>

#include "BitmapIO.h"

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define SWAP_WORD(w)    ((((w) & 0x00ff) << 8) | (((w) >> 8)))

#define SWAP_DWORD(d)   ((((d) & 0x000000ff) << 24) |           \
(((d) & 0x0000ff00) << 8) |                \
(((d) & 0x00ff0000) >> 8) |                \
(((d) >> 24)))

#if defined(__BIG_ENDIAN__)

#define LITTLE_WORD(w)    SWAP_WORD(w)
#define LITTLE_DWORD(d)   SWAP_DWORD(d)
#define BIG_WORD(w)               (w)
#define BIG_DWORD(d)      (d)

#else

#define LITTLE_WORD(w)    (w)
#define LITTLE_DWORD(d)   (d)
#define BIG_WORD(w)               SWAP_WORD(w)
#define BIG_DWORD(d)      SWAP_DWORD(d)

#endif /* __BIG_ENDIAN__ */

template<int size>
void SwapBytes(void *p);

template<>
inline void SwapBytes<4>(void *p)
{
	REUInt32 *c= (REUInt32 *) p;
	*c = ((((*c) & 0xff000000) >> 24) |
		  (((*c) & 0x00ff0000) >> 8 ) |
		  (((*c) & 0x0000ff00) << 8 ) |
		  (((*c) & 0x000000ff) << 24));
	(void)p;
}

template<>
inline void SwapBytes<8>(void *p)
{
	REUInt32 *c = (REUInt32 *) p;
	
	SwapBytes<4>(c);
	SwapBytes<4>(c + 1);
	
	REUInt32 t = *c;
	*c = *(c + 1);
	*(c + 1) = t;
	
	(void)p;
}

template<>
inline void SwapBytes<2>(void *p)
{
	REUInt16 * c= (REUInt16 *) p;
	*c = ((((*c) & 0xff00) >> 8) | (((*c) & 0x00ff) << 8));
	(void) p;
}

template<>
inline void SwapBytes<1>(void * p)
{
	(void)p;
}

template<typename T>
inline void FixByteOrder(T *p)
{
#if defined(__BIG_ENDIAN__)
	SwapBytes<sizeof(T)>(p);
#else
	(void) p;
#endif
}

extern "C" {
	
#if defined(__RE_OS_LINUX__) || defined(_WIN32) || defined(__APPLE__) || defined(__RE_OS_ANDROID__)
	
#ifdef BITMAP_IO_JPEG2000_SUPPORT
	
#ifndef OPJ_STATIC
#define OPJ_STATIC
#endif	
	
#include "../recore/libs/openjpeg/openjpeg.h"
	
#endif /* BITMAP_IO_JPEG2000_SUPPORT */
	
#ifdef BITMAP_IO_JPEG_SUPPORT

#include "../../addlibs/jpeg.h"
	
#endif /* BITMAP_IO_JPEG_SUPPORT */
	
#ifdef BITMAP_IO_PNG_SUPPORT
	
#include "../../addlibs/png.h"
	
#endif /* BITMAP_IO_PNG_SUPPORT */
	
	
	
#else
	

#ifdef BITMAP_IO_JPEG_SUPPORT
	
#include <jpeglib.h>
#include <jerror.h>
	
#endif /* BITMAP_IO_JPEG_SUPPORT */
	
#ifdef BITMAP_IO_JPEG2000_SUPPORT

#ifndef OPJ_STATIC
#define OPJ_STATIC
#endif
	
#include <openjpeg.h>
	
#endif /* BITMAP_IO_JPEG2000_SUPPORT */
	
#ifdef BITMAP_IO_PNG_SUPPORT
	
#include "../../addlibs/png.h"
//#include <png.h>
	
#endif /* BITMAP_IO_PNG_SUPPORT */
	
#endif /* _WIN32 */
	
}

#ifdef BITMAP_IO_JPEG_SUPPORT

struct my_error_mgr {
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr *my_error_ptr;

typedef struct {
	struct jpeg_source_mgr pub;
	
	CBitmapIO::CFile *pFile;
	JOCTET *buffer;
	boolean start_of_file;
} my_source_mgr;

typedef my_source_mgr * my_src_ptr;

#define JPEG_OUTPUT_BUFFER_SIZE    16384

typedef struct {
	struct jpeg_destination_mgr pub;
	
	CBitmapIO::CFile *pFile;
	JOCTET *buffer;
	size_t buffer_size;
} my_dest_mgr;

typedef my_dest_mgr * my_dest_ptr;

// METHODDEF
static void my_error_exit (j_common_ptr cinfo) {
	my_error_ptr myerr = (my_error_ptr) cinfo->err;
	longjmp(myerr->setjmp_buffer, 1);
}

#define JPEG_INPUT_BUF_SIZE    16384

static void jpeg_init_source(j_decompress_ptr cinfo)
{
	my_src_ptr src = (my_src_ptr) cinfo->src;
	src->start_of_file = true;
}

static void jpeg_init_destination(j_compress_ptr cinfo)
{
	my_dest_ptr dest = (my_dest_ptr) cinfo->dest;
	
	dest->buffer = (JOCTET *)
	(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE,
								dest->buffer_size * sizeof(JOCTET));
	
	dest->pub.free_in_buffer = dest->buffer_size;
	dest->pub.next_output_byte = dest->buffer;
}

static boolean jpeg_fill_input_buffer(j_decompress_ptr cinfo)
{
	my_src_ptr src = (my_src_ptr) cinfo->src;
	size_t nbytes;
	
	// nbytes = JFREAD(src->infile, src->buffer, INPUT_BUF_SIZE);
	nbytes = src->pFile->Read(src->buffer, JPEG_INPUT_BUF_SIZE);
	
	if (nbytes <= 0) {
		if (src->start_of_file) /* Treat empty input file as fatal error */
			ERREXIT(cinfo, JERR_INPUT_EMPTY);
		WARNMS(cinfo, JWRN_JPEG_EOF);
		
		src->buffer[0] = (JOCTET) 0xFF;
		src->buffer[1] = (JOCTET) JPEG_EOI;
		
		nbytes = 2;
	}
	
	src->pub.next_input_byte = src->buffer;
	src->pub.bytes_in_buffer = nbytes;
	src->start_of_file = false;
	
	return true;
}

static void jpeg_skip_input_data(j_decompress_ptr cinfo, long num_bytes)
{
	my_src_ptr src = (my_src_ptr) cinfo->src;
	
	if (num_bytes > 0) {
		while (num_bytes > (long) src->pub.bytes_in_buffer) {
			num_bytes -= (long) src->pub.bytes_in_buffer;
			(void) jpeg_fill_input_buffer(cinfo);
		}
		
		src->pub.next_input_byte += (size_t) num_bytes;
		src->pub.bytes_in_buffer -= (size_t) num_bytes;
	}
}

static void jpeg_term_source(j_decompress_ptr cinfo)
{
}

static void jpeg_term_destination(j_compress_ptr cinfo)
{
	my_dest_ptr dest = (my_dest_ptr) cinfo->dest;
	
	dest->pFile->Write(dest->buffer, dest->buffer_size - dest->pub.free_in_buffer);
}

static int jpeg_empty_output_buffer(j_compress_ptr cinfo)
{
	my_dest_ptr dest = (my_dest_ptr) cinfo->dest;
	
	dest->pFile->Write(dest->buffer, dest->buffer_size);
	dest->pub.next_output_byte = dest->buffer;
	dest->pub.free_in_buffer = dest->buffer_size;
	
	return true;
}

static void jpeg_set_cfile_src(j_decompress_ptr cinfo, CBitmapIO::CFile *pFile)
{
	my_src_ptr src;
	
	if (cinfo->src == NULL) {
		cinfo->src = (struct jpeg_source_mgr *)
		(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(my_source_mgr));
		src = (my_src_ptr) cinfo->src;
        src->buffer = (JOCTET *)
		(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT, JPEG_INPUT_BUF_SIZE * sizeof(JOCTET));
	}
	
	src = (my_src_ptr) cinfo->src;
	src->pub.init_source = jpeg_init_source;
	src->pub.fill_input_buffer = jpeg_fill_input_buffer;
	src->pub.skip_input_data = jpeg_skip_input_data;
	src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
	src->pub.term_source = jpeg_term_source;
	src->pub.bytes_in_buffer = 0;
	src->pub.next_input_byte = NULL;
	src->pFile = pFile;
}

static void jpeg_set_cfile_dest(j_compress_ptr cinfo, CBitmapIO::CFile *pFile)
{
	my_dest_ptr dest;
	
	if (cinfo->dest == NULL) {
		cinfo->dest = (struct jpeg_destination_mgr *)
		(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(my_dest_mgr));
		dest = (my_dest_ptr) cinfo->dest;
        dest->buffer = (JOCTET *)
		(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT, JPEG_INPUT_BUF_SIZE * sizeof(JOCTET));
	}
	
	dest = (my_dest_ptr) cinfo->dest;
	dest->buffer_size = JPEG_OUTPUT_BUFFER_SIZE;
	dest->pub.init_destination = jpeg_init_destination;
	dest->pub.term_destination = jpeg_term_destination;
	dest->pub.empty_output_buffer = (boolean (*)(j_compress_ptr))jpeg_empty_output_buffer;
	dest->pFile = pFile;
	
	dest->buffer = NULL;
}

bool CBitmapIO::LoadJPEG(CFile &File, BYTE DefaultAlpha, bool bLoadAlpha, int nAlphaComponent)
{
	assert(nAlphaComponent <= 2);
	
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	
	if (setjmp(jerr.setjmp_buffer))
	{
		jpeg_destroy_decompress(&cinfo);
		
		Free();
		return false;
	}
	
	jpeg_create_decompress(&cinfo);
	jpeg_set_cfile_src(&cinfo, &File);
	
	(void) jpeg_read_header(&cinfo, true);
    (void) jpeg_start_decompress(&cinfo);
	
	if (cinfo.output_components != 1 && cinfo.output_components != 3)
	{
		// g_Log.PrintLn("CGraphics::CBitmap::LoadJPEG() : Invalid number of components : %d", cinfo.output_components);
		
		Free();
		return false;
	}
	
    int buffer_size = cinfo.output_width * cinfo.output_components;
	
	if (bLoadAlpha)
	{
		int Width = cinfo.output_width;
		int Height = cinfo.output_height;
		
		if (m_Width != Width || m_Height != Height)
			Free();
	}
	else
	{
		Free();
		
		m_Width = cinfo.output_width;
		m_Height = cinfo.output_height;
		
		m_pImage = new DWORD[m_Width * m_Height];
	}
	
	if (!m_pImage)
	{
		Free();
		return false;
	}
	
	JSAMPARRAY buffer;
	buffer = (*cinfo.mem->alloc_sarray)
	((j_common_ptr) &cinfo, JPOOL_IMAGE, buffer_size, 1);
	
	// DWORD dwAlpha = ((DWORD) DefaultAlpha) << 24;
	
	BYTE *pDest = (BYTE *) m_pImage;
	
	if (cinfo.output_components == 1)
	{
		while (cinfo.output_scanline < cinfo.output_height)
		{
			(void) jpeg_read_scanlines(&cinfo, buffer, 1);
			
			int nPixels = cinfo.output_width;
			BYTE *pSrc = buffer[0];
			
			if (!bLoadAlpha)
			{
				while (nPixels--)
				{
					BYTE c = *pSrc ++;
					pDest[0] = c;
					pDest[1] = c;
					pDest[2] = c;
					pDest[3] = DefaultAlpha;
					
					pDest += 4;
				}
			}
			else
			{
				while (nPixels--)
				{
					pDest[3] = pSrc[0];
					
					pSrc ++, pDest += 4;
				}
			}
		}
	}
	else
	{
		while (cinfo.output_scanline < cinfo.output_height)
		{
			(void) jpeg_read_scanlines(&cinfo, buffer, 1);
			
			int nPixels = cinfo.output_width;
			BYTE *pSrc = buffer[0];
			
			if (!bLoadAlpha)
			{
				while (nPixels --)
				{
					pDest[0] = pSrc[0];
					pDest[1] = pSrc[1];
					pDest[2] = pSrc[2];
					pDest[3] = DefaultAlpha;
					
					pSrc += 3, pDest += 4;
				}
			}
			else
			{
				while (nPixels --)
				{
					pDest[3] = pSrc[nAlphaComponent];
					
					pSrc += 3, pDest += 4;
				}
			}
		}
	}
	
    (void) jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
	
	return true;
}

#endif /* BITMAP_IO_JPEG_SUPPORT */

#ifdef BITMAP_IO_PNG_SUPPORT

static void png_flush_data(png_structp png_ptr)
{
}

static void png_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
	CBitmapIO::CFile *pFile = (CBitmapIO::CFile *)png_get_io_ptr(png_ptr);// png_ptr->io_ptr;
	assert(pFile);
	
	if (pFile->Read(data, length) != (long) length)
		png_error(png_ptr, "read Error");
}

static void png_write_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
	CBitmapIO::CFile *pFile = (CBitmapIO::CFile *)png_get_io_ptr(png_ptr);// png_ptr->io_ptr;
	assert(pFile);
	
	if (pFile->Write(data, length) != (long) length)
		png_error(png_ptr, "read Error");
}

static void png_error_fn(png_structp png_ptr, png_const_charp error_message)
{
	//longjmp(png_ptr->jmpbuf, 1);
}

static void png_warning_fn(png_structp png_ptr, png_const_charp warning_message)
{
}

bool CBitmapIO::LoadPNG(CFile &File, REUByte DefaultAlpha, bool bLoadAlpha, int nAlphaComponent)
{
	png_structp png_ptr;
	png_infop info_ptr = NULL;
	png_bytep data = NULL;
	
	/*
	 png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
	 png_voidp user_error_ptr, user_error_fn, user_warning_fn);
	 */
	
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
									 NULL, png_error_fn, png_warning_fn);
	if (png_ptr == NULL)
		return false;
	
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		if (data)
			free(data);
		
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		
		Free();
		return false;
	}
	
	info_ptr = png_create_info_struct(png_ptr);
	
	if (info_ptr == NULL)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return false;
	}
	
	png_set_read_fn(png_ptr, (void *) &File, png_read_data);
	
	png_read_info(png_ptr, info_ptr);
	
	int bit_depth = png_get_bit_depth (png_ptr, info_ptr);
	int color_type = png_get_color_type (png_ptr, info_ptr);
	
	if ((color_type == PNG_COLOR_TYPE_PALETTE && bit_depth < 8) ||
		(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) ||
		png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_expand(png_ptr);
	
	int number_passes = 1;
	
	
	if (png_get_interlace_type(png_ptr, info_ptr))
		number_passes = png_set_interlace_handling(png_ptr);
	
	if (png_get_bit_depth(png_ptr, info_ptr) == 16)
		png_set_swap(png_ptr);
	
	png_read_update_info(png_ptr, info_ptr);
	
	int info_ptr_height = png_get_image_height(png_ptr, info_ptr);
	int info_ptr_rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	int info_ptr_width = png_get_image_width(png_ptr, info_ptr);
	
	png_bytep *row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * info_ptr_height);
	
	data = (png_bytep) malloc(info_ptr_height * info_ptr_rowbytes);
	
	
	
	for (int r = 0; r < (int) info_ptr_height; r ++)
		row_pointers[r] = data + info_ptr_rowbytes * r;
	
	png_read_image(png_ptr, row_pointers);
	
	png_read_end(png_ptr, info_ptr);
	
	if (bLoadAlpha)
	{
		int Width = info_ptr_width;
		int Height = info_ptr_height;
		
		if (m_Width != Width || m_Height != Height)
			Free();
	}
	else
	{
		Free();
		
		m_Width = info_ptr_width;
		m_Height = info_ptr_height;
		
		m_pImage = new REUInt32[m_Width * m_Height];
	}
	
	REUByte *pSrc = data;
	REUByte *pDest2 = (REUByte *) m_pImage;
	
	int nPixels = m_Width * m_Height;
	
	color_type = png_get_color_type(png_ptr, info_ptr);// info_ptr->color_type;
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);// info_ptr->bit_depth;
	
	switch (color_type)
	{
        case PNG_COLOR_TYPE_PALETTE:
		{
			REUInt32 *pDest = m_pImage;
			
			REUInt32 Palette[256];
			{
				REUByte *pPaletterDest = (REUByte *) &Palette[0];
				int png_ptr_num_palette = 0;
				png_colorp * png_ptr_palette = 0;
				png_get_PLTE(png_ptr, info_ptr, png_ptr_palette, &png_ptr_num_palette);
				
				for (int i = 0; i < png_ptr_num_palette; i ++)
				{
					pPaletterDest[0] = png_ptr_palette[i]->red;
					pPaletterDest[1] = png_ptr_palette[i]->green;
					pPaletterDest[2] = png_ptr_palette[i]->blue;
					pPaletterDest[3] = DefaultAlpha;
					pPaletterDest += 4;
				}
			}
			
			switch (bit_depth)
			{
				case 2:
				{
					for (int y = 0; y < m_Height; y ++)
						for (int x = 0; x < m_Width;)
						{
							REUByte b = *pSrc ++;
							*pDest ++ = Palette[(b & 0xc0) >> 6];
							if (++x >= m_Width) break;
							*pDest ++ = Palette[(b & 0x30) >> 4];
							if (++x >= m_Width) break;
							*pDest ++ = Palette[(b & 0x0c) >> 2];
							if (++x >= m_Width) break;
							*pDest ++ = Palette[(b & 0x03)     ];
							if (++x >= m_Width) break;
						}
					
					nPixels = 0;
				}
					break;
					
				case 4:
				{
					for (int y = 0; y < m_Height; y ++)
						for (int x = 0; x < m_Width;)
						{
							REUByte b = *pSrc ++;
							*pDest ++ = Palette[(b & 0xf0) >> 4];
							if (++x >= m_Width) break;
							*pDest ++ = Palette[(b & 0x0f)     ];
							if (++x >= m_Width) break;
						}
					
					nPixels = 0;
				}
					break;
					
				case 8:
				{
					while (nPixels --)
						*pDest ++ = Palette[*pSrc ++];
				}
					break;
			}
		}
			break;
			
        case PNG_COLOR_TYPE_GRAY_ALPHA:
			switch (bit_depth)
		{
			case 8:         while (nPixels --)
			{
				pDest2[0] = pSrc[0];
				pDest2[1] = pSrc[0];
				pDest2[2] = pSrc[0];
				pDest2[3] = pSrc[1];
				
				pSrc += 2, pDest2 += 4;
			}
				break;
				
			case 16:        while (nPixels --)
			{
				pDest2[0] = pSrc[1];
				pDest2[1] = pSrc[1];
				pDest2[2] = pSrc[1];
				pDest2[3] = pSrc[3];
				
				pSrc += 4, pDest2 += 4;
			}
				break;
		}
			break;
			
        case PNG_COLOR_TYPE_GRAY:
			if (!bLoadAlpha)
			{
				switch (bit_depth)
				{
					case 8:         while (nPixels --)
					{
						pDest2[0] = pSrc[0];
						pDest2[1] = pSrc[0];
						pDest2[2] = pSrc[0];
						pDest2[3] = DefaultAlpha;
						
						pSrc ++, pDest2 += 4;
					}
						break;
						
					case 16:        while (nPixels --)
					{
						pDest2[0] = pSrc[1];
						pDest2[1] = pSrc[1];
						pDest2[2] = pSrc[1];
						pDest2[3] = DefaultAlpha;
						
						pSrc += 2, pDest2 += 4;
					}
						break;
				}
			}
			else
			{
				switch (bit_depth)
				{
					case 8:         while (nPixels --)
					{
						pDest2[3] = pSrc[0];
						
						pSrc ++, pDest2 += 4;
					}
						break;
						
					case 16:        while (nPixels --)
					{
						pDest2[3] = pSrc[1];
						
						pSrc += 2, pDest2 += 4;
					}
						break;
				}
			}
			break;
			
        case PNG_COLOR_TYPE_RGB:
			switch (bit_depth)
		{
			case 8:         while (nPixels --)
			{
				pDest2[0] = pSrc[0];
				pDest2[1] = pSrc[1];
				pDest2[2] = pSrc[2];
				pDest2[3] = DefaultAlpha;
				
				pSrc += 3, pDest2 += 4;
			}
				break;
				
			case 16:        while (nPixels --)
			{
				pDest2[0] = pSrc[1];
				pDest2[1] = pSrc[3];
				pDest2[2] = pSrc[5];
				pDest2[3] = DefaultAlpha;
				
				pSrc += 6, pDest2 += 4;
			}
				break;
		}
			break;
			
        case PNG_COLOR_TYPE_RGB_ALPHA:
			switch (bit_depth)
		{
			case 8:         while (nPixels --)
			{
				pDest2[0] = pSrc[0];
				pDest2[1] = pSrc[1];
				pDest2[2] = pSrc[2];
				pDest2[3] = pSrc[3];
				
				pSrc += 4, pDest2 += 4;
			}
				break;
				
			case 16:        while (nPixels --)
			{
				pDest2[0] = pSrc[1];
				pDest2[1] = pSrc[3];
				pDest2[2] = pSrc[5];
				pDest2[3] = pSrc[7];
				
				pSrc += 8, pDest2 += 4;
			}
				break;
		}
			break;
	}
	
	free(data);
	free(row_pointers);
	
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	
	if (nPixels > 0)
	{
		Free();
		return false;
	}
	
	return true;
}

#endif /* BITMAP_IO_PNG_SUPPORT */

#pragma pack(1)

typedef struct
{
	unsigned char idlen;
	unsigned char cmtype;
	unsigned char imgtype;
	
	unsigned short cmorg;
	unsigned short cmlen;
	unsigned char cmes;
	
	short xorg;
	short yorg;
	short width;
	short height;
	unsigned char pixsize;
	unsigned char desc;
	
	void FixOrder()
	{
		FixByteOrder(&cmorg);
		FixByteOrder(&cmlen);
		FixByteOrder(&xorg);
		FixByteOrder(&yorg);
		FixByteOrder(&width);
		FixByteOrder(&height);
	}
	
} CTGAHeader;

#pragma pack()

static REUByte TGA_PrevPixel[4];
static int TGA_PixelBytes;
static int TGA_RLE_LeftBytes;
static bool TGA_RLE_bCompressed;
static bool TGA_bCompressedFile;
static CBitmapIO::CFile *pTGASource;

static void ReadTGAPixel(void *pPixel)
{
	if (TGA_bCompressedFile)
	{
		if (TGA_RLE_LeftBytes == 0)
		{
			REUByte Byte;
			pTGASource->Read(&Byte, 1);
			if (Byte > 127)
			{
				TGA_RLE_LeftBytes = Byte - 127;
				TGA_RLE_bCompressed = true;
			}
			else
			{
				TGA_RLE_LeftBytes = Byte + 1;
				TGA_RLE_bCompressed = false;
			}
			
			int nBytes = pTGASource->Read(pPixel, TGA_PixelBytes);
			if (nBytes != TGA_PixelBytes)
			{
#ifndef __RE_OS_ANDROID__								
				throw false;
#endif
				return;
			}
			
			memcpy(TGA_PrevPixel, pPixel, TGA_PixelBytes);
			
			TGA_RLE_LeftBytes --;
			return;
		}
		
		if (!TGA_RLE_bCompressed)
		{
			int nBytes = pTGASource->Read(pPixel, TGA_PixelBytes);
			if (nBytes != TGA_PixelBytes)
			{
#ifndef __RE_OS_ANDROID__				
				throw false;
#endif				
				return;
			}
		}
		else
		{
			memcpy(pPixel, TGA_PrevPixel, TGA_PixelBytes);
		}
		
		TGA_RLE_LeftBytes --;
	}
	else
	{
		int nBytes = pTGASource->Read(pPixel, TGA_PixelBytes);
		if (nBytes != TGA_PixelBytes)
		{
#ifndef __RE_OS_ANDROID__			
			throw false;
#endif
			return;
		}
	}
}

static CTGAHeader TGAHeader;

bool CBitmapIO::CheckIfTGA(CFile &File)
{
	long FilePos = File.Tell();
	
	if (File.Read(&TGAHeader, sizeof(CTGAHeader)) != sizeof(CTGAHeader))
		return false;
	
	TGAHeader.FixOrder();
	
	bool IsNotTGA = (TGAHeader.pixsize !=  8) &&
	(TGAHeader.pixsize != 16) &&
	(TGAHeader.pixsize != 24) &&
	(TGAHeader.pixsize != 32);
	
	IsNotTGA = IsNotTGA || (TGAHeader.width  <= 0);
	IsNotTGA = IsNotTGA || (TGAHeader.height <= 0);
	
	if (IsNotTGA)
		File.Seek(FilePos);
	
	return !IsNotTGA;
}

bool CBitmapIO::LoadTGA(CFile &File, REUByte DefaultAlpha, bool bLoadAlpha, int nAlphaComponent)
{
	bool bHFlip = (TGAHeader.desc & 0x10) ? true : false;
	bool bVFlip = (TGAHeader.desc & 0x20) ? false : true;
	
	TGAHeader.desc &= 0xcf;
	
	TGA_PixelBytes = TGAHeader.pixsize / 8;
	TGA_RLE_bCompressed = false;
	TGA_bCompressedFile = (TGAHeader.imgtype & 8) ? true : false;
	TGA_RLE_LeftBytes = 0;
	pTGASource = &File;
	
	TGAHeader.imgtype &= ~8;
	
	Free();
	
	m_Width = TGAHeader.width;
	m_Height = TGAHeader.height;
	m_pImage = new REUInt32[m_Width * m_Height];
	
	REUInt32 *pDest = m_pImage;
	
	int PixelStepX = !bHFlip ? 1 : -1;
	int PixelStepY = !bVFlip ? (bHFlip ? m_Width * 2 : 0) : (bHFlip ? 0 : -m_Width * 2);
	
	if (bHFlip)
		pDest += m_Width - 1;
	
	if (bVFlip)
		pDest += m_Width * (m_Height - 1);
	
	REUInt32 dwAlpha = ((REUInt32) DefaultAlpha << 24);
	
	File.Skip(TGAHeader.idlen);

#ifndef __RE_OS_ANDROID__				
	try
	{
#endif		
		switch (TGAHeader.pixsize)
		{
			case 32:
			{
				for (int y = 0; y < m_Height; y ++, pDest += PixelStepY)
					for (int x = 0; x < m_Width; x ++, pDest += PixelStepX)
					{
						ReadTGAPixel(pDest);
						
						{
							REUByte *_p = (REUByte *) pDest;
							REUByte t = _p[0];
							_p[0] = _p[2];
							_p[2] = t;
						}
						
					}
			}
				break;
				
			case 24:
			{
				for (int y = 0; y < m_Height; y ++, pDest += PixelStepY)
					for (int x = 0; x < m_Width; x ++, pDest += PixelStepX)
					{
						REUInt32 p = dwAlpha;
						ReadTGAPixel(&p);
						
						{
							REUByte *_p = (REUByte *) &p;
							REUByte t = _p[0];
							_p[0] = _p[2];
							_p[2] = t;
						}
						
						*pDest = p;
					}
			}
				break;
				
			case 16:
			{
				for (int y = 0; y < m_Height; y ++, pDest += PixelStepY)
					for (int x = 0; x < m_Width; x ++, pDest += PixelStepX)
					{
						REUInt16 p;
						ReadTGAPixel(&p);
						*pDest = ((p & 0x7c00) >> 7 ) |
						((p & 0x03e0) << 6 ) |
						((p & 0x001f) << 19) |
						dwAlpha;
					}
			}
				break;
				
			case 8:
			{
				if (TGAHeader.imgtype == 3)
				{
					for (int y = 0; y < m_Height; y ++, pDest += PixelStepY)
						for (int x = 0; x < m_Width; x ++, pDest += PixelStepX)
						{
							REUByte b;
							ReadTGAPixel(&b);
							*pDest = b | (b << 8) | (b << 16) | (b << 24) | dwAlpha;
						}
				}
				else
				{
					REUInt32 Palette[256];
					
					switch(TGAHeader.cmes)
					{
						case 15:
						case 16:
						{
							REUInt16 Palette16[256];
							int nBytes = pTGASource->Read(Palette16, TGAHeader.cmlen * 2);
							if (nBytes != TGAHeader.cmlen * 2)
							{
#ifndef __RE_OS_ANDROID__												
								throw false;
#endif
								return false;
							}
							for (int i = 0; i < TGAHeader.cmlen; i ++)
								Palette[i] = ((Palette16[i] & 0x7c00) >> 7 ) |
								((Palette16[i] & 0x03e0) << 6 ) |
								((Palette16[i] & 0x001f) << 19) |
								dwAlpha;
						}
							break;
							
						case 24:
						{
							for (int i = 0; i < TGAHeader.cmlen; i ++)
							{
								REUInt32 p = 0;
								int nBytes = pTGASource->Read(&p, 3);
								if (nBytes != 3)
								{
#ifndef __RE_OS_ANDROID__													
									throw false;
#endif
									return false;
								}
								Palette[i] = p | dwAlpha;
							}
						}
							break;
							
						case 32:
						{
							pTGASource->Read(Palette, TGAHeader.cmlen * 4);
						}
							break;
							
						default: 
						{
#ifndef __RE_OS_ANDROID__											
							throw false;
#endif
							return false;
						}
					}
					
					for (int y = 0; y < m_Height; y ++, pDest += PixelStepY)
						for (int x = 0; x < m_Width; x ++, pDest += PixelStepX)
						{
							REUByte p;
							ReadTGAPixel(&p);
							*pDest = Palette[p];
						}
				}
			}
				break;
				
			default: 
			{
#ifndef __RE_OS_ANDROID__								
				throw false;
#endif				
				return false;
			}
		}
		
#ifndef __RE_OS_ANDROID__						
	}
	catch(bool)
	{
		Free();
		return false;
	}
#endif
	
	return true;
}

#pragma pack(1)

typedef struct
{
	REUByte    Signature[4];
	REUInt16    nVersion;
	REUByte    Reserved[6];
	REUInt16    nChannels;
	REUInt32   Height;
	REUInt32   Width;
	REUInt16    Depth;
	REUInt16    Mode;
} CPsdHeader;

#pragma pack()

static REUInt16 PSD_PrevWord;
static int PSD_ComponentBytes;
static int PSD_RLE_LeftBytes;
static bool PSD_RLE_bCompressed;
static bool PSD_bCompressedFile;
static CBitmapIO::CFile *pPSDSource;

static void ReadPSDComponent(REUInt16 &Result)
{
	Result = 0;
	
	if (PSD_bCompressedFile)
	{
		if (PSD_RLE_LeftBytes == 0)
		{
			REUByte Byte;
			pPSDSource->Read(&Byte, 1);
			if (Byte >= 128)
			{
				PSD_RLE_LeftBytes = 257 - Byte;
				PSD_RLE_bCompressed = true;
			}
			else
				if (Byte <= 127)
				{
					PSD_RLE_LeftBytes = Byte + 1;
					PSD_RLE_bCompressed = false;
				}
			
			int nBytes = pPSDSource->Read(&Result, PSD_ComponentBytes);
			if (nBytes != PSD_ComponentBytes)
			{
#ifndef __RE_OS_ANDROID__								
				throw false;
#endif
				return;
			}
			
			PSD_PrevWord = Result;
			PSD_RLE_LeftBytes --;
			
			Result = LITTLE_WORD(Result);
			
			return;
		}
		
		if (!PSD_RLE_bCompressed)
		{
			int nBytes = pPSDSource->Read(&Result, PSD_ComponentBytes);
			if (nBytes != PSD_ComponentBytes)
			{
#ifndef __RE_OS_ANDROID__								
				throw false;
#endif
				return;
			}
		}
		else
		{
			Result = PSD_PrevWord;
		}
		
		PSD_RLE_LeftBytes --;
	}
	else
	{
		int nBytes = pPSDSource->Read(&Result, PSD_ComponentBytes);
		if (nBytes != PSD_ComponentBytes)
		{
#ifndef __RE_OS_ANDROID__							
			throw false;
#endif
			return;
		}
	}
	
	Result = LITTLE_WORD(Result);
}

bool CBitmapIO::LoadPSD(CFile &File, REUByte Alpha, bool bLoadAlpha, int nAlphaComponent)
{
	CPsdHeader PsdHeader;
	long nBytes;
	
	nBytes = File.Read(&PsdHeader, sizeof(PsdHeader));
	if (nBytes != sizeof(PsdHeader))
		return false;
	
	PsdHeader.nVersion = BIG_WORD(PsdHeader.nVersion);
	PsdHeader.nChannels = BIG_WORD(PsdHeader.nChannels);
	PsdHeader.Depth = BIG_WORD(PsdHeader.Depth);
	PsdHeader.Mode = BIG_WORD(PsdHeader.Mode);
	PsdHeader.Width = BIG_DWORD(PsdHeader.Width);
	PsdHeader.Height = BIG_DWORD(PsdHeader.Height);
	
	if (PsdHeader.nVersion != 1)
		return false;
	
	if (PsdHeader.nChannels < 1 || PsdHeader.nChannels > 24)
		return false;
	
	if (PsdHeader.Height < 1 || PsdHeader.Width < 1)
		return false;
	
	if (PsdHeader.Depth != 1 && PsdHeader.Depth != 8 && PsdHeader.Depth != 16)
		return false;
	
	Free();
	
	PSD_RLE_bCompressed = false;
	PSD_RLE_LeftBytes = 0;
	
	m_Width  = PsdHeader.Width;
	m_Height = PsdHeader.Height;
	
	int Area = m_Width * m_Height;
	
	REUInt32 ColorMode;
	File.Read(&ColorMode, sizeof(ColorMode));
	ColorMode = BIG_DWORD(ColorMode);
	
	REUInt32 *pPalette = NULL;
	
#ifndef __RE_OS_ANDROID__					
	try
	{
#endif		
		if (PsdHeader.Mode == 2)
		{
			if (ColorMode % 3 != 0)
				return false;
			
			pPalette = new REUInt32[ColorMode];
			int i;
			
			for (i = 0; i < (int) ColorMode / 3; i ++)
			{
				REUByte r;
				File.Read(&r, 1);
				pPalette[i] = r;
			}
			
			for (i = 0; i < (int) ColorMode / 3; i ++)
			{
				REUByte g;
				File.Read(&g, 1);
				pPalette[i] |= g << 8;
			}
			
			for (i = 0; i < (int) ColorMode / 3; i ++)
			{
				REUByte b;
				File.Read(&b, 1);
				pPalette[i] |= b << 16;
				
				pPalette[i] = LITTLE_DWORD(pPalette[i]);
			}
		}
		
		REUInt32 ResourceSize;
		File.Read(&ResourceSize, sizeof(ResourceSize));
		ResourceSize = BIG_DWORD(ResourceSize);
		
		File.Seek(ResourceSize, SEEK_CUR);
		
		REUInt32 MiscInfo;
		File.Read(&MiscInfo, sizeof(MiscInfo));
		MiscInfo = BIG_DWORD(MiscInfo);
		
		File.Seek(MiscInfo, SEEK_CUR);
		
		REUInt16 bCompressed;
		File.Read(&bCompressed, sizeof(bCompressed));
		bCompressed = BIG_WORD(bCompressed);
		
		PSD_bCompressedFile = (bCompressed != 0);
		pPSDSource = &File;
		
		switch (PsdHeader.Depth)
		{
			case 8:         PSD_ComponentBytes = 1; break;
			case 16:        PSD_ComponentBytes = 2; break;
			default:        
			{
#ifndef __RE_OS_ANDROID__								
				throw false;
#endif
				return false;
			};
		}
		
		if (bCompressed)
		{
			int nBytes = m_Height * PsdHeader.nChannels * 2;
			File.Skip(nBytes);
		}
		
		m_pImage = new REUInt32[m_Width * m_Height];
		
		REUByte *pDest = (REUByte *) m_pImage;
		
		switch (PsdHeader.Mode)
		{
			case 1:
			{
				if (PsdHeader.nChannels != 1 && PsdHeader.Depth != 8)
				{
#ifndef __RE_OS_ANDROID__									
					throw false;
#endif
					return false;
				}
				
				REUInt16 c = 0;
				
				if (PSD_ComponentBytes == 1)
				{
					for (int i = 0; i < Area; i ++)
					{
						ReadPSDComponent(c);
						
						pDest[i * 4 + 0] = c;
						pDest[i * 4 + 1] = c;
						pDest[i * 4 + 2] = c;
						pDest[i * 4 + 3] = Alpha;
					}
				}
				else
				{
					for (int i = 0; i < Area; i ++)
					{
						ReadPSDComponent(c);
						c &= 0x00ff;
						
						pDest[i * 4 + 0] = c;
						pDest[i * 4 + 1] = c;
						pDest[i * 4 + 2] = c;
						pDest[i * 4 + 3] = Alpha;
					}
				}
			}
				break;
				
			case 2:
			{
				if (PsdHeader.nChannels != 1 && PsdHeader.Depth != 8)
				{
#ifndef __RE_OS_ANDROID__									
					throw false;
#endif
					return false;
				}
				
				REUInt16 c = 0;
				
				if (PSD_ComponentBytes == 1)
				{
					for (int i = 0; i < Area; i ++)
					{
						ReadPSDComponent(c);
						m_pImage[i] = pPalette[c];
					}
				}
				else
				{
					for (int i = 0; i < Area; i ++)
					{
						ReadPSDComponent(c); c = BIG_WORD(c);
						m_pImage[i] = pPalette[c];
					}
				}
				
			}
				break;
				
			case 3:
			{
				REUInt16 c = 0;
				int i;
				
				if (PSD_ComponentBytes == 1)
				{
					for (i = 0; i < Area; i ++)
					{
						ReadPSDComponent(c);
						
						pDest[i * 4 + 0] = c;
					}
					
					for (i = 0; i < Area; i ++)
					{
						ReadPSDComponent(c);
						
						pDest[i * 4 + 1] = c;
					}
					
					if (PsdHeader.nChannels == 3)
					{
						for (i = 0; i < Area; i ++)
						{
							ReadPSDComponent(c);
							
							pDest[i * 4 + 2] = c;
							pDest[i * 4 + 3] = Alpha;
						}
					}
					else
						if (PsdHeader.nChannels >= 4)
						{
							for (i = 0; i < Area; i ++)
							{
								ReadPSDComponent(c);
								
								pDest[i * 4 + 2] = c;
							}
							
							for (i = 0; i < Area; i ++)
							{
								ReadPSDComponent(c);
								
								pDest[i * 4 + 3] = c;
							}
							
							for (i = 0; i < PsdHeader.nChannels - 4; i ++)
								for (int j = 0; j < Area; j ++)
									ReadPSDComponent(c);
						}
						else
						{
#ifndef __RE_OS_ANDROID__											
							throw false;
#endif
							return false;
						}
				}
				else
				{
					for (i = 0; i < Area; i ++)
					{
						ReadPSDComponent(c);
						c &= 0x00ff;
						
						pDest[i * 4 + 0] = c;
					}
					
					for (i = 0; i < Area; i ++)
					{
						ReadPSDComponent(c);
						c &= 0x00ff;
						
						pDest[i * 4 + 1] = c;
					}
					
					if (PsdHeader.nChannels == 3)
					{
						for (i = 0; i < Area; i ++)
						{
							ReadPSDComponent(c);
							c &= 0x00ff;
							
							pDest[i * 4 + 2] = c;
							pDest[i * 4 + 3] = Alpha;
						}
					}
					else
						if (PsdHeader.nChannels >= 4)
						{
							for (i = 0; i < Area; i ++)
							{
								ReadPSDComponent(c);
								c &= 0x00ff;
								
								pDest[i * 4 + 2] = c;
							}
							
							for (i = 0; i < Area; i ++)
							{
								ReadPSDComponent(c);
								c &= 0x00ff;
								
								pDest[i * 4 + 3] = c;
							}
							
							for (i = 0; i < PsdHeader.nChannels - 4; i ++)
								for (int j = 0; j < Area; j ++)
									ReadPSDComponent(c);
						}
						else
						{
#ifndef __RE_OS_ANDROID__											
							throw false;
#endif
							return false;
						}
				}
			}
				break;
				
			case 4:
			{
				REUInt16 c = 0;
				int i;
				
				if (PSD_ComponentBytes == 1)
				{
					for (i = 0; i < Area; i ++)
					{
						ReadPSDComponent(c);
						pDest[i * 4 + 0] = c;
					}
					
					for (i = 0; i < Area; i ++)
					{
						ReadPSDComponent(c);
						pDest[i * 4 + 1] = c;
					}
					
					for (i = 0; i < Area; i ++)
					{
						ReadPSDComponent(c);
						pDest[i * 4 + 2] = c;
					}
					
					for (i = 0; i < Area; i ++)
					{
						ReadPSDComponent(c);
						pDest[i * 4 + 3] = c;
					}
				}
				else
				{
					for (i = 0; i < Area; i ++)
					{
						ReadPSDComponent(c);
						c &= 0x00ff;
						
						pDest[i * 4 + 0] = c;
					}
					
					for (i = 0; i < Area; i ++)
					{
						ReadPSDComponent(c);
						c &= 0x00ff;
						
						pDest[i * 4 + 1] = c;
					}
					
					for (i = 0; i < Area; i ++)
					{
						ReadPSDComponent(c);
						c &= 0x00ff;
						
						pDest[i * 4 + 2] = c;
					}
					
					for (i = 0; i < Area; i ++)
					{
						ReadPSDComponent(c);
						c &= 0x00ff;
						
						pDest[i * 4 + 3] = c;
					}
				}
				
				for (i = 0; i < Area; i ++)
				{
					int c = (int) pDest[i * 4 + 0];
					int m = (int) pDest[i * 4 + 1];
					int y = (int) pDest[i * 4 + 2];
					int k = (int) pDest[i * 4 + 3];
					
					int r = (c * k) >> 8;
					int g = (m * k) >> 8;
					int b = (y * k) >> 8;
					
					pDest[i * 4 + 0] = r;
					pDest[i * 4 + 1] = g;
					pDest[i * 4 + 2] = b;
					pDest[i * 4 + 3] = Alpha;
				}
				
				if (PsdHeader.nChannels > 4)
				{
					if (PSD_ComponentBytes == 1)
					{
						for (i = 0; i < Area; i ++)
						{
							ReadPSDComponent(c);
							pDest[i * 4 + 3] = c;
						}
					}
					else
					{
						for (i = 0; i < Area; i ++)
						{
							ReadPSDComponent(c);
							c &= 0x00ff;
							pDest[i * 4 + 3] = c;
						}
					}
				}
			}
				break;
				
			default:
			{
#ifndef __RE_OS_ANDROID__								
				throw false;
#endif
				return false;
			}
		}
#ifndef __RE_OS_ANDROID__						
	}
	catch (bool bError)
	{
		(void) bError;
		
		Free();
		return false;
	}
#endif
	
	return true;
}

#pragma pack(1)

struct CDDSHeader
{
	REUByte m_Signature[4];
	
	unsigned int m_Size1;                           // size of the structure (minus MagicNum)
    unsigned int m_Flags1;                              // determines what fields are valid
	unsigned int m_Height;                          // height of surface to be created
    unsigned int m_Width;                               // width of input surface
    unsigned int m_LinearSize;                  // Formless late-allocated optimized surface size
    unsigned int m_Depth;                               // Depth if a volume texture
    unsigned int m_MipMapCount;                 // number of mip-map levels requested
    unsigned int m_AlphaBitDepth;               // depth of alpha buffer requested
	
	unsigned int m_NotUsed[10];
	
	unsigned int m_Size2;                           // size of structure
	unsigned int m_Flags2;                          // pixel format flags
	unsigned int m_FourCC;                          // (FOURCC code)
	unsigned int m_RGBBitCount;                     // how many bits per pixel
	unsigned int m_RBitMask;                        // mask for red bit
	unsigned int m_GBitMask;                        // mask for green bits
	unsigned int m_BBitMask;                        // mask for blue bits
	unsigned int m_RGBAlphaBitMask;         // mask for alpha channel
	
    unsigned int m_ddsCaps1, m_ddsCaps2, m_ddsCaps3, m_ddsCaps4; // direct draw surface capabilities
	unsigned int m_TextureStage;
	
	void FixOrder()
	{
		FixByteOrder(&m_Size1);
		FixByteOrder(&m_Flags1);
		FixByteOrder(&m_Height);
		FixByteOrder(&m_Width);
		FixByteOrder(&m_LinearSize);
		FixByteOrder(&m_Depth);
		FixByteOrder(&m_MipMapCount);
		FixByteOrder(&m_AlphaBitDepth);
		
		FixByteOrder(&m_Size2);
		FixByteOrder(&m_Flags2);
		FixByteOrder(&m_FourCC);
		FixByteOrder(&m_RGBBitCount);
		FixByteOrder(&m_RBitMask);
		FixByteOrder(&m_GBitMask);
		FixByteOrder(&m_BBitMask);
		FixByteOrder(&m_RGBAlphaBitMask);
		
		FixByteOrder(&m_ddsCaps1);
		FixByteOrder(&m_ddsCaps2);
		FixByteOrder(&m_ddsCaps3);
		FixByteOrder(&m_ddsCaps4);
		
		FixByteOrder(&m_TextureStage);
	}
	
};

struct CDDSColor8888
{
	REUByte r;         // change the order of names to change the
	REUByte g;         //  order of the output ARGB or BGRA, etc...
	REUByte b;         //  Last one is MSB, 1st is LSB.
	REUByte a;
};

struct CDDSColor888
{
	REUByte r;         // change the order of names to change the
	REUByte g;         //  order of the output ARGB or BGRA, etc...
	REUByte b;         //  Last one is MSB, 1st is LSB.
};

#ifdef __BIG_ENDIAN__

struct CDDSColor565
{
	unsigned nRed   : 5;
	unsigned nGreen : 6;            //      byte order of output to 32 bit
	unsigned nBlue  : 5;            // order of names changes
};

#else

struct CDDSColor565
{
	unsigned nBlue  : 5;            // order of names changes
	unsigned nGreen : 6;            //      byte order of output to 32 bit
	unsigned nRed   : 5;
};

#endif

struct CDXTColBlock
{
	short m_Col0;
	short m_Col1;
	
	// no bit fields - use bytes
	REUByte m_Row[4];
};

struct CDXTAlphaBlockExplicit
{
	short m_Row[4];
	
} DXTAlphaBlockExplicit;

struct CDXTAlphaBlock3BitLinear
{
	REUByte m_Alpha0;
	REUByte m_Alpha1;
	REUByte m_Stuff[6];
};

#pragma pack()

enum CPixFormat
{
	PF_ARGB,
	PF_RGB,
	PF_DXT1,
	PF_DXT2,
	PF_DXT3,
	PF_DXT4,
	PF_DXT5,
	PF_UNKNOWN
};


#define MAKEFOURCC(ch0, ch1, ch2, ch3)                          \
((REUInt32)(REUByte)(ch0) | ((REUInt32)(REUByte)(ch1) << 8) |                       \
((REUInt32)(REUByte)(ch2) << 16) | ((REUInt32)(REUByte)(ch3) << 24 ))


#define DDS_ALPHAPIXELS                 0x00000001l
#define DDS_ALPHA                               0x00000002l
#define DDS_FOURCC                              0x00000004l
#define DDS_PITCH                               0x00000008l
#define DDS_COMPLEX                             0x00000008l
#define DDS_TEXTURE                             0x00001000l
#define DDS_MIPMAPCOUNT                 0x00020000l
#define DDS_LINEARSIZE                  0x00080000l
#define DDS_VOLUME                              0x00200000l
#define DDS_MIPMAP                              0x00400000l
#define DDS_DEPTH                               0x00800000l

#define DDS_CUBEMAP                             0x00000200L
#define DDS_CUBEMAP_POSITIVEX   0x00000400L
#define DDS_CUBEMAP_NEGATIVEX   0x00000800L
#define DDS_CUBEMAP_POSITIVEY   0x00001000L
#define DDS_CUBEMAP_NEGATIVEY   0x00002000L
#define DDS_CUBEMAP_POSITIVEZ   0x00004000L
#define DDS_CUBEMAP_NEGATIVEZ   0x00008000L

static void DDSGetBitsFromMask(REUInt32 Mask, REUInt32 &ShiftLeft, REUInt32 &ShiftRight)
{
	REUInt32 Temp, i;
	
	if (Mask == 0)
	{
		ShiftLeft = ShiftRight = 0;
		return;
	}
	
	Temp = Mask;
	for (i = 0; i < 32; i++, Temp >>= 1)
		if (Temp & 1)
			break;
	
	ShiftRight = i;
	
	for (i = 0; i < 8; i++, Temp >>= 1)
		if (!(Temp & 1))
			break;
	
	ShiftLeft = 8 - i;
}

bool CBitmapIO::LoadDDS(CFile &File, REUByte Alpha, bool bLoadAlpha, int nAlphaComponent)
{
	CDDSHeader DDSHeader;
	long nBytes;
	
	nBytes = File.Read(&DDSHeader, sizeof(DDSHeader));
	if (nBytes != sizeof(DDSHeader))
		return false;
	
	DDSHeader.FixOrder();
	
	if (DDSHeader.m_Depth == 0)
		DDSHeader.m_Depth = 1;
	
	if (DDSHeader.m_Size1 != 124)
		return false;
	
	if (DDSHeader.m_Size2 != 32)
		return false;
	
	if (DDSHeader.m_Width == 0 || DDSHeader.m_Height == 0)
		return false;
	
	int BlockSize;
	int CompFormat;
	
	if (DDSHeader.m_Flags2 & DDS_FOURCC)
	{
		BlockSize = ((DDSHeader.m_Width + 3) / 4) * ((DDSHeader.m_Height + 3) / 4) * ((DDSHeader.m_Depth + 3) / 4);
		
		switch (DDSHeader.m_FourCC)
		{
			case MAKEFOURCC('D','X','T','1'):
				CompFormat = PF_DXT1;
				BlockSize *= 8;
				break;
				
			case MAKEFOURCC('D','X','T','2'):
				CompFormat = PF_DXT2;
				BlockSize *= 16;
				break;
				
			case MAKEFOURCC('D','X','T','3'):
				CompFormat = PF_DXT3;
				BlockSize *= 16;
				break;
				
			case MAKEFOURCC('D','X','T','4'):
				CompFormat = PF_DXT4;
				BlockSize *= 16;
				break;
				
			case MAKEFOURCC('D','X','T','5'):
				CompFormat = PF_DXT5;
				BlockSize *= 16;
				break;
				
			default:
				CompFormat = PF_UNKNOWN;
				BlockSize *= 16;
				break;
		}
	}
	else
	{
		// This dds texture isn't compressed so write out ARGB format
		if (DDSHeader.m_Flags2 & DDS_ALPHAPIXELS)
		{
			CompFormat = PF_ARGB;
		}
		else
		{
			CompFormat = PF_RGB;
		}
		
		BlockSize = (DDSHeader.m_Width * DDSHeader.m_Height * DDSHeader.m_Depth * (DDSHeader.m_RGBBitCount >> 3));
	}
	
	if (!(DDSHeader.m_Flags1 & (DDS_LINEARSIZE | DDS_PITCH)))
	{
		DDSHeader.m_Flags1 |= DDS_LINEARSIZE;
		DDSHeader.m_LinearSize = BlockSize;
	}
	
	if (DDSHeader.m_ddsCaps1 & DDS_COMPLEX) {
		if (DDSHeader.m_ddsCaps2 & DDS_CUBEMAP) {
			/*
			 TODO
			 if (!iLoadDdsCubemapInternal())
			 return IL_FALSE;
			 return IL_TRUE;
			 */
		}
	}
	
	m_Width    = DDSHeader.m_Width;
	m_Height   = DDSHeader.m_Height;
	
	REUInt32 Depth = DDSHeader.m_Depth;
	
	/* Adjust volume texture */
	
	if (DDSHeader.m_Depth > 1)
	{
		if (!(DDSHeader.m_ddsCaps1 & DDS_COMPLEX) || !(DDSHeader.m_ddsCaps2 & DDS_VOLUME))
		{
			DDSHeader.m_Depth = 1;
			// Depth = 1;
		}
		
		switch (CompFormat)
		{
			case PF_ARGB:
			case PF_RGB:
				DDSHeader.m_LinearSize = MAX(1,DDSHeader.m_Width) * MAX(1, DDSHeader.m_Height) *
				(DDSHeader.m_RGBBitCount / 8);
				break;
				
			case PF_DXT1:
				DDSHeader.m_LinearSize = MAX(1, DDSHeader.m_Width / 4) * MAX(1, DDSHeader.m_Height / 4) * 8;
				break;
				
			case PF_DXT2:
			case PF_DXT3:
			case PF_DXT4:
			case PF_DXT5:
				DDSHeader.m_LinearSize = MAX(1, DDSHeader.m_Width / 4) * MAX(1, DDSHeader.m_Height / 4) * 16;
				break;
		}
		
		DDSHeader.m_Flags1 |= DDS_LINEARSIZE;
		DDSHeader.m_LinearSize *= DDSHeader.m_Depth;
	}
	
	/* Read Data */
	
	REUInt32 Bpp;
	REUInt32 Bps;
	
	if (CompFormat == PF_RGB)
		Bpp = 3;
	else
		Bpp = 4;
	
	REUByte *pCompData;
	
	if (DDSHeader.m_Flags1 & DDS_LINEARSIZE)
	{
		Bps = m_Width * Bpp;
		
		// Head.LinearSize = Head.LinearSize * Depth;
		
		pCompData = (REUByte *) malloc(DDSHeader.m_LinearSize);
		if (pCompData == NULL)
			return false;
		
		if (File.Read(pCompData, DDSHeader.m_LinearSize) != (long) DDSHeader.m_LinearSize)
		{
			free(pCompData);
			return false;
		}
	}
	else
	{
		Bps = m_Width * DDSHeader.m_RGBBitCount / 8;
		REUInt32 CompSize = Bps * m_Height * Depth;
		//REUInt32 CompLineSize = Bps;
		
		pCompData = (REUByte *) malloc(CompSize);
		if (pCompData == NULL)
			return false;
		
		REUByte *pTemp = pCompData;
		
		for (REUInt32 z = 0; z < Depth; z++)
		{
			if (File.Read(pTemp, Bps * m_Height) != (long) (Bps * m_Height))
			{
				free(pCompData);
				return false;
			}
			
			pTemp += Bps * m_Height;
		}
	}
	
	m_pImage = new REUInt32[m_Width * m_Height];
	
	REUByte *pSrc = pCompData;
	
	if (CompFormat == PF_ARGB || CompFormat == PF_RGB)
	{
		REUInt32 Area = m_Width * m_Height /* * m_Depth */;
		REUByte *pDest = (REUByte *) m_pImage;
		REUByte *pSrc = (REUByte *) pCompData;
		
		REUInt32 AlphaL, AlphaR;
		REUInt32 RedL, RedR;
		REUInt32 GreenL, GreenR;
		REUInt32 BlueL, BlueR;
		
		DDSGetBitsFromMask(DDSHeader.m_RBitMask, RedL,   RedR  );
		DDSGetBitsFromMask(DDSHeader.m_GBitMask, GreenL, GreenR);
		DDSGetBitsFromMask(DDSHeader.m_BBitMask, BlueL,  BlueR );
		
		DDSGetBitsFromMask(DDSHeader.m_RGBAlphaBitMask, AlphaL, AlphaR);
		
		REUInt32 BytesPerPixel = DDSHeader.m_RGBBitCount / 8;
		
		while (Area --)
		{
			REUInt32 c = *((REUInt32 *) pSrc); c = LITTLE_DWORD(c);
			pSrc += BytesPerPixel;
			
			((REUByte *) pDest)[0] = (REUByte) (((c & DDSHeader.m_RBitMask) >> RedR  ) << RedL  );
			((REUByte *) pDest)[1] = (REUByte) (((c & DDSHeader.m_GBitMask) >> GreenR) << GreenL);
			((REUByte *) pDest)[2] = (REUByte) (((c & DDSHeader.m_BBitMask) >> BlueR ) << BlueL );
			
			if (Bpp == 4)
			{
				((REUByte *) pDest)[3] = (REUByte) (((c & DDSHeader.m_RGBAlphaBitMask) >> AlphaR) << AlphaL);
				if (AlphaL >= 7)
					((REUByte *) pDest)[3] = ((REUByte *) pDest)[3] ? 0xff : 0x00;
				else
					if (AlphaL >= 4)
						((REUByte *) pDest)[3] |= ((REUByte *) pDest)[3] >> 4;
			}
			else
			{
				((REUByte *) pDest)[3] = 0xff;
			}
			
			pDest += 4;
		}
	}
	else
        if (CompFormat == PF_DXT1)
        {
			for (REUInt32 z = 0; z < Depth; z ++)
			{
				for (REUInt32 y = 0; y < (REUInt32) m_Height; y += 4)
				{
					for (REUInt32 x = 0; x < (REUInt32) m_Width; x += 4)
					{
						REUInt16 WColor0 = *((REUInt16 *) (pSrc + 0)); FixByteOrder(&WColor0);
						REUInt16 WColor1 = *((REUInt16 *) (pSrc + 2)); FixByteOrder(&WColor1);
						CDDSColor565 *pColor0 = ((CDDSColor565 *) &WColor0);
						CDDSColor565 *pColor1 = ((CDDSColor565 *) &WColor1);
						REUInt32 Bitmask = ((REUInt32 *) pSrc)[1]; Bitmask = LITTLE_DWORD(Bitmask);
						pSrc += 8;
						
						CDDSColor8888 Colors[4];
						
						Colors[0].r = pColor0->nRed << 3;
						Colors[0].g = pColor0->nGreen << 2;
						Colors[0].b = pColor0->nBlue << 3;
						Colors[0].a = 0xff;
						
						Colors[1].r = pColor1->nRed << 3;
						Colors[1].g = pColor1->nGreen << 2;
						Colors[1].b = pColor1->nBlue << 3;
						Colors[1].a = 0xff;
						
						if (WColor0 > WColor1)
						{
							// Four-color block: derive the other two colors.
							// 00 = color_0, 01 = color_1, 10 = color_2, 11 = color_3
							// These 2-bit codes correspond to the 2-bit fields
							// stored in the 64-bit block.
							
							Colors[2].b = (2 * Colors[0].b + Colors[1].b + 1) / 3;
							Colors[2].g = (2 * Colors[0].g + Colors[1].g + 1) / 3;
							Colors[2].r = (2 * Colors[0].r + Colors[1].r + 1) / 3;
							Colors[2].a = 0xff;
							
							Colors[3].b = (Colors[0].b + 2 * Colors[1].b + 1) / 3;
							Colors[3].g = (Colors[0].g + 2 * Colors[1].g + 1) / 3;
							Colors[3].r = (Colors[0].r + 2 * Colors[1].r + 1) / 3;
							Colors[3].a = 0xff;
						}
						else
						{
							// Three-color block: derive the other color.
							// 00 = color_0,  01 = color_1,  10 = color_2,
							// 11 = transparent.
							// These 2-bit codes correspond to the 2-bit fields
							// stored in the 64-bit block.
							
							Colors[2].b = (Colors[0].b + Colors[1].b) / 2;
							Colors[2].g = (Colors[0].g + Colors[1].g) / 2;
							Colors[2].r = (Colors[0].r + Colors[1].r) / 2;
							Colors[2].a = 0xff;
							
							Colors[3].b = (Colors[0].b + 2 * Colors[1].b + 1) / 3;
							Colors[3].g = (Colors[0].g + 2 * Colors[1].g + 1) / 3;
							Colors[3].r = (Colors[0].r + 2 * Colors[1].r + 1) / 3;
							Colors[3].a = 0x00;
						}
						
						for (REUInt32 j = 0, k = 0; j < 4; j ++) {
							for (REUInt32 i = 0; i < 4; i ++, k ++) {
								
								REUInt32 Select = (Bitmask & (0x03 << k * 2)) >> k * 2;
								CDDSColor8888 *pCol = &Colors[Select];
								
								if ((x + i < (REUInt32) m_Width) && (y + j < (REUInt32) m_Height))
								{
									REUInt32 Offset = /* z * Image->SizeOfPlane + */ (y + j) * Bps + (x + i) * Bpp;
									((REUByte *) m_pImage)[Offset + 0] = pCol->r;
									((REUByte *) m_pImage)[Offset + 1] = pCol->g;
									((REUByte *) m_pImage)[Offset + 2] = pCol->b;
									((REUByte *) m_pImage)[Offset + 3] = pCol->a;
								}
							}
						}
					}
				}
			}
        }
        else
			if (CompFormat == PF_DXT2 || CompFormat == PF_DXT3)
			{
                for (REUInt32 z = 0; z < Depth; z ++)
                {
					for (REUInt32 y = 0; y < (REUInt32) m_Height; y += 4)
					{
						for (REUInt32 x = 0; x < (REUInt32) m_Width; x += 4)
						{
							CDXTAlphaBlockExplicit *pAlpha = (CDXTAlphaBlockExplicit *) pSrc;
							REUInt16 WColor0 = *((REUInt16 *) (pSrc +  8)); WColor0 = LITTLE_WORD(WColor0);
							REUInt16 WColor1 = *((REUInt16 *) (pSrc + 10)); WColor1 = LITTLE_WORD(WColor1);
							CDDSColor565 *pColor0 = ((CDDSColor565 *) &WColor0);
							CDDSColor565 *pColor1 = ((CDDSColor565 *) &WColor1);
							REUInt32 Bitmask = *((REUInt32 *) (pSrc + 12)); Bitmask = LITTLE_DWORD(Bitmask);
							pSrc += 16;
							
							CDDSColor8888 Colors[4];
							
							Colors[0].r = pColor0->nRed << 3;
							Colors[0].g = pColor0->nGreen << 2;
							Colors[0].b = pColor0->nBlue << 3;
							Colors[0].a = 0xff;
							
							Colors[1].r = pColor1->nRed << 3;
							Colors[1].g = pColor1->nGreen << 2;
							Colors[1].b = pColor1->nBlue << 3;
							Colors[1].a = 0xff;
							
							Colors[2].b = (2 * Colors[0].b + Colors[1].b + 1) / 3;
							Colors[2].g = (2 * Colors[0].g + Colors[1].g + 1) / 3;
							Colors[2].r = (2 * Colors[0].r + Colors[1].r + 1) / 3;
							Colors[2].a = 0xff;
							
							Colors[3].b = (Colors[0].b + 2 * Colors[1].b + 1) / 3;
							Colors[3].g = (Colors[0].g + 2 * Colors[1].g + 1) / 3;
							Colors[3].r = (Colors[0].r + 2 * Colors[1].r + 1) / 3;
							Colors[3].a = 0xff;
							
							REUInt32 k = 0;
							REUInt32 j;
							
							for (j = 0; j < 4; j ++)
							{
								for (REUInt32 i = 0; i < 4; i ++, k ++)
								{
									REUInt32 Select = (Bitmask & (0x03 << k * 2)) >> k * 2;
									CDDSColor8888 *pCol = &Colors[Select];
									
									if ((x + i < (REUInt32) m_Width) && (y + j < (REUInt32) m_Height))
									{
										REUInt32 Offset = /* z * Image->SizeOfPlane + */ (y + j) * Bps + (x + i) * Bpp;
										((REUByte *) m_pImage)[Offset + 0] = pCol->r;
										((REUByte *) m_pImage)[Offset + 1] = pCol->g;
										((REUByte *) m_pImage)[Offset + 2] = pCol->b;
									}
								}
							}
							
							for (j = 0; j < 4; j++)
							{
								REUInt16 Word = pAlpha->m_Row[j]; Word = LITTLE_WORD(Word);
								
								for (REUInt32 i = 0; i < 4; i ++, k ++)
								{
									if ((x + i < (REUInt32) m_Width) && (y + j < (REUInt32) m_Height))
									{
										REUInt32 Offset = /* z * Image->SizeOfPlane + */ (y + j) * Bps + (x + i) * Bpp;
										((REUByte *) m_pImage)[Offset + 3] = (Word & 0x0f) | (Word << 4);
									}
									
									Word >>= 4;
								}
							}
						}
					}
                }
				
                if (CompFormat == PF_DXT2)
                {
					REUInt32 Area = m_Width * m_Height /* * m_Depth */;
					REUByte *pDest = (REUByte *) m_pImage;
					
					while (Area --)
					{
						if (pDest[3])
						{
							pDest[0] = (REUByte) ((((REUInt32) pDest[0]) << 8) / pDest[3]);
							pDest[1] = (REUByte) ((((REUInt32) pDest[1]) << 8) / pDest[3]);
							pDest[2] = (REUByte) ((((REUInt32) pDest[2]) << 8) / pDest[3]);
						}
						
						pDest += 4;
					}
                }
			}
			else
				if (CompFormat == PF_DXT4 || CompFormat == PF_DXT5)
				{
					for (REUInt32 z = 0; z < Depth; z ++)
					{
                        for (REUInt32 y = 0; y < (REUInt32) m_Height; y += 4)
                        {
							for (REUInt32 x = 0; x < (REUInt32) m_Width; x += 4)
							{
								REUByte Alpha[8];
								REUByte *pAlphaMask;
								
								Alpha[0] = pSrc[0];
								Alpha[1] = pSrc[1];
								pAlphaMask = pSrc + 2;
								
								REUInt16 WColor0 = *((REUInt16 *) (pSrc +  8)); WColor0 = LITTLE_WORD(WColor0);
								REUInt16 WColor1 = *((REUInt16 *) (pSrc + 10)); WColor1 = LITTLE_WORD(WColor1);
								CDDSColor565 *pColor0 = ((CDDSColor565 *) &WColor0);
								CDDSColor565 *pColor1 = ((CDDSColor565 *) &WColor1);
								REUInt32 Bitmask = *((REUInt32 *) (pSrc + 12)); Bitmask = LITTLE_DWORD(Bitmask);
								pSrc += 16;
								CDDSColor8888 Colors[4];
								
								Colors[0].r = pColor0->nRed << 3;
								Colors[0].g = pColor0->nGreen << 2;
								Colors[0].b = pColor0->nBlue << 3;
								Colors[0].a = 0xff;
								
								Colors[1].r = pColor1->nRed << 3;
								Colors[1].g = pColor1->nGreen << 2;
								Colors[1].b = pColor1->nBlue << 3;
								Colors[1].a = 0xff;
								
								Colors[2].b = (2 * Colors[0].b + Colors[1].b + 1) / 3;
								Colors[2].g = (2 * Colors[0].g + Colors[1].g + 1) / 3;
								Colors[2].r = (2 * Colors[0].r + Colors[1].r + 1) / 3;
								Colors[2].a = 0xff;
								
								Colors[3].b = (Colors[0].b + 2 * Colors[1].b + 1) / 3;
								Colors[3].g = (Colors[0].g + 2 * Colors[1].g + 1) / 3;
								Colors[3].r = (Colors[0].r + 2 * Colors[1].r + 1) / 3;
								Colors[3].a = 0xff;
								
								REUInt32 k = 0;
								REUInt32 j;
								
								for (j = 0; j < 4; j ++)
								{
									for (REUInt32 i = 0; i < 4; i ++, k ++)
									{
										REUInt32 Select = (Bitmask & (0x03 << k * 2)) >> k * 2;
										CDDSColor8888 *pCol = &Colors[Select];
										
										if ((x + i < (REUInt32) m_Width) && (y + j < (REUInt32) m_Height))
										{
											REUInt32 Offset = /* z * Image->SizeOfPlane + */ (y + j) * Bps + (x + i) * Bpp;
											((REUByte *) m_pImage)[Offset + 0] = pCol->r;
											((REUByte *) m_pImage)[Offset + 1] = pCol->g;
											((REUByte *) m_pImage)[Offset + 2] = pCol->b;
										}
									}
								}
								
								if (Alpha[0] > Alpha[1])
								{
									Alpha[2] = (6 * Alpha[0] + 1 * Alpha[1] + 3) / 7;
									Alpha[3] = (5 * Alpha[0] + 2 * Alpha[1] + 3) / 7;
									Alpha[4] = (4 * Alpha[0] + 3 * Alpha[1] + 3) / 7;
									Alpha[5] = (3 * Alpha[0] + 4 * Alpha[1] + 3) / 7;
									Alpha[6] = (2 * Alpha[0] + 5 * Alpha[1] + 3) / 7;
									Alpha[7] = (1 * Alpha[0] + 6 * Alpha[1] + 3) / 7;
								}
								else
								{
									Alpha[2] = (4 * Alpha[0] + 1 * Alpha[1] + 2) / 5;
									Alpha[3] = (3 * Alpha[0] + 2 * Alpha[1] + 2) / 5;
									Alpha[4] = (2 * Alpha[0] + 3 * Alpha[1] + 2) / 5;
									Alpha[5] = (1 * Alpha[0] + 4 * Alpha[1] + 2) / 5;
									Alpha[6] = 0x00;
									Alpha[7] = 0xff;
								}
								
								REUInt32 Bits = *((REUInt32 *) pAlphaMask); Bits = LITTLE_DWORD(Bits);
								
								for (j = 0; j < 2; j++)
								{
									for (REUInt32 i = 0; i < 4; i ++)
									{
										if ((x + i < (REUInt32) m_Width) && (y + j < (REUInt32) m_Height))
										{
											REUInt32 Offset = /* z * Image->SizeOfPlane + */ (y + j) * Bps + (x + i) * Bpp;
											((REUByte *) m_pImage)[Offset + 3] = Alpha[Bits & 0x07];
										}
										
										Bits >>= 3;
									}
								}
								
								Bits = *((REUInt32 *) &pAlphaMask[3]); Bits = LITTLE_DWORD(Bits);
								
								for (j = 2; j < 4; j++)
								{
									for (REUInt32 i = 0; i < 4; i ++)
									{
										if ((x + i < (REUInt32) m_Width) && (y + j < (REUInt32) m_Height))
										{
											REUInt32 Offset = /* z * Image->SizeOfPlane + */ (y + j) * Bps + (x + i) * Bpp;
											((REUByte *) m_pImage)[Offset + 3] = Alpha[Bits & 0x07];
										}
										
										Bits >>= 3;
									}
								}
							}
                        }
					}
					
					if (CompFormat == PF_DXT4)
					{
                        REUInt32 Area = m_Width * m_Height /* * m_Depth */;
                        REUByte *pDest = (REUByte *) m_pImage;
						
                        while (Area --)
                        {
							if (pDest[3])
							{
								pDest[0] = (REUByte) ((((REUInt32) pDest[0]) << 8) / pDest[3]);
								pDest[1] = (REUByte) ((((REUInt32) pDest[1]) << 8) / pDest[3]);
								pDest[2] = (REUByte) ((((REUInt32) pDest[2]) << 8) / pDest[3]);
							}
							
							pDest += 4;
                        }
					}
				}
				else
				{
					assert(0);
				}
	
	free(pCompData);
	
	return true;
}

CBitmapIO::EFormats CBitmapIO::DetermineFormat(FILE *pFile)
{
	CStdioFile File(pFile);
	return DetermineFormat(File);
}

CBitmapIO::EFormats CBitmapIO::DetermineFormat(void *pData, long nBytes)
{
	CMemoryFile File(pData, nBytes);
	return DetermineFormat(File);
}

static CBitmapIO::CFormatInfo g_FormatList[] =
{
	CBitmapIO::CFormatInfo(CBitmapIO::FORMAT_PSD,                   "psd"),
	CBitmapIO::CFormatInfo(CBitmapIO::FORMAT_PNG,                   "png"),
	CBitmapIO::CFormatInfo(CBitmapIO::FORMAT_TGA,                   "tga"),
	CBitmapIO::CFormatInfo(CBitmapIO::FORMAT_JPEG,                  "jpg"),
	CBitmapIO::CFormatInfo(CBitmapIO::FORMAT_JPEG2000,              "j2k"),
	CBitmapIO::CFormatInfo(CBitmapIO::FORMAT_DDS,                   "dds")
};

static int g_Formats = sizeof(g_FormatList) / sizeof(g_FormatList[0]);

CBitmapIO::EFormats CBitmapIO::GetFormatByExtension(const char *pExtension)
{
	for (int i = 0; i < g_Formats; i ++)
		if (strcmp(g_FormatList[i].GetExtension(), pExtension) == 0)
			return g_FormatList[i].GetFormat();
	
	return FORMAT_UNKNOWN;
}

const char *CBitmapIO::GetFormatExtension(EFormats Format)
{
	for (int i = 0; i < g_Formats; i ++)
		if (g_FormatList[i].GetFormat() == Format)
			return g_FormatList[i].GetExtension();
	
	return NULL;
}

CBitmapIO::EFormats CBitmapIO::DetermineFormat(CFile &File)
{
	REUInt32 Sig;
	REUInt32 Sig2;
	long FilePos = File.Tell();
	
	if (File.Read(&Sig, 4) != 4)
	{
		File.Seek(FilePos);
		return FORMAT_UNKNOWN;
	}
	
	FixByteOrder(&Sig);
	
	if (File.Read(&Sig2, 4) != 4)
	{
		File.Seek(FilePos);
		return FORMAT_UNKNOWN;
	}
	
	FixByteOrder(&Sig2);
	
	File.Seek(FilePos);
	
	if (Sig == 0x474e5089) /* PNG */
		return FORMAT_PNG;
	
	if (Sig == 0xe0ffd8ff) /* JPEG */
		return FORMAT_JPEG;
	
	if (Sig == 0x53504238)
		return FORMAT_PSD;
	
	if (Sig == 0x20534444)
		return FORMAT_DDS;
	
	if (Sig2 == 0x2020506a)
		return FORMAT_JPEG2000;
	
	if (CheckIfTGA(File))
		return FORMAT_TGA;
	
	return FORMAT_UNKNOWN;
}

bool CBitmapIO::Save( CFile &File, const CSaveOptions &SaveOptions )
{
	switch (SaveOptions.GetFormat())
	{
        case FORMAT_DDS:
			return SaveDDS(File, SaveOptions.GetDDSFormat());
			
#ifdef BITMAP_IO_JPEG_SUPPORT
        case FORMAT_JPEG:
			return SaveJPEG(File, SaveOptions.GetJPEGQuality(), SaveOptions.GetComponent());
#endif /* BITMAP_IO_JPEG_SUPPORT */
			
#ifdef BITMAP_IO_JPEG2000_SUPPORT
        case FORMAT_JPEG2000:
			return SaveJPEG2000(File, SaveOptions.GetComponent());
#endif /* BITMAP_IO_JPEG2000_SUPPORT */
			
#ifdef BITMAP_IO_PNG_SUPPORT
        case FORMAT_PNG:
			return SavePNG(File, SaveOptions.GetComponent());
#endif /* BITMAP_IO_PNG_SUPPORT */
			
        case FORMAT_TGA:
			return SaveTGA(File);
	}
	
	return false;
}

bool CBitmapIO::Load(CFile &File, REUByte Alpha, bool bLoadAlpha, int nAlphaComponent)
{
	EFormats Format = DetermineFormat(File);
	
	switch (Format)
	{
        case FORMAT_DDS:
			return LoadDDS(File, Alpha, bLoadAlpha, nAlphaComponent);
			
#ifdef BITMAP_IO_PNG_SUPPORT
        case FORMAT_PNG:
			return LoadPNG(File, Alpha, bLoadAlpha, nAlphaComponent);
#endif /* BITMAP_IO_PNG_SUPPORT */
			
#ifdef BITMAP_IO_JPEG_SUPPORT
        case FORMAT_JPEG:
			return LoadJPEG(File, Alpha, bLoadAlpha, nAlphaComponent);
#endif /* BITMAP_IO_JPEG_SUPPORT */
			
        case FORMAT_PSD:
			return LoadPSD(File, Alpha, bLoadAlpha, nAlphaComponent);
			
#ifdef BITMAP_IO_JPEG2000_SUPPORT
        case FORMAT_JPEG2000:
			return LoadJPEG2000(File, Alpha, bLoadAlpha, nAlphaComponent);
#endif /* BITMAP_IO_JPEG2000_SUPPORT */
			
        case FORMAT_TGA:
			return LoadTGA(File, Alpha, bLoadAlpha, nAlphaComponent);
	}
	
	return false;
}

bool CBitmapIO::Load(FILE *pFile, REUByte Alpha, bool bLoadAlpha, int nAlphaComponent)
{
	CStdioFile File(pFile);
	return Load(File, Alpha, bLoadAlpha, nAlphaComponent);
}

bool CBitmapIO::Load(void *pData, long nBytes, REUByte Alpha, bool bLoadAlpha, int nAlphaComponent)
{
	CMemoryFile File(pData, nBytes);
	return Load(File, Alpha, bLoadAlpha, nAlphaComponent);
}

bool CBitmapIO::SaveTGA(CFile &File)
{
	assert(m_pImage);
	assert(m_Width > 0 && m_Height > 0);
	
	static REUByte Header[0x12] = {
		0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x20, 0x00
	};
	
	Header[0x0c] = (REUByte) m_Width;
	Header[0x0d] = (REUByte) (m_Width >> 8);
	
	Header[0x0e] = (REUByte) m_Height;
	Header[0x0f] = (REUByte) (m_Height >> 8);
	
	File.Write(Header, sizeof(Header));
	
	REUByte *pBuffer = new REUByte[m_Width * 4];
	
	for (int i = 0; i < m_Height; i ++)
	{
		REUByte *pSrc = (REUByte *) (m_pImage + (m_Height - 1 - i) * m_Width);
		REUByte *pDest = pBuffer;
		
		for (int j = 0; j < m_Width; j ++, pSrc += 4, pDest += 4)
		{
			pDest[0] = pSrc[2];
			pDest[1] = pSrc[1];
			pDest[2] = pSrc[0];
			pDest[3] = pSrc[3];
		}
		
		File.Write(pBuffer, m_Width * 4);
	}
	
	delete pBuffer;
	
	return true;
}

bool CBitmapIO::SaveTGA(FILE *pFile)
{
	CStdioFile File(pFile);
	return SaveTGA(File);
}

bool CBitmapIO::SaveTGA(REUByte *&pData, long &nBytes)
{
	CMemoryFile File;
	bool bOK = SaveTGA(File);
	if (!bOK)
	{
		pData = NULL;
		nBytes = 0;
		
		return false;
	}
	
	File.Compact();
	
	pData = File.GetData();
	nBytes = File.GetSize();
	
	File.DontFreeData();
	
	return true;
}

#ifdef BITMAP_IO_JPEG_SUPPORT

bool CBitmapIO::SaveJPEG(CFile &File, int Quality, int nComponent)
{
	struct jpeg_compress_struct cinfo;
	struct my_error_mgr jerr;
	JSAMPROW row_pointer[1];
	
	assert(nComponent <= 3);
	
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	
	if (setjmp(jerr.setjmp_buffer))
	{
		jpeg_destroy_compress(&cinfo);
		
		return false;
	}
	
	jpeg_create_compress(&cinfo);
	jpeg_set_cfile_dest(&cinfo, &File);
	
	bool bGrayScale = (nComponent >= 0);
	
	cinfo.image_width = m_Width;
	cinfo.image_height = m_Height;
	cinfo.input_components = bGrayScale ? 1 : 3;
	
	cinfo.in_color_space = bGrayScale ? JCS_GRAYSCALE : JCS_RGB;
	
	jpeg_set_defaults(&cinfo);
	
	cinfo.write_JFIF_header = true;
	
	jpeg_set_quality(&cinfo, Quality, true);
	
	jpeg_start_compress(&cinfo, true);
	
	BYTE *pBuffer = new BYTE[m_Width * cinfo.input_components];
	BYTE *pSrc = (BYTE *) m_pImage;
	
	row_pointer[0] = pBuffer;
	
	while (cinfo.next_scanline < cinfo.image_height) {
		
		if (nComponent < 0)
		{
			BYTE *pDest = pBuffer;
			
			for (int i = 0; i < m_Width; i ++)
			{
				pDest[0] = pSrc[0];
				pDest[1] = pSrc[1];
				pDest[2] = pSrc[2];
				
				pSrc += 4, pDest += 3;
			}
		}
		else
		{
			BYTE *pDest = pBuffer;
			
			for (int i = 0; i < m_Width; i ++)
			{
				*pDest ++ = pSrc[nComponent];
				pSrc += 4;
			}
		}
		
		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}
	
	delete pBuffer;
	
	jpeg_finish_compress(&cinfo);
	
    jpeg_destroy_compress(&cinfo);
	
	return true;
}

bool CBitmapIO::SaveJPEG(FILE *pFile, int Quality, int nComponent)
{
	CStdioFile File(pFile);
	return SaveJPEG(File, Quality, nComponent);
}

bool CBitmapIO::SaveJPEG(BYTE *&pData, long &nBytes, int Quality, int nComponent)
{
	CMemoryFile File;
	bool bOK = SaveJPEG(File, Quality, nComponent);
	if (!bOK)
	{
		pData = NULL;
		nBytes = 0;
		
		return false;
	}
	
	File.Compact();
	
	pData = File.GetData();
	nBytes = File.GetSize();
	
	File.DontFreeData();
	
	return true;
}

#endif /* BITMAP_IO_JPEG_SUPPORT */

#ifdef BITMAP_IO_JPEG2000_SUPPORT

bool CBitmapIO::LoadJPEG2000(CFile &File, BYTE DefaultAlpha, bool bLoadAlpha, int nAlphaComponent)
{
	opj_dparameters_t parameters;
	opj_set_default_decoder_parameters(&parameters);
	
	opj_dinfo_t *dinfo = opj_create_decompress(CODEC_JP2);
	
	opj_set_event_mgr((opj_common_ptr)dinfo, NULL, NULL);
	
	File.Seek(0, SEEK_END);
	long FileSize = File.Tell();
	File.Seek(0, SEEK_SET);
	
	BYTE *pBuffer = (BYTE *) malloc(sizeof(BYTE) * FileSize);
	File.Read(pBuffer, FileSize);
	
	opj_setup_decoder(dinfo, &parameters);
	
	opj_cio_t *cio = opj_cio_open((opj_common_ptr)dinfo, pBuffer, FileSize);
	
	opj_image_t *image = opj_decode(dinfo, cio);
	
	if (!image)
	{
		opj_destroy_decompress(dinfo);
		opj_cio_close(cio);
		free(pBuffer);
		return false;
	}
	
	opj_cio_close(cio);
	free(pBuffer);
	
	m_Width = image->x1;
	m_Height = image->y1;
	
	m_pImage = new DWORD[m_Width * m_Height];
	
	if (image->numcomps == 3)
	{
		BYTE *pDest = (BYTE *) m_pImage;
		
		for (int i = 0; i < m_Width * m_Height; i ++, pDest += 4)
		{
			pDest[0] = image->comps[0].data[i];
			pDest[1] = image->comps[1].data[i];
			pDest[2] = image->comps[2].data[i];
			pDest[3] = DefaultAlpha;
		}
	}
	else
	{
		BYTE *pDest = (BYTE *) m_pImage;
		
		for (int i = 0; i < m_Width * m_Height; i ++, pDest += 4)
		{
			pDest[0] = image->comps[0].data[i];
			pDest[1] = image->comps[0].data[i];
			pDest[2] = image->comps[0].data[i];
			pDest[3] = DefaultAlpha;
		}
	}
	
	opj_image_destroy(image);
	
	opj_destroy_decompress(dinfo);
	
	return true;
}

bool CBitmapIO::SaveJPEG2000(CFile &File, int Quality, int nComponent)
{
	opj_cparameters_t parameters;
	
	opj_image_cmptparm_t cmptparm[3];
	
	memset(&cmptparm[0], 0, sizeof(opj_image_cmptparm_t));
	
	cmptparm[0].x0 = 0;
	cmptparm[0].y0 = 0;
	cmptparm[0].w = m_Width;
	cmptparm[0].h = m_Height;
	cmptparm[0].dx = 1;
	cmptparm[0].dy = 1;
	cmptparm[0].prec =
	cmptparm[0].bpp = 8;
	cmptparm[0].sgnd = 0;
	
	cmptparm[1] = cmptparm[0];
	cmptparm[2] = cmptparm[0];
	
	bool bGrayScale = (nComponent >= 0);
	
	OPJ_COLOR_SPACE color_space = bGrayScale ? CLRSPC_GRAY : CLRSPC_SRGB;
	
	opj_image_t *image = opj_image_create(nComponent >= 0 ? 1 : 3, &cmptparm[0], color_space);
	
	image->x0 = 0;
	image->y0 = 0;
	image->x1 = m_Width;
	image->y1 = m_Height;
	
	if (nComponent < 0)
	{
		BYTE *pSrc = (BYTE *) m_pImage;
		
		for (int i = 0; i < m_Width * m_Height; i ++, pSrc += 4)
		{
			image->comps[0].data[i] = pSrc[0];
			image->comps[1].data[i] = pSrc[1];
			image->comps[2].data[i] = pSrc[2];
		}
	}
	else
	{
		BYTE *pSrc = (BYTE *) m_pImage;
		
		for (int i = 0; i < m_Width * m_Height; i ++, pSrc += 4)
		{
			image->comps[0].data[i] = pSrc[nComponent];
		}
	}
	
	opj_cinfo_t *cinfo = opj_create_compress(CODEC_JP2);
	
	opj_set_event_mgr((opj_common_ptr)cinfo, NULL, NULL);
	
	opj_set_default_encoder_parameters(&parameters);
	
	parameters.tcp_rates[parameters.tcp_numlayers] = Quality;
	parameters.tcp_numlayers ++;
	
	parameters.tcp_mct = image->numcomps == 3 ? 1 : 0;
	
	parameters.cp_disto_alloc = 1;
	
	opj_setup_encoder(cinfo, &parameters, image);
	
	opj_cio_t *cio = opj_cio_open((opj_common_ptr)cinfo, NULL, 0);
	
	bool bSuccess = opj_encode(cinfo, cio, image, parameters.index);
	if (!bSuccess) {
		opj_cio_close(cio);
		return false;
	}
	
	opj_image_destroy(image);
	
	int codestream_length = cio_tell(cio);
	
	File.Write(cio->buffer, codestream_length);
	
	opj_cio_close(cio);
	
	opj_destroy_compress(cinfo);
	
	return true;
}

bool CBitmapIO::SaveJPEG2000(FILE *pFile, int Quality, int nComponent)
{
	CStdioFile File(pFile);
	return SaveJPEG2000(File, Quality, nComponent);
}

bool CBitmapIO::SaveJPEG2000(BYTE *&pData, long &nBytes, int Quality, int nComponent)
{
	CMemoryFile File;
	bool bOK = SaveJPEG2000(File, Quality, nComponent);
	if (!bOK)
	{
		pData = NULL;
		nBytes = 0;
		
		return false;
	}
	
	File.Compact();
	
	pData = File.GetData();
	nBytes = File.GetSize();
	
	File.DontFreeData();
	
	return true;
}

#endif /* BITMAP_IO_JPEG2000_SUPPORT */

static REUInt32 DDSDistance(CDDSColor888 &c0, CDDSColor888 &c1)
{
	return (c0.r - c1.r) * (c0.r - c1.r) +
	(c0.g - c1.g) * (c0.g - c1.g) +
	(c0.b - c1.b) * (c0.b - c1.b);
}

static REUInt32 DDSRMSAlpha(REUByte *pOrig, REUByte *pTest)
{
	REUInt32 RMS = 0;
	
	for (int i = 0; i < 16; i ++)
	{
		REUInt32 d = pOrig[i] - pTest[i];
		RMS += d * d;
	}
	
	return RMS;
}

static void DDSGetBlock(REUInt16 *pBlock, REUInt32 *pData, REUInt32 ImageWidth, REUInt32 ImageHeight, bool bPreMult, REUInt32 XPos, REUInt32 YPos)
{
	REUInt32 i = 0;
	
	for (REUInt32 y = 0; y < 4; y ++)
	{
		REUInt32 iy = YPos + y;
		
		if (iy >= ImageHeight)
			iy = ImageHeight - 1;
		
		for (REUInt32 x = 0; x < 4; x ++)
		{
			REUInt32 ix = XPos + x;
			
			if (ix >= ImageWidth)
				ix = ImageWidth - 1;
			
			REUInt32 Offset = iy * ImageWidth + ix;
			REUByte *pSrc = (REUByte *) (pData + Offset);
			
			REUByte r = pSrc[0];
			REUByte g = pSrc[1];
			REUByte b = pSrc[2];
			
			if (bPreMult)
			{
				REUByte a = pSrc[3];
				
				r = (REUByte) (((REUInt32) r) * a >> 8);
				g = (REUByte) (((REUInt32) g) * a >> 8);
				b = (REUByte) (((REUInt32) b) * a >> 8);
			}
			
			pBlock[i] = ((r >> 3) << (6 + 5)) | ((g >> 2) << 5) | (b >> 3);
			
			i ++;
		}
	}
}

static void DDSGetAlphaBlock(REUByte *pBlock, REUInt32 *pData, REUInt32 ImageWidth, REUInt32 ImageHeight, REUInt32 XPos, REUInt32 YPos)
{
	REUInt32 i = 0;
	
	for (REUInt32 y = 0; y < 4; y ++)
	{
		REUInt32 iy = YPos + y;
		
		if (iy >= ImageHeight)
			iy = ImageHeight - 1;
		
		for (REUInt32 x = 0; x < 4; x ++)
		{
			REUInt32 ix = XPos + x;
			
			if (ix >= ImageWidth)
				ix = ImageWidth - 1;
			
			REUInt32 Offset = iy * ImageWidth + ix;
			REUInt32 d = pData[Offset];
			FixByteOrder(&d);
			pBlock[i ++] = (REUByte) (d >> 24);
		}
	}
}

static void DDSShortToColor565(REUByte Pixel, CDDSColor565 &Color)
{
	Color.nRed   = (Pixel & 0xF800) >> 11;
	Color.nGreen = (Pixel & 0x07E0) >> 5;
	Color.nBlue  = (Pixel & 0x001F);
}

static void DDSShortToColor888(REUInt16 Pixel, CDDSColor888 &Color)
{
	Color.r = ((Pixel & 0xF800) >> 11) << 3;
	Color.g = ((Pixel & 0x07E0) >> 5)  << 2;
	Color.b = ((Pixel & 0x001F))       << 3;
}

static REUInt16 DDSColor565ToShort(CDDSColor565 &Color)
{
	return (Color.nRed << 11) | (Color.nGreen << 5) | (Color.nBlue);
}

static REUInt16 DDSColor888ToShort(CDDSColor888 &Color)
{
	return ((Color.r >> 3) << 11) | ((Color.g >> 2) << 5) | (Color.b >> 3);
}

static REUInt32 DDSGenBitMask(REUInt16 ex0, REUInt16 ex1, REUInt32 NumCols, REUInt16 *pIn, REUByte *pAlpha, CDDSColor888 *pOutCol)
{
	REUInt32 i, j, Closest, Dist, BitMask = 0;
	REUByte Mask[16];
	CDDSColor888 c, Colors[4];
	
	DDSShortToColor888(ex0, Colors[0]);
	DDSShortToColor888(ex1, Colors[1]);
	
	if (NumCols == 3)
	{
		Colors[2].r = (Colors[0].r + Colors[1].r) / 2;
		Colors[2].g = (Colors[0].g + Colors[1].g) / 2;
		Colors[2].b = (Colors[0].b + Colors[1].b) / 2;
		Colors[3].r = (Colors[0].r + Colors[1].r) / 2;
		Colors[3].g = (Colors[0].g + Colors[1].g) / 2;
		Colors[3].b = (Colors[0].b + Colors[1].b) / 2;
	}
	else
	{
		// NumCols == 4
		Colors[2].r = (2 * Colors[0].r + Colors[1].r + 1) / 3;
		Colors[2].g = (2 * Colors[0].g + Colors[1].g + 1) / 3;
		Colors[2].b = (2 * Colors[0].b + Colors[1].b + 1) / 3;
		Colors[3].r = (Colors[0].r + 2 * Colors[1].r + 1) / 3;
		Colors[3].g = (Colors[0].g + 2 * Colors[1].g + 1) / 3;
		Colors[3].b = (Colors[0].b + 2 * Colors[1].b + 1) / 3;
	}
	
	for (i = 0; i < 16; i++)
	{
		if (pAlpha)
		{
			// Test to see if we have 1-bit transparency
			if (pAlpha[i] < 128)
			{
				Mask[i] = 3;  // Transparent
				
				if (pOutCol)
				{
					pOutCol[i].r = Colors[3].r;
					pOutCol[i].g = Colors[3].g;
					pOutCol[i].b = Colors[3].b;
				}
				
				continue;
			}
		}
		
		// If no transparency, try to find which colour is the closest.
		Closest = ~0;
		DDSShortToColor888(pIn[i], c);
		
		for (j = 0; j < NumCols; j++)
		{
			Dist = DDSDistance(c, Colors[j]);
			
			if (Dist < Closest)
			{
				Closest = Dist;
				Mask[i] = (REUByte) j;
				
				if (pOutCol)
				{
					pOutCol[i].r = Colors[j].r;
					pOutCol[i].g = Colors[j].g;
					pOutCol[i].b = Colors[j].b;
				}
			}
		}
	}
	
	for (i = 0; i < 16; i++)
		BitMask |= (Mask[i] << (i * 2));
	
	return BitMask;
}

static void DDSGenAlphaBitMask(REUByte a0, REUByte a1, REUInt32 Num, REUByte *pIn, REUByte *pMask, REUByte *pOut)
{
	REUByte Alphas[8], M[16];
	REUInt32 i, j, Closest, Dist;
	
	Alphas[0] = a0;
	Alphas[1] = a1;
	
	// 8-alpha or 6-alpha block?
	if (Num == 8)
	{
		// 8-alpha block:  derive the other six alphas.
		// Bit code 000 = alpha_0, 001 = alpha_1, others are interpolated.
		Alphas[2] = (6 * Alphas[0] + 1 * Alphas[1] + 3) / 7;    // bit code 010
		Alphas[3] = (5 * Alphas[0] + 2 * Alphas[1] + 3) / 7;    // bit code 011
		Alphas[4] = (4 * Alphas[0] + 3 * Alphas[1] + 3) / 7;    // bit code 100
		Alphas[5] = (3 * Alphas[0] + 4 * Alphas[1] + 3) / 7;    // bit code 101
		Alphas[6] = (2 * Alphas[0] + 5 * Alphas[1] + 3) / 7;    // bit code 110
		Alphas[7] = (1 * Alphas[0] + 6 * Alphas[1] + 3) / 7;    // bit code 111
	}
	else
	{
		// 6-alpha block.
		// Bit code 000 = alpha_0, 001 = alpha_1, others are interpolated.
		Alphas[2] = (4 * Alphas[0] + 1 * Alphas[1] + 2) / 5;    // Bit code 010
		Alphas[3] = (3 * Alphas[0] + 2 * Alphas[1] + 2) / 5;    // Bit code 011
		Alphas[4] = (2 * Alphas[0] + 3 * Alphas[1] + 2) / 5;    // Bit code 100
		Alphas[5] = (1 * Alphas[0] + 4 * Alphas[1] + 2) / 5;    // Bit code 101
		Alphas[6] = 0x00;                                                                               // Bit code 110
		Alphas[7] = 0xFF;                                                                               // Bit code 111
	}
	
	for (i = 0; i < 16; i++)
	{
		Closest = ~0;
		for (j = 0; j < 8; j++) {
			Dist = abs((int) pIn[i] - (int) Alphas[j]);
			if (Dist < Closest) {
				Closest = Dist;
				M[i] = (REUByte) j;
			}
		}
	}
	
	if (pOut)
	{
		for (i = 0; i < 16; i++)
			pOut[i] = Alphas[M[i]];
	}
	
	// First three bytes.
	pMask[0] = (M[0]) | (M[1] << 3) | ((M[2] & 0x03) << 6);
	pMask[1] = (M[2] & 0x04) | (M[3] << 1) | (M[4] << 4) | ((M[5] & 0x01) << 7);
	pMask[2] = (M[5] & 0x06) | (M[6] << 2) | (M[7] << 5);
	
	// Second three bytes.
	pMask[3] = (M[8]) | (M[9] << 3) | ((M[10] & 0x03) << 6);
	pMask[4] = (M[10] & 0x04) | (M[11] << 1) | (M[12] << 4) | ((M[13] & 0x01) << 7);
	pMask[5] = (M[13] & 0x06) | (M[14] << 2) | (M[15] << 5);
}

static void DDSChooseEndpoints(REUInt16 *pBlock, REUInt16 &Ex0, REUInt16 &Ex1)
{
	REUInt32 i, j;
	CDDSColor888 Colors[16];
	int Farthest = -1;
	
	for (i = 0; i < 16; i ++)
		DDSShortToColor888(pBlock[i], Colors[i]);
	
	for (i = 0; i < 16; i ++)
	{
		for (j = i + 1; j < 16; j ++)
		{
			int d = (int) DDSDistance(Colors[i], Colors[j]);
			
			if (d > Farthest)
			{
				Farthest = d;
				Ex0 = pBlock[i];
				Ex1 = pBlock[j];
			}
		}
	}
}

static void DDSChooseAlphaEndpoints(REUByte *pBlock, REUByte &a0, REUByte &a1)
{
	REUByte Lowest = 0xFF, Highest = 0;
	
	for (REUInt32 i = 0; i < 16; i ++)
	{
		if (pBlock[i] < Lowest)
		{
			Lowest = pBlock[i];
		}
		else
			if (pBlock[i] > Highest)
			{
				Highest = pBlock[i];
			}
	}
	
	a0 = Highest;  // a0 is the higher of the two.
	a1 = Lowest;   // a1 is the lower of the two.
}

static void DDSCorrectEndDXT1(REUInt16 &Ex0, REUInt16 &Ex1, bool bHasAlpha)
{
	if (bHasAlpha)
	{
		if (Ex0 > Ex1)
		{
			REUInt16 Temp = Ex0;
			Ex0 = Ex1;
			Ex1 = Temp;
		}
	}
	else
	{
		if (Ex0 < Ex1)
		{
			REUInt16 Temp = Ex0;
			Ex0 = Ex1;
			Ex1 = Temp;
		}
	}
}

static void DDSPreMult(REUInt16 *pData, REUByte *pAlpha)
{
	for (REUInt32 i = 0; i < 16; i++)
	{
		CDDSColor888 Color;
		
		DDSShortToColor888(pData[i], Color);
		
		Color.r = (REUByte) (((REUInt32) Color.r * pAlpha[i]) >> 8);
		Color.g = (REUByte) (((REUInt32) Color.g * pAlpha[i]) >> 8);
		Color.b = (REUByte) (((REUInt32) Color.b * pAlpha[i]) >> 8);
		
		pData[i] = DDSColor888ToShort(Color);
		// DDSShortToColor888(pData[i], Color);
	}
}

bool CBitmapIO::SaveDDS(CFile &File, EDDSFormat Format)
{
	CDDSHeader DDSHeader;
	
	memset(&DDSHeader, 0, sizeof(DDSHeader));
	
	DDSHeader.m_Signature[0] = 'D';
	DDSHeader.m_Signature[1] = 'D';
	DDSHeader.m_Signature[2] = 'S';
	DDSHeader.m_Signature[3] = ' ';
	
	DDSHeader.m_Size1  = 124;
	DDSHeader.m_Width  = m_Width;
	DDSHeader.m_Height = m_Height;
	DDSHeader.m_Flags1 = DDS_LINEARSIZE;
	
	DDSHeader.m_MipMapCount = 1;
	
	if (Format == DDS_DXT1)
		DDSHeader.m_LinearSize = m_Width * m_Height / 16 * 8;
	else    DDSHeader.m_LinearSize = m_Width * m_Height / 16 * 16;
	
	DDSHeader.m_Size2 = 32;
	
	REUInt32 FourCC;
	
	switch (Format)
	{
        case DDS_DXT1:  FourCC = MAKEFOURCC('D', 'X', 'T', '1'); break;
        case DDS_DXT2:  FourCC = MAKEFOURCC('D', 'X', 'T', '2'); break;
        case DDS_DXT3:  FourCC = MAKEFOURCC('D', 'X', 'T', '3'); break;
        case DDS_DXT4:  FourCC = MAKEFOURCC('D', 'X', 'T', '4'); break;
        case DDS_DXT5:  FourCC = MAKEFOURCC('D', 'X', 'T', '5'); break;
        default:                return false;
	}
	
	DDSHeader.m_Flags2 = DDS_FOURCC | DDS_TEXTURE;
	DDSHeader.m_FourCC = FourCC;
	
	{
		CDDSHeader DDSHeaderSave = DDSHeader;
		DDSHeaderSave.FixOrder();
		
		File.Write(&DDSHeaderSave, sizeof(DDSHeaderSave));
	}
	
	REUInt32 x, y, i;
	REUByte AlphaBlock[16], AlphaBitMask[6], AlphaOut[16];
	REUByte a0, a1;
	
	REUInt32 ImageWidth = m_Width;
	REUInt32 ImageHeight = m_Height;
	REUInt32 *pImage = m_pImage;
	
	bool bPreMult = (Format == DDS_DXT2 || Format == DDS_DXT4);
	
	REUInt16 Block[16], ex0, ex1;
	
	REUByte *pOutput = new REUByte[DDSHeader.m_LinearSize];
	
	REUByte *pDest = pOutput;
	
	switch (Format)
	{
        case DDS_DXT1:
			for (y = 0; y < (REUInt32) m_Height; y += 4)
			{
				for (x = 0; x < (REUInt32) m_Width; x += 4)
				{
					DDSGetAlphaBlock(AlphaBlock, pImage, ImageWidth, ImageHeight, x, y);
					
					bool bHasAlpha = false;
					
					for (i = 0 ; i < 16; i++)
						if (AlphaBlock[i] < 128)
						{
							bHasAlpha = true;
							break;
						}
					
					DDSGetBlock(Block, pImage, ImageWidth, ImageHeight, bPreMult, x, y);
					DDSChooseEndpoints(Block, ex0, ex1);
					DDSCorrectEndDXT1(ex0, ex1, bHasAlpha);
					
					REUInt32 BitMask;
					
					if (bHasAlpha)
						BitMask = DDSGenBitMask(ex0, ex1, 3, Block, AlphaBlock, NULL);
					else    BitMask = DDSGenBitMask(ex0, ex1, 4, Block, NULL, NULL);
					
					((REUInt16 *) pDest)[0] = LITTLE_WORD(ex0);
					((REUInt16 *) pDest)[1] = LITTLE_WORD(ex1);
					((REUInt32 *) pDest)[1] = LITTLE_DWORD(BitMask);
					
					pDest += 8;
				}
			}
			break;
			
        case DDS_DXT2:
        case DDS_DXT3:
			for (y = 0; y < (REUInt32) m_Height; y += 4)
			{
				for (REUInt32 x = 0; x < (REUInt32) m_Width; x += 4)
				{
					DDSGetAlphaBlock(AlphaBlock, pImage, ImageWidth, ImageHeight, x, y);
					
					for (i = 0; i < 16; i += 2)
						pDest[i / 2] = (REUByte) (((AlphaBlock[i] >> 4) << 4) | (AlphaBlock[i + 1] >> 4));
					
					DDSGetBlock(Block, pImage, ImageWidth, ImageHeight, bPreMult, x, y);
					DDSChooseEndpoints(Block, ex0, ex1);
					
					REUInt32 BitMask = DDSGenBitMask(ex0, ex1, 4, Block, NULL, NULL);
					
					((REUInt16 *) pDest)[4] = LITTLE_WORD(ex0);
					((REUInt16 *) pDest)[5] = LITTLE_WORD(ex1);
					((REUInt32 *) pDest)[3] = LITTLE_DWORD(BitMask);
					
					pDest += 16;
				}
			}
			break;
			
        case DDS_DXT4:
        case DDS_DXT5:
			for (y = 0; y < (REUInt32) m_Height; y += 4)
			{
				for (x = 0; x < (REUInt32) m_Width; x += 4)
				{
					DDSGetAlphaBlock(AlphaBlock, pImage, ImageWidth, ImageHeight, x, y);
					DDSChooseAlphaEndpoints(AlphaBlock, a0, a1);
					DDSGenAlphaBitMask(a0, a1, 6, AlphaBlock, AlphaBitMask, AlphaOut);
					
					DDSGetBlock(Block, pImage, ImageWidth, ImageHeight, bPreMult, x, y);
					DDSChooseEndpoints(Block, ex0, ex1);
					
					REUInt32 BitMask = DDSGenBitMask(ex0, ex1, 4, Block, NULL, NULL);
					
					pDest[0] = a0;
					pDest[1] = a1;
					
					pDest[2] = AlphaBitMask[0];
					pDest[3] = AlphaBitMask[1];
					pDest[4] = AlphaBitMask[2];
					pDest[5] = AlphaBitMask[3];
					pDest[6] = AlphaBitMask[4];
					pDest[7] = AlphaBitMask[5];
					
					((REUInt16 *) pDest)[4] = LITTLE_WORD(ex0);
					((REUInt16 *) pDest)[5] = LITTLE_WORD(ex1);
					((REUInt32 *) pDest)[3] = LITTLE_DWORD(BitMask);
					
					pDest += 16;
				}
			}
			break;
	}
	
	File.Write(pOutput, pDest - pOutput);
	
	delete pOutput;
	
	return true;
}

bool CBitmapIO::SaveDDS(FILE *pFile, EDDSFormat Format)
{
	CStdioFile File(pFile);
	return SaveDDS(File, Format);
}

bool CBitmapIO::SaveDDS(REUByte *&pData, long &nBytes, EDDSFormat Format)
{
	CMemoryFile File;
	bool bOK = SaveDDS(File, Format);
	if (!bOK)
	{
		pData = NULL;
		nBytes = 0;
		
		return false;
	}
	
	File.Compact();
	
	pData = File.GetData();
	nBytes = File.GetSize();
	
	File.DontFreeData();
	
	return true;
}

#ifdef BITMAP_IO_PNG_SUPPORT

bool CBitmapIO::SavePNG(CFile &File, bool bSaveAlpha, int nComponent)
{
	assert(nComponent <= 3);
	
	REUByte **pRowBuffer = NULL;
	png_structp png_ptr;
	png_infop info_ptr = NULL;
	//png_bytep data = NULL;
	
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
									  NULL, png_error_fn, png_warning_fn);
	if (png_ptr == NULL)
		return false;
	
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		if (pRowBuffer)
			delete pRowBuffer;
		
		png_destroy_write_struct(&png_ptr, NULL);
		
		return false;
	}
	
	info_ptr = png_create_info_struct(png_ptr);
	
	if (info_ptr == NULL)
	{
		png_destroy_write_struct(&png_ptr, NULL);
		return false;
	}
	
	png_set_write_fn(png_ptr, (void *) &File, png_write_data, png_flush_data);
	
	int PngType = PNG_COLOR_TYPE_RGB_ALPHA;
	
	if (bSaveAlpha)
		PngType = PNG_COLOR_TYPE_RGB_ALPHA;
	else
        if (nComponent < 0)
			PngType = PNG_COLOR_TYPE_RGB;
        else    PngType = PNG_COLOR_TYPE_GRAY;
	
	int BitDepth = 8;
	
	png_set_IHDR(png_ptr, info_ptr, m_Width, m_Height, BitDepth, PngType,
				 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	
	png_write_info(png_ptr, info_ptr);
	
	png_set_swap(png_ptr);
	
	pRowBuffer = new REUByte*[m_Height];
	
	if (bSaveAlpha)
	{
		for (int i = 0; i < m_Height; i ++)
			pRowBuffer[i] = (REUByte *) (m_pImage + m_Width * i);
		
		png_write_image(png_ptr, pRowBuffer);
		png_write_end(png_ptr, info_ptr);
	}
	else
        if (nComponent < 0)
        {
			REUByte *pImage = new REUByte[m_Width * m_Height * 3];
			
			REUByte *pSrc = (REUByte *) m_pImage;
			REUByte *pDest = (REUByte *) pImage;
			
			for (int y = 0; y < m_Height; y ++)
			{
				pRowBuffer[y] = pDest;
				
				for (int x = 0; x < m_Width; x ++)
				{
					pDest[0] = pSrc[0];
					pDest[1] = pSrc[1];
					pDest[2] = pSrc[2];
					
					pSrc += 4, pDest += 3;
				}
			}
			
			png_write_image(png_ptr, pRowBuffer);
			png_write_end(png_ptr, info_ptr);
			
			delete pImage;
        }
        else
        {
			REUByte *pImage = new REUByte[m_Width * m_Height];
			
			REUByte *pSrc = (REUByte *) m_pImage;
			REUByte *pDest = (REUByte *) pImage;
			
			for (int y = 0; y < m_Height; y ++)
			{
				pRowBuffer[y] = pDest;
				
				for (int x = 0; x < m_Width; x ++)
				{
					*pDest = pSrc[nComponent];
					pSrc += 4, pDest ++;
				}
			}
			
			png_write_image(png_ptr, pRowBuffer);
			png_write_end(png_ptr, info_ptr);
			
			delete pImage;
        }
	
	delete pRowBuffer;
	
	return true;
}

bool CBitmapIO::SavePNG(FILE *pFile, bool bSaveAlpha, int nComponent)
{
	CStdioFile File(pFile);
	return SavePNG(File, bSaveAlpha, nComponent);
}

bool CBitmapIO::SavePNG(REUByte *&pData, long &nBytes, bool bSaveAlpha, int nComponent)
{
	CMemoryFile File;
	bool bOK = SavePNG(File, bSaveAlpha, nComponent);
	if (!bOK)
	{
		pData = NULL;
		nBytes = 0;
		
		return false;
	}
	
	File.Compact();
	
	pData = File.GetData();
	nBytes = File.GetSize();
	
	File.DontFreeData();
	
	return true;
}

#endif /* BITMAP_IO_PNG_SUPPORT */

#endif /* ENGINE_FLASH */
