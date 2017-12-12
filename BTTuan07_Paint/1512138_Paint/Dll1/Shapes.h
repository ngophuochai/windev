#pragma once
#include "stdafx.h"

class CShape {
public:
	Pen* pen;

	virtual short GetType() = 0;
	virtual void Draw(Graphics *graphics) = 0;
	virtual CShape* Create() = 0;
	virtual void SetData(Point A, Point B, Pen* pen) = 0;
};

class CLine : public CShape {
public:
	Point A, B;
	Pen* pen;
public:
	short GetType();
	void Draw(Graphics *graphics);
	CShape* Create();
	void SetData(Point A, Point B, Pen* pen);
};

class CRectangle : public CShape {
public:
	Point A, B;
	Pen* pen;
public:
	short GetType();
	void Draw(Graphics *graphics);
	CShape* Create();
	void SetData(Point A, Point B, Pen* pen);
};

class CSquare : public CShape {
public:
	Point A, B;
	Pen* pen;
public:
	short GetType();
	void Draw(Graphics *graphics);
	CShape* Create();
	void SetData(Point A, Point B, Pen* pen);
};

class CEllipse : public CShape {
public:
	Point A, B;
	Pen* pen;
public:
	short GetType();
	void Draw(Graphics *graphics);
	CShape* Create();
	void SetData(Point A, Point B, Pen* pen);
};

class CCircle : public CShape {
public:
	Point A, B;
	Pen* pen;
public:
	short GetType();
	void Draw(Graphics *graphics);
	CShape* Create();
	void SetData(Point A, Point B, Pen* pen);
};