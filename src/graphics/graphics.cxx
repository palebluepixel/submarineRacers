#include "graphics.hxx"


// void Texture::buildFrom(const char *path){
// 	w = 256;
// 	h = 256;
// 	c = 3;
// 	glGenTextures(1,&id);
// 	glBindTexture(GL_TEXTURE_2D, id);

// 	// debug: generate data array.

// 	data = new char[w*h*c];
// 	for(int i=0;i<w*h;i+=3){
// 		// tealish color.
// 		data[i+0] = 0;
// 		data[i+0] = 200;
// 		data[i+0] = 100;
// 	}

// 	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
// }