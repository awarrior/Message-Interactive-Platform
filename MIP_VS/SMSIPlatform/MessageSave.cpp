#include "StdAfx.h"
#include "MessageSave.h"

MessageSave::MessageSave()
{
	time = "";
	phonum = "";
	text = "";
}

MessageSave::MessageSave(CString time, CString phonum, CString text)
{
	this->time = time;
	this->phonum = phonum;
	this->text = text;
}

MessageSave::~MessageSave(void)
{
}
