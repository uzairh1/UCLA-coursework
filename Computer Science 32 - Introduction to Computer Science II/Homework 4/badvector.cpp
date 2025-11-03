void removeBad(vector<Restaurant*>& v)
{
	vector<Restaurant*>::iterator it = v.begin();
	while (it != v.end())
	{
		if ((*it)->stars() <= 2)
		{
			(*it)->~Restaurant();
			it = v.erase(it);
		}
		else
		{
			it++;
		}
	}
}
