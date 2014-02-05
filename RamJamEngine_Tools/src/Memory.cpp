#include "Memory.h"

//////////////////////////////////////////////////////////////////////////
typedef struct {
	DWORD	address;
	DWORD	size;
	char	file[128];
	DWORD	line;
} ALLOC_INFO;

struct free_delete
{
	void operator()(void* x) { free(x); }
};

typedef std::list<std::shared_ptr<ALLOC_INFO>> AllocList;

AllocList	allocList;

//////////////////////////////////////////////////////////////////////////
void AddTrack(DWORD addr, DWORD asize, const char *fname, DWORD lnum)
{
	std::shared_ptr<ALLOC_INFO> pInfo((ALLOC_INFO*) malloc(sizeof(ALLOC_INFO)), free_delete());

	pInfo->address = addr;
	strncpy_s(pInfo->file, fname, 127);
	pInfo->line = lnum;
	pInfo->size = asize;
	allocList.push_front(pInfo);
};

//////////////////////////////////////////////////////////////////////////
void RemoveTrack(DWORD addr)
{
	AllocList::iterator i;

	if(allocList.size() == 0)
		return;
	for(i = allocList.begin(); i != allocList.end(); i++)
	{
		if((*i)->address == addr)
		{
			allocList.remove((*i));
			break;
		}
	}
};

//////////////////////////////////////////////////////////////////////////
void MemoryReport()
{
	AllocList::iterator i;
	DWORD totalSize = 0;

	if(allocList.empty())
	{
		std::cout << " No Memory Leaks !" << std::endl;
		return;
	}

	HANDLE hstdout = GetStdHandle( STD_OUTPUT_HANDLE );
	int White     = 0x07;
	int LightRed  = 0x0C;
	int LightBlue = 0x09;

	std::cout << "----------------------------------------------------------" << std::endl;
	SetConsoleTextAttribute(hstdout, LightRed);
	std::cout << "  Detected Memory Leaks !!" << std::endl;
	SetConsoleTextAttribute(hstdout, White);
	std::cout << "----------------------------------------------------------" << std::endl;
	for(i = allocList.begin(); i != allocList.end(); i++) {
		std::cout << "FILE :\t\t" <<(*i)->file << "\nLINE :\t\t" << (*i)->line << "\nADDRESS :\t" << (*i)->address << "\n" << (*i)->size << " bytes unfreed" << std::endl;
		std::cout << "-----------" << std::endl;
		totalSize += (*i)->size;
	}
	std::cout << "----------------------------------------------------------" << std::endl;
	SetConsoleTextAttribute(hstdout, LightBlue);
	std::cout << "Total Unfreed: " << totalSize << " bytes" << std::endl;
	SetConsoleTextAttribute(hstdout, White);
	RJE_MESSAGE_BOX(NULL, L"Memory Leaks Found !\nCheck the console for details", L"Memory Manager", MB_ICONWARNING | MB_OK);
	getchar();
};
