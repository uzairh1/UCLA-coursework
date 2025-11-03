void removeBad(list<Restaurant*>& li)
{
	list<Restaurant*>::iterator it = li.begin();
	while (it != li.end())
	{
		if ((*it)->stars() <= 2)
		{
			(*it)->~Restaurant();
			it = li.erase(it);
		}
		else
		{
			it++;
		}
	}
}