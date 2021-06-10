#include "image.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <syslog.h>
#include <sys/stat.h>
#include <jpeglib.h>

void image::reset(string filename)
{
	init = false; cleanup();

	int rc, i, j;

	struct stat file_info;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	length = 0;
	pixelsize = 0; 
	int stride = 0;

	rc = stat(filename.c_str(), &file_info);
	if (rc) return;

	unsigned long jpg_size = file_info.st_size;
	unsigned char *jpg_buffer = new unsigned char[jpg_size + 100];
	if(jpg_buffer == NULL) return;

	int fd = open(filename.c_str(), O_RDONLY);
	i = 0;
	while (i < jpg_size)
	{
		rc = read(fd, jpg_buffer + i, jpg_size - i);
		i += rc;
	}

	close(fd);

	cinfo.err = jpeg_std_error(&jerr);

	jpeg_create_decompress(&cinfo);
	jpeg_mem_src(&cinfo, jpg_buffer, jpg_size);

	rc = jpeg_read_header(&cinfo, TRUE);
	if (rc != 1) return;

	jpeg_start_decompress(&cinfo);

	width = cinfo.output_width;
	height = cinfo.output_height;
	pixelsize = cinfo.output_components;

	length = width * height * pixelsize;
	buffer = (unsigned char*) new unsigned char[length];
	if (buffer == NULL) return;

	stride = width * pixelsize;

	while (cinfo.output_scanline < cinfo.output_height)
	{
		unsigned char *buffer_array[1];
		buffer_array[0] = buffer + (cinfo.output_scanline) * stride;

		jpeg_read_scanlines(&cinfo, buffer_array, 1);
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	delete[] jpg_buffer;

	init = true;
}

legacy::graphics *image::getGraphics()
{
	return NULL;
}

colour image::get(unsigned long x, unsigned long y)
{
	colour result;
	unsigned char *t2[] = {&result.red, &result.green, &result.blue, &result.alpha };

	unsigned char *t = (unsigned char*)&result;

	int offset = ((y * width) + x) * pixelsize;
	for(int i=0;i<pixelsize;++i) //t[i] = buffer[offset+i];
		*t2[i] = buffer[offset + i];

	//result = buffer[offset];

	//result = *((colour*)&buffer[offset]);
	/*result.red = buffer[offset];
	result.green = buffer[offset+1];
	result.blue = buffer[offset+2];*/
	//result.alpha = buffer[offset+3];

	return result;
}

image *image::createImage(string filename)
{
	return new image(filename);
	/*
	init = false; cleanup();

	init = true;
	*/
}

image *image::createImage(int width, int height)
{
	return NULL;
}
/*
image *image::createImageOld(string filename)
{
    int rc, i, j;


	char *syslog_prefix = (char*) malloc(1024);
	sprintf(syslog_prefix, "%s", argv[0]);
	openlog(syslog_prefix, LOG_PERROR | LOG_PID, LOG_USER);

	if (argc != 2) {
		fprintf(stderr, "USAGE: %s filename.jpg\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// Variables for the source jpg
	struct stat file_info;
	unsigned long jpg_size;
	unsigned char *jpg_buffer;

	// Variables for the decompressor itself
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	// Variables for the output buffer, and how long each row is
	unsigned long bmp_size;
	unsigned char *bmp_buffer;
	int row_stride, width, height, pixel_size;


	// Load the jpeg data from a file into a memory buffer for 
	// the purpose of this demonstration.
	// Normally, if it's a file, you'd use jpeg_stdio_src, but just
	// imagine that this was instead being downloaded from the Internet
	// or otherwise not coming from disk
	rc = stat(argv[1]filename.c_str(), &file_info);
	if (rc) {
		//syslog(LOG_ERR, "FAILED to stat source jpg");
		exit(EXIT_FAILURE);
	}
	jpg_size = file_info.st_size;
	jpg_buffer = (unsigned char*) malloc(jpg_size + 100);

	int fd = open(filename.c_str(), O_RDONLY);
	i = 0;
	while (i < jpg_size) {
		// HERE
		//rc = read(fd, jpg_buffer + i, jpg_size - i);
		//syslog(LOG_INFO, "Input: Read %d/%lu bytes", rc, jpg_size-i);
		i += rc;
	}
	// HERE
	//close(fd);

//   SSS    TTTTTTT     A     RRRR     TTTTTTT

	//syslog(LOG_INFO, "Proc: Create Decompress struct");
	// Allocate a new decompress struct, with the default error handler.
	// The default error handler will exit() on pretty much any issue,
	// so it's likely you'll want to replace it or supplement it with
	// your own.
	cinfo.err = jpeg_std_error(&jerr);	
	jpeg_create_decompress(&cinfo);


	//syslog(LOG_INFO, "Proc: Set memory buffer as source");
	// Configure this decompressor to read its data from a memory 
	// buffer starting at unsigned char *jpg_buffer, which is jpg_size
	// long, and which must contain a complete jpg already.
	//
	// If you need something fancier than this, you must write your 
	// own data source manager, which shouldn't be too hard if you know
	// what it is you need it to do. See jpeg-8d/jdatasrc.c for the 
	// implementation of the standard jpeg_mem_src and jpeg_stdio_src 
	// managers as examples to work from.
	jpeg_mem_src(&cinfo, jpg_buffer, jpg_size);


	//syslog(LOG_INFO, "Proc: Read the JPEG header");
	// Have the decompressor scan the jpeg header. This won't populate
	// the cinfo struct output fields, but will indicate if the
	// jpeg is valid.
	rc = jpeg_read_header(&cinfo, TRUE);

	if (rc != 1) {
		//syslog(LOG_ERR, "File does not seem to be a normal JPEG");
		exit(EXIT_FAILURE);
	}

	//syslog(LOG_INFO, "Proc: Initiate JPEG decompression");
	// By calling jpeg_start_decompress, you populate cinfo
	// and can then allocate your output bitmap buffers for
	// each scanline.
	jpeg_start_decompress(&cinfo);
	
	width = cinfo.output_width;
	height = cinfo.output_height;
	pixel_size = cinfo.output_components;

	//syslog(LOG_INFO, "Proc: Image is %d by %d with %d components", 
		//	width, height, pixel_size);

    // HERE
	bmp_size = width * height * pixel_size;
	bmp_buffer = (unsigned char*) malloc(bmp_size);

	// The row_stride is the total number of bytes it takes to store an
	// entire scanline (row). 
	row_stride = width * pixel_size;


//	syslog(LOG_INFO, "Proc: Start reading scanlines");
	//
	// Now that you have the decompressor entirely configured, it's time
	// to read out all of the scanlines of the jpeg.
	//
	// By default, scanlines will come out in RGBRGBRGB...  order, 
	// but this can be changed by setting cinfo.out_color_space
	//
	// jpeg_read_scanlines takes an array of buffers, one for each scanline.
	// Even if you give it a complete set of buffers for the whole image,
	// it will only ever decompress a few lines at a time. For best 
	// performance, you should pass it an array with cinfo.rec_outbuf_height
	// scanline buffers. rec_outbuf_height is typically 1, 2, or 4, and 
	// at the default high quality decompression setting is always 1.
	while (cinfo.output_scanline < cinfo.output_height) {
		unsigned char *buffer_array[1];
		buffer_array[0] = bmp_buffer + \
						   (cinfo.output_scanline) * row_stride;

		jpeg_read_scanlines(&cinfo, buffer_array, 1);

	}
	//syslog(LOG_INFO, "Proc: Done reading scanlines");


	// Once done reading *all* scanlines, release all internal buffers,
	// etc by calling jpeg_finish_decompress. This lets you go back and
	// reuse the same cinfo object with the same settings, if you
	// want to decompress several jpegs in a row.
	//
	// If you didn't read all the scanlines, but want to stop early,
	// you instead need to call jpeg_abort_decompress(&cinfo)
	jpeg_finish_decompress(&cinfo);

	// At this point, optionally go back and either load a new jpg into
	// the jpg_buffer, or define a new jpeg_mem_src, and then start 
	// another decompress operation.
	
	// Once you're really really done, destroy the object to free everything
	jpeg_destroy_decompress(&cinfo);
	// And free the input buffer
	free(jpg_buffer);

    return NULL;
}
*/
void image::makeNull()
{
	buffer = NULL;
}

void image::cleanup()
{
	if(buffer != NULL) delete[] buffer;
}