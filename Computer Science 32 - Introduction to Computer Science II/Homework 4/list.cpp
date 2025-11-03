void listAll(string path, const MenuItem* m)
{
    if (m == nullptr)
    {
        return;
    }
    string newPath = "";

    if (path.empty())
    {
        newPath = m->name();
    }
    else
    {
        newPath = path + "/" + m->name();
    }

    if (!m->name().empty())
    {
        cout << newPath << endl;
    }

    if (m->menuItems() != nullptr)
    {
        vector<MenuItem*> mi = *(m->menuItems());
        vector <MenuItem*>::iterator it = mi.begin();
        for (int i = 0; i < mi.size(); i++, it++)
        {
            listAll(newPath, *it);
        }
    }
}

