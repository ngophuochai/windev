#include "stdafx.h"
#include "Shapes.h"

short CLine::GetType() {
	return 0;
}

void CLine::Draw(Graphics *graphics) {
	graphics->DrawLine(pen, A.X, A.Y, B.X, B.Y);
}

CShape* CLine::Create() { return new CLine; }

void CLine::SetData(Point A, Point B, Pen* pen) {
	this->A.X = A.X;
	this->A.Y = A.Y;
	this->B.X = B.X;
	this->B.Y = B.Y;
	this->pen = pen;
}

short CRectangle::GetType() {
	return 1;
}
void CRectangle::Draw(Graphics *graphics) {
	int width = abs(A.X - B.X);
	int height = abs(A.Y - B.Y);

	if (B.X > A.X && B.Y > A.Y)
		graphics->DrawRectangle(pen, A.X, A.Y, width, height);
	if (B.X > A.X && B.Y < A.Y)
		graphics->DrawRectangle(pen, A.X, B.Y, width, height);
	if (B.X < A.X && B.Y > A.Y)
		graphics->DrawRectangle(pen, B.X, A.Y, width, height);
	if (B.X < A.X && B.Y < A.Y)
		graphics->DrawRectangle(pen, B.X, B.Y, width, height);
}

CShape* CRectangle::Create() { return new CRectangle; }

void CRectangle::SetData(Point A, Point B, Pen* pen) {
	this->A.X = A.X;
	this->A.Y = A.Y;
	this->B.X = B.X;
	this->B.Y = B.Y;
	this->pen = pen;
}

short CSquare::GetType() {
	return 2;
}
void CSquare::Draw(Graphics *graphics) {
	int width = abs(A.X - B.X);
	int height = abs(A.Y - B.Y);

	if (B.X > A.X && B.Y > A.Y) {
		if (width < height)
			graphics->DrawRectangle(pen, A.X, A.Y, width, width);
		else
			graphics->DrawRectangle(pen, A.X, A.Y, height, height);
	}

	if (B.X > A.X && B.Y < A.Y) {
		if (width < height)
			graphics->DrawRectangle(pen, A.X, A.Y - width, width, width);
		else
			graphics->DrawRectangle(pen, A.X, B.Y, height, height);
	}

	if (B.X < A.X && B.Y > A.Y) {
		if (width < height)
			graphics->DrawRectangle(pen, A.X - width, A.Y, width, width);
		else
			graphics->DrawRectangle(pen, A.X - height, A.Y, height, height);
	}

	if (B.X < A.X && B.Y < A.Y) {
		if (width < height)
			graphics->DrawRectangle(pen, A.X - width, A.Y - width, width, width);
		else
			graphics->DrawRectangle(pen, A.X - height, A.Y - height, height, height);
	}
}

CShape* CSquare::Create() { return new CSquare; }

void CSquare::SetData(Point A, Point B, Pen* pen) {
	this->A.X = A.X;
	this->A.Y = A.Y;
	this->B.X = B.X;
	this->B.Y = B.Y;
	this->pen = pen;
}

short CEllipse::GetType() {
	return 3;
}
void CEllipse::Draw(Graphics *graphics) {
	int width = abs(A.X - B.X);
	int height = abs(A.Y - B.Y);

	if (B.X > A.X && B.Y > A.Y)
		graphics->DrawEllipse(pen, A.X, A.Y, width, height);
	if (B.X > A.X && B.Y < A.Y)
		graphics->DrawEllipse(pen, A.X, B.Y, width, height);
	if (B.X < A.X && B.Y > A.Y)
		graphics->DrawEllipse(pen, B.X, A.Y, width, height);
	if (B.X < A.X && B.Y < A.Y)
		graphics->DrawEllipse(pen, B.X, B.Y, width, height);
}

CShape* CEllipse::Create() { return new CEllipse; }

void CEllipse::SetData(Point A, Point B, Pen* pen) {
	this->A.X = A.X;
	this->A.Y = A.Y;
	this->B.X = B.X;
	this->B.Y = B.Y;
	this->pen = pen;
}

short CCircle::GetType() {
	return 4;
}
void CCircle::Draw(Graphics *graphics) {
	int width = abs(A.X - B.X);
	int height = abs(A.Y - B.Y);

	if (B.X > A.X && B.Y > A.Y) {
		if (width < height)
			graphics->DrawEllipse(pen, A.X, A.Y, width, width);
		else
			graphics->DrawEllipse(pen, A.X, A.Y, height, height);
	}

	if (B.X > A.X && B.Y < A.Y) {
		if (width < height)
			graphics->DrawEllipse(pen, A.X, A.Y - width, width, width);
		else
			graphics->DrawEllipse(pen, A.X, B.Y, height, height);
	}

	if (B.X < A.X && B.Y > A.Y) {
		if (width < height)
			graphics->DrawEllipse(pen, A.X - width, A.Y, width, width);
		else
			graphics->DrawEllipse(pen, A.X - height, A.Y, height, height);
	}

	if (B.X < A.X && B.Y < A.Y) {
		if (width < height)
			graphics->DrawEllipse(pen, A.X - width, A.Y - width, width, width);
		else
			graphics->DrawEllipse(pen, A.X - height, A.Y - height, height, height);
	}
}

CShape* CCircle::Create() { return new CCircle; }

void CCircle::SetData(Point A, Point B, Pen* pen) {
	this->A.X = A.X;
	this->A.Y = A.Y;
	this->B.X = B.X;
	this->B.Y = B.Y;
	this->pen = pen;
}