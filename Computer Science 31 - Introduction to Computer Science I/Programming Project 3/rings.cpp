#include <string>
#include <iostream>
#include <cassert>
using namespace std;

bool isValidRingString(string ringstring);
int totalExerciseMinutes(string ringstring);
int totalStandMinutes(string ringstring);
int totalMoveMinutes(string ringstring);
bool metMoveGoal(string ringstring, int goal);
bool metStandGoal(string ringstring, int goal);
bool metExerciseGoal(string ringstring, int goal);

int main()
{
	return 0;
}

bool isValidRingString(string ringstring)
{
	int eMinutes = 0;
	int mMinutes = 0;
	int sMinutes = 0;
	int i = 0;

	if (ringstring.empty())
	{
		return false;
	}
	if ((ringstring.at(0) != 'E') && (ringstring.at(0) != 'S') && (ringstring.at(0) != 'M'))
	{
		return false;
	}
	if ((ringstring.at(i + 1) != 'E') && (ringstring.at(i + 1) != 'M') && (ringstring.at(i + 1) != 'S') && (ringstring.at(i + 1) != '1') && (ringstring.at(i + 1) != '2') && (ringstring.at(i + 1) != '3') && (ringstring.at(i + 1) != '4') && (ringstring.at(i + 1) != '5') && (ringstring.at(i + 1) != '6') && (ringstring.at(i + 1) != '7') && (ringstring.at(i + 1) != '8') && (ringstring.at(i + 1) != '9'))
	{
		return false;
	}
	i = 0;
	for (; i <= ringstring.size() - 1; i++)
	{
		if ((ringstring.at(i) == 'E') || (ringstring.at(i) == 'M') || (ringstring.at(i) ==
			'S'))
		{
			char c = ringstring.at(i);
			if ((!isdigit(ringstring.at(i + 1))))
			{
				return false;
			}
			int temp = 0;
			for (int j = i + 1; j <= ringstring.size() - 1; j++)
			{
				if ((ringstring.at(j) == 'E') || (ringstring.at(j) == 'M') || (ringstring.at(j) == 'S'))
				{
					if (c == 'E')
					{
						eMinutes += temp;
					}
					else if (c == 'M')
					{
						mMinutes += temp;
					}
					else
					{
						sMinutes += temp;
					}
					i = j - 1;
					break;
				}
				temp = temp * 10 + (ringstring.at(j) - '0');

				if (j + 1 == ringstring.size())
				{
					if (c == 'E')
					{
						eMinutes += temp;
					}
					else if (c == 'M')
					{
						mMinutes += temp;
					}
					else
					{
						sMinutes += temp;
					}
					i = j;
					break;
				}

			}

		}

	}
	if ((sMinutes <= 999) && (mMinutes <= 999) && (eMinutes <= 999))
	{
		return true;
	}
	else
	{
		return false;
	}
}

int totalExerciseMinutes(string ringstring)
{
	int eMinutes = 0;
	int mMinutes = 0;
	int sMinutes = 0;
	if (!isValidRingString(ringstring))
	{
		return -1;
	}
	else
	{
		int i = 0;
		for (; i <= ringstring.size() - 1; i++)
		{
			if ((ringstring.at(i) == 'E') || (ringstring.at(i) == 'M') || (ringstring.at(i) ==
				'S'))
			{
				char c = ringstring.at(i);
				if (!isdigit(ringstring.at(i + 1)))
				{
					return false;
				}
				int temp = 0;
				for (int j = i + 1; j <= ringstring.size() - 1; j++)
				{
					if ((ringstring.at(j) == 'E') || (ringstring.at(j) == 'M') || (ringstring.at(j) == 'S'))
					{
						if (c == 'E')
						{
							eMinutes += temp;
						}
						else if (c == 'M')
						{
							mMinutes += temp;
						}
						else
						{
							sMinutes += temp;
						}
						i = j - 1;
						break;
					}
					temp = temp * 10 + (ringstring.at(j) - '0');

					if (j + 1 == ringstring.size())
					{
						if (c == 'E')
						{
							eMinutes += temp;
						}
						else if (c == 'M')
						{
							mMinutes += temp;
						}
						else
						{
							sMinutes += temp;
						}
						i = j;
						break;
					}

				}

			}

		}
		return eMinutes;
	}
}

int totalStandMinutes(string ringstring)
{
	{
		int eMinutes = 0;
		int mMinutes = 0;
		int sMinutes = 0;
		if (!isValidRingString(ringstring))
		{
			return -1;
		}
		else
		{
			int i = 0;
			for (; i <= ringstring.size() - 1; i++)
			{
				if ((ringstring.at(i) == 'E') || (ringstring.at(i) == 'M') || (ringstring.at(i) ==
					'S'))
				{
					char c = ringstring.at(i);
					if (!isdigit(ringstring.at(i + 1)))
					{
						return false;
					}
					int temp = 0;
					for (int j = i + 1; j <= ringstring.size() - 1; j++)
					{
						if ((ringstring.at(j) == 'E') || (ringstring.at(j) == 'M') || (ringstring.at(j) == 'S'))
						{
							if (c == 'E')
							{
								eMinutes += temp;
							}
							else if (c == 'M')
							{
								mMinutes += temp;
							}
							else
							{
								sMinutes += temp;
							}
							i = j - 1;
							break;
						}
						temp = temp * 10 + (ringstring.at(j) - '0');

						if (j + 1 == ringstring.size())
						{
							if (c == 'E')
							{
								eMinutes += temp;
							}
							else if (c == 'M')
							{
								mMinutes += temp;
							}
							else
							{
								sMinutes += temp;
							}
							i = j;
							break;
						}

					}

				}

			}
			return sMinutes;
		}
	}
}

int totalMoveMinutes(string ringstring)
{
	{
		int eMinutes = 0;
		int mMinutes = 0;
		int sMinutes = 0;
		if (!isValidRingString(ringstring))
		{
			return -1;
		}
		else
		{
			int i = 0;
			for (; i <= ringstring.size() - 1; i++)
			{
				if ((ringstring.at(i) == 'E') || (ringstring.at(i) == 'M') || (ringstring.at(i) ==
					'S'))
				{
					char c = ringstring.at(i);
					if (!isdigit(ringstring.at(i + 1)))
					{
						return false;
					}
					int temp = 0;
					for (int j = i + 1; j <= ringstring.size() - 1; j++)
					{
						if ((ringstring.at(j) == 'E') || (ringstring.at(j) == 'M') || (ringstring.at(j) == 'S'))
						{
							if (c == 'E')
							{
								eMinutes += temp;
							}
							else if (c == 'M')
							{
								mMinutes += temp;
							}
							else
							{
								sMinutes += temp;
							}
							i = j - 1;
							break;
						}
						temp = temp * 10 + (ringstring.at(j) - '0');

						if (j + 1 == ringstring.size())
						{
							if (c == 'E')
							{
								eMinutes += temp;
							}
							else if (c == 'M')
							{
								mMinutes += temp;
							}
							else
							{
								sMinutes += temp;
							}
							i = j;
							break;
						}

					}

				}

			}
			return mMinutes;
		}
	}
}


bool metMoveGoal(string ringstring, int goal)
{
	if ((!isValidRingString(ringstring) || (goal < 0)))
	{
		return false;
	}
	else
	{
		return (totalMoveMinutes(ringstring) > goal);
	}
}

bool metStandGoal(string ringstring, int goal)
{
	if ((!isValidRingString(ringstring) || (goal < 0)))
	{
		return false;
	}
	else
	{
		return (totalStandMinutes(ringstring) > goal);
	}
}

bool metExerciseGoal(string ringstring, int goal)
{
	if ((!isValidRingString(ringstring) || (goal < 0)))
	{
		return false;
	}
	else
	{
		return (totalExerciseMinutes(ringstring) > goal);
	}
}
