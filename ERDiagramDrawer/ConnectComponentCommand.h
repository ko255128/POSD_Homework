/*
 * ConnectComponentCommand.h
 *
 *  Created on: 2013/10/5
 *      Author: User
 */

#ifndef CONNECTCOMPONENTCOMMAND_H_
#define CONNECTCOMPONENTCOMMAND_H_

#include "Command.h"
#include "Connection.h"
#include "ERModel.h"

class ConnectComponentCommand: public Command
{
public:
	ConnectComponentCommand(ERModel* model,unsigned int firstNodeID,unsigned int secondNodeID,Connection::REALTION_CARDINALITY realtionCardinality);
	~ConnectComponentCommand();
	void execute();
	void unexecute();
private:
	ERModel* _Model;
	unsigned int _FirstNodeID;
	unsigned int _SecondNodeID;
	Component* _UnDoAddConnectNode;
	Connection::REALTION_CARDINALITY _RelationCardinality;
};

#endif /* CONNECTCOMPONENTCOMMAND_H_ */
