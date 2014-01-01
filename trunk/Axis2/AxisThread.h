#pragma once



// CAxisThread

class CAxisThread : public CWinThread
{
	DECLARE_DYNCREATE(CAxisThread)

protected:
	CAxisThread();           // protected constructor used by dynamic creation
	virtual ~CAxisThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


