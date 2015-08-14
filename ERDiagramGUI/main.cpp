#include "erdiagramdrawerui.h"
#include "PresentationModel.h"
#include "TextUI.h"

#include <QtGui>
#include <QApplication>
#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

void RunTextModel(PresentationModel* presentationModel, ERModel* eRmodel)
{
	TextUI* textUI = new TextUI(presentationModel);
	bool continueFlag = false;
	do
	{
		textUI->displayMenu();
		continueFlag = textUI->processCommand();
	} while (continueFlag);
	delete textUI;
	delete eRmodel;
	delete presentationModel;
}

int main(int argc, char *argv[])
{
	static ERModel* eRmodel = new ERModel();
	static PresentationModel* presentationModel = new PresentationModel(eRmodel);
	Q_INIT_RESOURCE(resource);
	if (argc == 1 + 1)
	{
		string paramater;
		paramater.assign(argv[1]);
		if (paramater.compare("TextModel") == 0)
		{
			RunTextModel(presentationModel, eRmodel);
			return EXIT_SUCCESS;
		}
		else
			cout << "GUI Model:No Parameter (ERDiagramDrawerUI.exe) Text Model:please enter Parameter TextModel (ERDiagramDrawerUI.exe TextModel)";
	}
	else if (argc == 1)
	{
		QApplication a(argc, argv);
		ERDiagramDrawerUI w(presentationModel);
		w.show();
		a.exec();
	}
	else
		cout << "GUI Model:Don't bring Parameter (ERDiagramDrawerUI.exe) Text Model:please enter Parameter TextModel (ERDiagramDrawerUI.exe TextModel)";
	delete eRmodel;
	delete presentationModel;
	return EXIT_SUCCESS;
}
