/*
 * Command.h
 *
 *  Created on: 2013/10/5
 *      Author: User
 */

#ifndef COMMAND_H_
#define COMMAND_H_

class Command
{
public:
	Command();
	virtual ~Command();
	virtual void execute() =0;
	virtual void unexecute() =0;

};

#endif /* COMMAND_H_ */
