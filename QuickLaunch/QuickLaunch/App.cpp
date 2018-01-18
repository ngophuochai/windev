#include "App.h"
#include "stdafx.h"

CApp::CApp() {
	name[0] = path[0] = 0;
	seq = 0;
}

CApp::CApp(WCHAR* name, WCHAR* path, int seq) {
	StrCpy(this->name, name);
	StrCpy(this->path, path);
	this->seq = seq;
}

void CApp::SetCApp(WCHAR* name, WCHAR* path, int seq) {
	StrCpy(this->name, name);
	StrCpy(this->path, path);
	this->seq = seq;
}

WCHAR* CApp::GetName() {
	return name;
}

WCHAR* CApp::GetPath() {
	return path;
}

int CApp::GetSeq() {
	return seq;
}

void CApp::SetSeq(int seq) {
	this->seq = seq;
}

void CApp::Run() {
	this->seq++;
	ShellExecute(NULL, _T("open"), (LPCWSTR)path, NULL, NULL, SW_SHOWNORMAL);
}