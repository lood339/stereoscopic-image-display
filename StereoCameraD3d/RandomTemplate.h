#ifndef RANDOM_TEMPLATE_H
#define RANDOM_TEMPLATE_H 1

#include <vector>
#include <stdlib.h>
#include <conio.h>
using std::vector;

//a template to generate random numbers
// 
template<class T> class RandomTemplate
{
public:
	RandomTemplate(const vector<T> &data)
	{
		assert(data.size() != 0);
		m_data = data;
		m_randomIndex.resize(m_data.size());
		for(unsigned int i = 0; i<m_randomIndex.size(); ++i)
		{
			m_randomIndex[i] = i;
		}

		//randomly swap
		for (unsigned int i = 0; i<m_randomIndex.size(); ++i)
		{
			int j = randRange(i, m_randomIndex.size()-1);
			std::swap(m_randomIndex[i], m_randomIndex[j]);
		}
		m_currentIndex = 0;
	}
	bool isEnd()
	{
		return m_currentIndex == m_randomIndex.size();
	}

	void reset()
	{
		//randomly swap
		for (int i = 0; i<m_randomIndex.size(); ++i)
		{
			int j = randRange(i, m_randomIndex.size()-1);
			std::swap(m_randomIndex[i], m_randomIndex[j]);
		}
		m_currentIndex = 0;
	}

	T getCurrentItemAndStepForward()  //get current item and step to next item
	{
		if(m_currentIndex >= 0 && m_currentIndex < m_randomIndex.size())
		{
			int cur = m_randomIndex[m_currentIndex];
			m_currentIndex += 1;
			return m_data[cur];			
		}
		else
		{
			_cprintf("finish random number, return the first first item \n");
			return m_data[0];
		}
	}

	void getPreviousItemAndTotalItem(int &curIdx, int &totalItems)
	{
		curIdx = m_currentIndex - 1;
		totalItems = m_randomIndex.size();
	}

private:
	int randRange(int s, int e)
	{
		assert(s <= e);
		if (s == e)	{
			return e;
		}
		return s + rand()%(e-s);
	}

private:
	vector<T> m_data;
	vector<int> m_randomIndex;
	unsigned int m_currentIndex;
};

struct XYPosition
{
	int x;
	int y;
};

typedef RandomTemplate<XYPosition> TwoDimensionSample;

#endif


