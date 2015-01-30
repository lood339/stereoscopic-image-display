#include "ImageModel.h"
#include <conio.h>
#include <fstream>


/*
ImageModel
*/

ImageModel::ImageModel()
{
	m_vNormal.x = 0.0f;
	m_vNormal.x = 0.0f;
	m_vNormal.y = -1.0f;

	m_vPosition.x = 0.0f;
	m_vPosition.y = 0.0f;
	m_vPosition.z = 5.0f;

	m_sampleEng = NULL;
}
ImageModel::~ImageModel()
{

}

void ImageModel::loadConfig(const string &fileName)
{
	ifstream ifile(fileName.c_str(), ios_base::in);
	ifile>>m_scene;  //read file

	m_mid    = cv::imread(m_scene.m_leftName, 1);
	m_depth  = cv::imread(m_scene.m_depthName, 0);
	m_normal = cv::imread(m_scene.m_normalName, 1);
	m_alpha  = cv::imread(m_scene.m_alphaName, 0);

	assert(!m_mid.empty());
	assert(!m_depth.empty());
	assert(!m_normal.empty());
	assert(!m_alpha.empty());

	if(m_scene.m_samplePoints.size() != 0)
	{
		_cprintf("disorder %d samples.\n", m_scene.m_samplePoints.size());
	    //std::vector<SamplePoint> m_samplePoints;
		vector<XYPosition> positions;
		for(unsigned int i = 0; i<m_scene.m_samplePoints.size(); ++i)
		{
			XYPosition pos;
			pos.x = m_scene.m_samplePoints[i].x;
			pos.y = m_scene.m_samplePoints[i].y;
			positions.push_back(pos);
		}
		m_sampleEng = new TwoDimensionSample(positions);
		assert(m_sampleEng);
	}
}
void ImageModel::saveItem(SampleItem &item)
{
	int x = item.getXpos();
	int y = item.getYpos();	

	//find x, y in scene
	for(unsigned int i = 0; i<m_scene.m_samplePoints.size(); ++i)
	{
		if(x == m_scene.m_samplePoints[i].x && y == m_scene.m_samplePoints[i].y)
		{			
			m_scene.m_samplePoints[i].gt_theta = item.getGdSlantAngle();
			m_scene.m_samplePoints[i].gt_phi   = item.getGdRotationAngle();
			m_scene.m_samplePoints[i].r_theta  = item.getEstimatedSlantAngle();
			m_scene.m_samplePoints[i].r_phi    = item.getEstimatedRotationAngle();	
			_cprintf("save one item\n");
			return;
		}
	}
	_cprintf("can't save item................................................\n");
}

void ImageModel::getNumbers(int &restNum, int &totalNum)
{
	if(m_sampleEng)
	{
		int curIdx = 0;
		m_sampleEng->getPreviousItemAndTotalItem(curIdx, totalNum);
		restNum = totalNum - (curIdx);		
	}

}

void ImageModel::resample()
{
	if(m_scene.m_samplePoints.size() != 0)
	{
		_cprintf("create two dimensional sample\n");
	    //std::vector<SamplePoint> m_samplePoints;
		vector<XYPosition> positions;
		for(unsigned int i = 0; i<m_scene.m_samplePoints.size(); ++i)
		{
			XYPosition pos;
			pos.x = m_scene.m_samplePoints[i].x;
			pos.y = m_scene.m_samplePoints[i].y;
			positions.push_back(pos);
		}

		if(m_sampleEng)
		{
			delete m_sampleEng;
			m_sampleEng = NULL;
		}

		m_sampleEng = new TwoDimensionSample(positions);
		assert(m_sampleEng);
	}
}



vec3f ImageModel::calcNormalFromImage(int x, int y, bool fullScreen)
{
	if(!fullScreen)
	{
		x *= 2;
		y *= 2;
	}

	if (x >=0 && x<m_normal.cols && y >= 0 && y<m_normal.rows)
	{
		cvx::Rgb8UImage nor_map(&m_normal);
		float nx, ny, nz;

		nx = nor_map[y][x].r/255.0f;
		ny = nor_map[y][x].g/255.0f;
		nz = nor_map[y][x].b/255.0f;		

		nx = (nx - 0.5) * 2.0;
		ny = (ny - 0.5) * 2.0;
		nz = (nz - 0.5) * 2.0;

		
		m_vNormal.x =  -nx;
		m_vNormal.y =  -ny;
		m_vNormal.z =  nz;		
		
		m_vNormal = nv::normalize(m_vNormal);
		return m_vNormal;
	}
	else
	{
		assert(0);
		return nv::vec3f(0, 0, -1.0);
	}
}


vec3f ImageModel::calcPositionFromImage(int x, int y, bool fullScreen)
{
	if(!fullScreen)
	{
		x *= 2;
		y *= 2;
	}

	if(x >= 0 && x<m_depth.cols && y >= 0 && y<m_depth.rows)
	{
		float px, py, pz;
		assert(m_scene.m_depthResize != 0.0);
		int h = m_depth.rows;
		int w = m_depth.cols;
		//float nx, ny, nz;		

		cvx::Bw8UImage depth_map(&m_depth);
		px = (float)x;
		py = (float)y;
		pz = ((float)depth_map[y][x]/255.0)/m_scene.m_depthResize - m_scene.m_depthOffset;

		float fc = m_scene.m_focalLength;
		float sw = m_scene.m_sensorWidth;
		float ratio = w/sw;
		fc *= ratio;

		//reverse y since y = 0 in left top corner of windows
		y = h - y;

		//image the image plane in [0, 0, fc] and [-w/2, -h/2] to [w/2, h/2]
		nv::vec3f dir(x - w/2, y - h/2, fc);

		dir = nv::normalize(dir);
		assert(dir.z != 0.0);

		float scale = pz/dir.z;

		dir *= scale;

		px = dir.x;
		py = dir.y;
		pz = dir.z;

		m_vPosition = dir;
		return m_vPosition;
	}
	else
	{
		assert(0);
		return vec3f(0, 0, 10);
	}
}

bool ImageModel::getNextSampleItem(SampleItem &item)
{
	if(m_sampleEng)
	{
		if(m_sampleEng->isEnd())
		{
			_cprintf("SampleItem is to the end\n");			
			return false;
		}
		else
		{
			XYPosition pos = m_sampleEng->getCurrentItemAndStepForward();			

			item.setSampleCenter(pos.x, pos.y, true);
			//resize sample points if the imagesize is not the screen size
			vec3f n = this->calcNormalFromImage(pos.x, pos.y, true);
			vec3f p = this->calcPositionFromImage(pos.x, pos.y, true);
			item.setGroundtruthNormal(n);
			item.setGroundtruthPosition(p);			

			_cprintf("return predifined position.\n");
			return true;			
		}
	}
	else
	{
		_cprintf("return random position.\n");
		return this->getARandomSamplePoint(item);
	}
}
bool ImageModel::getARandomSamplePoint(SampleItem &item)
{
	cvx::Bw8UImage a_map(&m_alpha);
	int h = m_alpha.rows;
	int w = m_alpha.cols;

	int num = 100;
	while(true && num)
	{
		int x = rand()%w;
		int y = rand()%h;
		if(a_map[y][x] == 255)
		{
			item.setSampleCenter(x, y, true);

			vec3f n = this->calcNormalFromImage(x, y, true);
			vec3f p = this->calcPositionFromImage(x, y, true);
			item.setGroundtruthNormal(n);
			item.setGroundtruthPosition(p);			
			return true;
		}
		num--;
		if(num == 0)
		{
			break;			
		}
	}
	_cprintf("can not sample valid point, please check alpha map!");
	return false;
}

void ImageModel::printCurrentResult(bool slant, bool rotation)
{
	for(int i = 0; i<m_scene.m_samplePoints.size(); ++i)
	{
		float gt_theta = m_scene.m_samplePoints[i].gt_theta;
		float gt_phi   = m_scene.m_samplePoints[i].gt_phi;
		float r_theta  = m_scene.m_samplePoints[i].r_theta;
		float r_phi    = m_scene.m_samplePoints[i].r_phi;

		if(slant && rotation)
		{
			_cprintf("slant angle: %d %d\nrotation angle %d %d\n", (int)gt_theta, (int)r_theta, (int)gt_phi, (int)r_phi);
		}
		else if(slant)
		{
			_cprintf("slant angle: %d %d\n", (int)gt_theta, (int)r_theta);
		}
		else if(rotation)
		{
			_cprintf("rotation angle %d %d\n", (int)gt_phi, (int)r_phi);
		}
	}
}
void ImageModel::saveResult(const string &fileName)
{
	_cprintf("save result to file %s \n", fileName.c_str());

	ofstream ofile(fileName.c_str(), ios_base::out);
	ofile<<m_scene;
	ofile.close();
}
void ImageModel::saveResult(ostream & os)
{
	os<<m_scene;
	os<<endl;
}

