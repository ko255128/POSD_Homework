/*
 * Compoment.h
 *
 *  Created on: 2013/9/16
 *      Author: User
 */

#ifndef COMPOMENT_H_
#define COMPOMENT_H_

#include <string>
#include <vector>
#include <sstream>
#include <qwidget.h>

using namespace std;
class ComponentVisitor;
class Component
{
public:
	enum COMPOMENT_TYPE
	{
		ENTITY, ATTRIBUTE, RELATIONSHIP, CONNECTION
	};

	Component();
	virtual ~Component();
	virtual string getTypeSymbol() = 0;
	void connectTo(Component* target);
	virtual bool canConnectTo(Component* target) = 0;
	virtual void disConnectTo(Component* target) = 0;
	virtual void draw(QWidget* widget)=0;
	virtual bool isCollision(int x, int y)=0;
	virtual void accept(ComponentVisitor* visitor)=0;
	virtual Component* clone()=0;

	unsigned int getId() const
	{
		return _id;
	}

	void setId(unsigned int id)
	{
		_id = id;
	}

	const string& getName() const
	{
		return _name;
	}

	void setName(const string& name)
	{
		_name = name;
	}

	COMPOMENT_TYPE getType() const
	{
		return _type;
	}

	bool checkAlreadyConnect(Component* target);

	const vector<Component*>& getConnectinons() const
	{
		return _connectinons;
	}

	int getHeight() const
	{
		return _height;
	}

	void setHeight(int height)
	{
		_height = height;
	}

	int getPosX() const
	{
		return _posX;
	}

	void setPosX(int posX)
	{
		_posX = posX;
	}

	int getPosY() const
	{
		return _posY;
	}

	void setPosY(int posY)
	{
		_posY = posY;
	}

	int getWidth() const
	{
		return _width;
	}

	void setWidth(int width)
	{
		_width = width;
	}

	bool isSelected() const
	{
		return _selected;
	}

	void setSelected(bool selected)
	{
		_selected = selected;
	}

	void setIsPreview(bool isPreview)
	{
		_isPreview = isPreview;
	}

protected:
	unsigned int _id;
	COMPOMENT_TYPE _type;
	string _name;
	vector<Component*> _connectinons;
	bool _selected;
	bool _isPreview;
	int _posX;
	int _posY;
	int _width;
	int _height;
	static const int outBoundOffset = 2;
	static const int pastPosOffset = 50;

	void hookdisConnectTo(Component* target);

};

#endif /* COMPOMENT_H_ */
