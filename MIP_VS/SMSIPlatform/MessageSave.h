#pragma once

class MessageSave
{
public:
	CString time;
	CString phonum;
	CString text;
public:
	MessageSave();
	MessageSave(CString time, CString phonum, CString text);
	~MessageSave(void);
};
