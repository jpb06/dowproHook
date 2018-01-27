#include "Zipper.hpp"

#include <zlib.h>
#include <algorithm>
#include <time.h>

// #include <ioapi.h>
// #include <time.h>

namespace ziputils
{
	const unsigned int BUFSIZE = 2048;


	// Default constructor
	zipper::zipper() :
		zipFile(0),
		entryOpen(false)
	{
	}

	// Default destructor
	zipper::~zipper(void)
	{
		close();
	}

	// Create a new zip file.
	// param:
	//		filename	path and the filename of the zip file to open
	//		append		set true to append the zip file
	// return:
	//		true if open, false otherwise
	bool zipper::open(const char* filename, bool append)
	{
		close();
		zipFile = zipOpen64(filename, append ? APPEND_STATUS_ADDINZIP : 0);

		return isOpen();
	}

	// Close the zip file
	void zipper::close()
	{
		if (zipFile)
		{
			closeEntry();
			zipClose(zipFile, "");
			zipFile = 0;
		}
	}

	// Check if a zipfile is open.
	// return:
	//		true if open, false otherwise
	bool zipper::isOpen()
	{
		return zipFile != 0;
	}

	// Create a zip entry; either file or folder. Folder has to 
	// end with a slash or backslash.
	// return:
	//		true if open, false otherwise
	bool zipper::addEntry(const char* filename)
	{
		if (isOpen())
		{
			closeEntry();

			while (filename[0] == '\\' || filename[0] == '/')
			{
				filename++;
			}

			//?? we dont need the stinking time
			zip_fileinfo zi = { 0 };
			getTime(zi.tmz_date);

			int err = zipOpenNewFileInZip(zipFile, filename, &zi,
				NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION);

			entryOpen = (err == ZIP_OK);
		}
		return entryOpen;
	}

	// Close the currently open zip entry.
	void zipper::closeEntry()
	{
		if (entryOpen)
		{
			zipCloseFileInZip(zipFile);
			entryOpen = false;
		}
	}

	// Check if there is a currently open file zip entry.
	// return:
	//		true if open, false otherwise
	bool zipper::isOpenEntry()
	{
		return entryOpen;
	}

	// Stream operator for dumping data from an input stream to the 
	// currently open zip entry.
	zipper& zipper::operator<<(std::istream& is)
	{
		int err = ZIP_OK;
		char buf[BUFSIZE];
		unsigned long nRead = 0;

		if (isOpenEntry())
		{
			while (err == ZIP_OK && is.good())
			{
				is.read(buf, BUFSIZE);
				unsigned int nRead = (unsigned int)is.gcount();

				if (nRead)
				{
					err = zipWriteInFileInZip(zipFile, buf, nRead);
				}
				else
				{
					break;
				}
			}
		}
		return *this;
	}

	// Fill the zip time structure
	// param:
	//		tmZip	time structure to be filled
	void zipper::getTime(tm_zip& tmZip)
	{
		struct tm timeinfo;
		time_t rawtime;
		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);
		//auto timeinfo = localtime(&rawtime);
		tmZip.tm_sec = timeinfo.tm_sec;
		tmZip.tm_min = timeinfo.tm_min;
		tmZip.tm_hour = timeinfo.tm_hour;
		tmZip.tm_mday = timeinfo.tm_mday;
		tmZip.tm_mon = timeinfo.tm_mon;
		tmZip.tm_year = timeinfo.tm_year;
	}
};
