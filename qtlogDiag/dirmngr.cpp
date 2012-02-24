#include <QtCore>
#include "dirmngr.h"
#ifdef Q_WS_WIN
//#define DIR_SEP '\\'
//weil Qt alles mit Slash macht
#include <windows.h>
#define DIR_SEP '/'
#else
#define DIR_SEP '/'
#endif

void StartProcess(QString cmd)
{
#ifdef Q_WS_WIN
    cmd.remove('&');
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    
    if( !CreateProcess( NULL, 
        (char*)(const char*) cmd.toAscii(), 
        NULL,             
        NULL,             
        FALSE,            
        0,                
        NULL,             
        NULL,             
        &si,              
        &pi )             
    )
        ;//ErrorExit( "CreateProcess failed." );
    
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
#else
    
    int x;
    x = system(cmd.toAscii());
#endif
}

DirMngr::DirMngr()
{
}

void DirMngr::SetProgramDir(QString dir)
{
	QFileInfo applInfo(dir);
   programDir=applInfo.absolutePath();
	if(programDir.right(1)!="\\" && programDir.right(1)!="/")programDir+=DIR_SEP;
	configDir=programDir;
	data1Dir=programDir;
	data2Dir=programDir;
	data3Dir=programDir;
   SetDir(QDir::tempPath(),DIR_TEMP);
   SetDir(QDir::homePath(),DIR_HOME);
#ifdef Q_WS_WIN
#else
	SetDir(getenv("HOME"),DIR_HOME);
#endif
}

void DirMngr::SetDir(QString dir,int where)
{
	QString tmp=dir;
	if(tmp.right(1)!="\\" && tmp.right(1)!="/")tmp+=DIR_SEP;
	switch(where){
		case DIR_PROGRAM:programDir=tmp;break;
		case DIR_CONFIG:configDir=tmp;break;
		case DIR_DATA1:data1Dir=tmp;break;
		case DIR_DATA2:data2Dir=tmp;break;
		case DIR_DATA3:data3Dir=tmp;break;
		case DIR_TEMP:tempDir=tmp;break;
	}
}

QString DirMngr::GetDir(int where)
{
	switch(where){
		case DIR_PROGRAM:return programDir;
		case DIR_CONFIG:return configDir;
		case DIR_DATA1:return data1Dir;
		case DIR_DATA2:return data2Dir;
		case DIR_DATA3:return data3Dir;
		case DIR_TEMP:return tempDir;
	}//switch
	return QString("");
}

QString DirMngr::CreatePath(QString fileName,int where)
{
	return GetDir(where)+fileName;
}
