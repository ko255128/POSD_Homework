/*
 * FileControl.cpp
 *
 *  Created on: 2013/10/7
 *      Author: User
 */

#include "FileControl.h"
#include "SaveComponentVisitor.h"
#include "saveXmlComponentVisitor.h"

#include<iostream>
#include<fstream>
#include <list>
#include <vector>
#include <algorithm>
#include <sys/stat.h>
#include <string>
#include "Entity.h"
#include "Compoment.h"
#include "Connection.h"
#include "stringResource.h"

using namespace std;


FileControl::FileControl(ERModel* model)
{
	this->_Model = model;
}

FileControl::~FileControl()
{
	// TODO Auto-generated destructor stub
}

//寫檔
bool FileControl::writeFile(string FilePath,bool isWithPos)
{
	ofstream erdFileOutputFilestream(FilePath.c_str(), ios::out | ios::trunc);
	if (erdFileOutputFilestream.is_open())
	{
		SaveComponentVisitor visitor(_Model,isWithPos);
		vector<Component*> components = _Model->getComponents();
		for(unsigned int index = 0 ; index < components.size();index++)
			components.at(index)->accept(&visitor);
		erdFileOutputFilestream << visitor.getComponentData();
		erdFileOutputFilestream.close();
		if(isWithPos)
			writePosFile(FilePath, visitor.getPosData());
		else
			remove(extractPosFilePath(FilePath).c_str());
		return true;
	}
	return false;
}
//讀檔
bool FileControl::loadFile(string FilePath)
{
	ifstream inputFileStream(FilePath.c_str());
	vector<string> loadcomponent;
	vector<string> loadConnect;
	vector<string> loadPrimaryKey;
	if (inputFileStream.is_open())
	{
		if (loadABlock(loadcomponent, inputFileStream))
			if (loadABlock(loadConnect, inputFileStream))
				loadABlock(loadPrimaryKey, inputFileStream);
		inputFileStream.close();
		return _Model->loadComponentsByString(loadcomponent,loadConnect,loadPrimaryKey);
	}
	return false;
}
//讀入一個區塊(以空行為界)
bool FileControl::loadABlock(vector<string>& loadComponents, ifstream& inputFileStream)
{
	string loadString;
	do
	{
		if (inputFileStream.eof())
			return false;
		getline(inputFileStream, loadString);
		if (!loadString.empty())
			loadComponents.push_back(loadString);
	} while (!loadString.empty());
	return true;
}

std::string FileControl::extractPosFilePath( string filePath )
{
	unsigned int pos = filePath.find(BACK_SLASH);
	while(pos != filePath.npos)
	{
		filePath.replace(pos,1,SLASH);
		pos = filePath.find(BACK_SLASH,pos+1);
	}
	string fileName = filePath;
	pos = filePath.find_last_of(SLASH);
	if(pos != filePath.npos)
	{
		fileName = filePath.substr(pos+1, filePath.size());
		filePath = filePath.substr(0,pos+1);
	}
	else
		filePath.clear();
	pos = fileName.find_last_of(DOT);
	if(pos!=fileName.npos)
		return filePath + fileName.substr(0,pos) + POS_FILE_FILE_NAME_EXTENSION;
	else
		return filePath + fileName + POS_FILE_FILE_NAME_EXTENSION;
}

void FileControl::writePosFile(string FilePath, string posData )
{
	ofstream posFileOutputStream(extractPosFilePath(FilePath),ios::out | ios::trunc);
	if(posFileOutputStream.is_open())
		posFileOutputStream  <<  posData;
	posFileOutputStream.close();
}

bool FileControl::writeXmlFile( string FilePath )
{
	ofstream posFileOutputStream(FilePath,ios::out | ios::trunc);
	if(posFileOutputStream.is_open())
	{
		saveXmlComponentVisitor visitor;
		vector<Component*> components = _Model->getComponents();
		for(unsigned int index = 0 ; index < components.size();index++)
			components.at(index)->accept(&visitor);
		posFileOutputStream << visitor.getData();
		posFileOutputStream.close();
		return true;
	}
	return false;
}

bool FileControl::loadPosFile( string FilePath )
{
	ifstream posInputFileStream(extractPosFilePath(FilePath).c_str());
	if(posInputFileStream.is_open())
	{
		unsigned int counter = 0;
		vector<Component*> components = _Model->getComponents();
		do
		{
			if(!extractPos(posInputFileStream, components, counter))
				return false;
		}while(!posInputFileStream.eof());
		return true;
	}
	return false;
}

bool FileControl::extractPos(ifstream& posInputFileStream, vector<Component*> &components, unsigned int &counter )
{
	string loadString;
	unsigned int posX,posY;
	getline(posInputFileStream, loadString);
	unsigned int pos = loadString.find(BLANK);
	if(pos != loadString.npos)
	{
		if(_Model->transStringToInt(loadString.substr(0,pos),posX) && _Model->transStringToInt(loadString.substr(pos+1,loadString.size()),posY))
		{
			while(components.at(counter)->getType() == Component::CONNECTION)
				counter++;
			if(counter < components.size())
			{
				components.at(counter)->setPosX(posX);
				components.at(counter)->setPosY(posY);
				counter++;
				return true;
			}
		}
	}
	return false;
}

