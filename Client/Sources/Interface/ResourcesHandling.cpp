#include "ResourcesHandling.hpp"

#include <cstdio>
#include <olectl.h>

namespace ResourcesHandling
{
	HBITMAP LoadImage(const char* FileName)
	{
		// Use IPicture stuff to use JPG / GIF files
		IPicture* p;
		IStream* s;
		HGLOBAL hG;
		void* pp;
		FILE* fp;


		// Read file in memory
		fopen_s(&fp, FileName, "rb");
		//fp = fopen(FileName, "rb");
		if (!fp)
			return NULL;

		fseek(fp, 0, SEEK_END);
		int fs = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		hG = GlobalAlloc(GPTR, fs);
		if (!hG)
		{
			fclose(fp);
			return NULL;
		}
		pp = (void*)hG;
		fread(pp, 1, fs, fp);
		fclose(fp);

		// Create an IStream so IPicture can 
		CreateStreamOnHGlobal(hG, false, &s);
		if (!s)
		{
			GlobalFree(hG);
			return NULL;
		}

		OleLoadPicture(s, 0, false, IID_IPicture, (void**)&p);

		if (!p)
		{
			s->Release();
			GlobalFree(hG);
			return NULL;
		}
		s->Release();
		GlobalFree(hG);

		HBITMAP hB = 0;
		p->get_Handle((unsigned int*)&hB);

		// Copy the image. Necessary, because upon p's release,
		// the handle is destroyed.
		HBITMAP hBB = (HBITMAP)CopyImage(hB, IMAGE_BITMAP, 0, 0,
			LR_COPYRETURNORG);

		p->Release();
		return hBB;
	}
}