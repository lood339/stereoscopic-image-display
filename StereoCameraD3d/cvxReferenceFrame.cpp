#include "cvxReferenceFrame.h"
#include <iostream>
#include "d3dStereoCamera.h"

using namespace::std;

CvxReferenceFrame::CvxReferenceFrame()
{
//	m_leftTopCorner = cv::Point(0, 1080/4);
//	m_rightTopConer = cv::Point(1920/4, 1080/4);
	m_referenceGauge = new StdGauge();
	m_referenceGauge->setGauge(0.05f);
	m_referenceGauge->setColorIdx(1);

	m_groundTruthGauge = new StdGauge();
	m_groundTruthGauge->setGauge(0.04f);
	m_groundTruthGauge->setColorIdx(2);
}

CvxReferenceFrame::~CvxReferenceFrame()
{
	cout<<"delete CvxReferenceFrame"<<endl;
}

void CvxReferenceFrame::loadImages(const cv::Mat &lImage, const cv::Mat &rImage)
{
	int rows = lImage.rows;
	int cols = lImage.cols;	

	assert(rows == rImage.rows);
	assert(cols == rImage.cols);
	
	
	cv::imshow("left", m_leftImage);
	cv::imshow("right", m_rightImage);
	cv::imwrite("left.png", m_leftImage);
	cv::imwrite("right.png", m_rightImage);	
}

void CvxReferenceFrame::pushBackNormalPosition(const vec3f &n, const vec3f &p)
{
	m_normals.push_back(n);
	m_positions.push_back(p);
}

void CvxReferenceFrame::pushBackGroundTruthNormal(const vec3f &n)
{
	m_groundTruthNormals.push_back(n);
}

void CvxReferenceFrame::clear()
{
	m_normals.clear();
	m_positions.clear();
	m_groundTruthNormals.clear();
}
void CvxReferenceFrame::clearHalf()
{
	int sz = m_normals.size();
	if(sz != 0)
	{
		sz /= 2;
		if(sz == 0)
		{
			clear();
		}
		else
		{
			m_normals.resize(sz);
			m_positions.resize(sz);
			m_groundTruthNormals.resize(sz);
		}

	}

}

void CvxReferenceFrame::draw(IDirect3DDevice9* pDevice, D3dStereoCamera* pStereoCam, bool showGroundTruth)
{
	//draw line nodel
	m_referenceGauge->setColorIdx(1);
	m_referenceGauge->initMaterial(pDevice);
	for(unsigned int i = 0; i<m_normals.size() && i<m_positions.size(); i++)
	{
		pStereoCam->setRotationTranslationMatrix(pDevice, m_normals[i], m_positions[i]);		
		m_referenceGauge->DrawGauge(pDevice, true);
	}

	if(showGroundTruth)
	{		
		m_groundTruthGauge->setColorIdx(2);
		m_groundTruthGauge->initMaterial(pDevice);
		for(unsigned int i = 0; i<m_groundTruthNormals.size() && i<m_positions.size(); i++)
		{
			pStereoCam->setRotationTranslationMatrix(pDevice, m_groundTruthNormals[i], m_positions[i]);		
			m_groundTruthGauge->DrawGauge(pDevice, true);
		}
	}
	
}
