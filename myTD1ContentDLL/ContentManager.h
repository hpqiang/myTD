#pragma once

#include "Content.h"

class ContentManager
{
public:
	ContentManager() {}
	~ContentManager() {}
	// to do : add CRUD for content handling
private:
	list<IContent *>	m_Contents;
	mutex				m_Mutex;
	condition_variable	m_CondVar;
};