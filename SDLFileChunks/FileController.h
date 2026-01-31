#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include "StandartIncludes.h"

class FileController : public Singleton<FileController>
{
public:
	//Constructors and Destructors
	FileController();
	virtual ~FileController();

	//Accessors 
	bool GetFileReadDone() { return !m_thread.joinable(); }
	bool GetFileReadSuccess() { return m_readSuccess; }

	//Public Methods
	string GetCurDirectory();
	int GetFileSize(string _filePath);
	bool ReadFile(string _filePath, unsigned char* _buffer, unsigned int _bufferSize);
	void ReadFileAsync(string _filePath, unsigned char* _buffer, unsigned int _bufferSize);
private:
	FILE* m_handle;
	bool m_readSuccess;
	thread m_thread;
};

#endif // FILECONTROLLER_H