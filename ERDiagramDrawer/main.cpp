/*
 * main.cpp
 *
 *  Created on: 2013/9/13
 *      Author: User
 */
#include <cstdlib>
#include <iostream>
#include "TextUI.h"
#include "PresentationModel.h"

using namespace std;

int main(int argc, char *argv[])
{
	TextUI* textUI;
	static ERModel* eRmodel = new ERModel();
	static PresentationModel* presentationModel = new PresentationModel(eRmodel);
	textUI = new TextUI(presentationModel);
	bool continueFlag = false;
	do
	{
		textUI->displayMenu();
		continueFlag = textUI->processCommand();
	} while (continueFlag);
	delete textUI;
	delete presentationModel;
	delete eRmodel;
	return EXIT_SUCCESS;
}

