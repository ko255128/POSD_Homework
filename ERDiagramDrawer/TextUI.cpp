/*
 * TextUI.cpp
 *
 *  Created on: 2013/9/13
 *      Author: User
 */

#include "TextUI.h"
#include "Compoment.h"
#include "Connection.h"
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <limits>

using namespace std;

TextUI::TextUI(PresentationModel* presentationModel)
{
	this->_presentationModel = presentationModel;
}

TextUI::~TextUI()
{
}

void TextUI::displayMenu()
{
	cout << _presentationModel->getShowMessage();
}

bool TextUI::processCommand()
{
	string input;
	getline(cin, input);
	return _presentationModel->processInput(input);
}

