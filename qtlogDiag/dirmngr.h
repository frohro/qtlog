#ifndef DIRMNGR_H_
#define DIRMNGR_H_
#define DIR_PROGRAM   1
#define DIR_CONFIG    2
#define DIR_DATA1     3
#define DIR_DATA2     4
#define DIR_DATA3     5
#define DIR_TEMP      6
#define DIR_HOME      7
#include <QString>
void StartProcess(QString s);
class DirMngr{
	protected:
	QString programDir,
	        configDir,
	        data1Dir,
	        data2Dir,
	        data3Dir,
	        tempDir,
	        homeDir
	        ;
	public:
	DirMngr();
	void SetProgramDir(QString dir);
	void SetDir(QString dir,int where);
	QString GetDir(int where);
	QString CreatePath(QString fileName,int where);
};
#endif /*DIRMNGR_H_*/
