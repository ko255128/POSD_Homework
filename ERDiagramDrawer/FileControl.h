/*
 * FileControl.h
 *
 *  Created on: 2013/10/7
 *      Author: User
 */

#ifndef FILECONTROL_H_
#define FILECONTROL_H_
#include "ERModel.h"

using namespace std;
class FileControl
{
public:
	FileControl(ERModel* model);
	~FileControl();
	bool writeFile(string FilePath,bool isWithPos);
	bool writeXmlFile(string FilePath);
	bool loadFile(string FilePath);
	bool loadPosFile(string FilePath);

	bool extractPos( ifstream &posInputFileStream, vector<Component*> &components, unsigned int &counter );

private:
	ERModel* _Model;
	bool loadABlock(vector<string>& string,ifstream& inputFileStream);
	void writePosFile(string FilePath, string posData );
	string extractPosFilePath(string filePath);
};

#endif /* FILECONTROL_H_ */
