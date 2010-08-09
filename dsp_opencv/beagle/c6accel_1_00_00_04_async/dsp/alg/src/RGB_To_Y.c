

void IMG_RGB_To_Y (
		unsigned char * restrict src,
		unsigned char * restrict dst,		
		unsigned int count
) {

	unsigned short R, G, B;
	
	unsigned short RC = 0x4c;
	unsigned short GC = 0x95;
	unsigned short BC = 0x1D;

	unsigned int acc; 

	unsigned int i;
	


	for (i = 0; i < count; i ++) {
             /*
		R = *src++;
		G = *src++;
		B = *src++;
	     */
		B = *src++;
		G = *src++;
		R = *src++;
	      
		R = R << 8;
		G = G << 8;
		B = B << 8;

		acc = R * RC;
		acc += G * GC;
		acc += B * BC;

		acc = acc >> 16;

		*dst++ = acc;
	}
}


