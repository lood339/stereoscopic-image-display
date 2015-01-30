#ifndef USER_INPUT_UI_H
#define USER_INPUT_UI_H 1

//user input for experiment type, is show reference gauge and keyboard input

#include "cvxDxut.h"
#include "cvxD3d9.h"

enum ModelType{SHOW_GROUND_TRUTH = 0, EXPERIMENT, EXPERIMENT_ONLY_SLANT, EXPERIMENT_ONLY_RORATION, EXPERIMENT_3D_ROTATION};
enum ReferenceType {REFERENCE_PRACTICE, REFERENCE_SHOW_GROUNDTRUTH, REFERENCE_SHOW_GROUNDTRUTH_AND_REMINDER};
enum SeparationState {TESTING, ADJUST_SEPERATION};

struct UsertInputUI
{
	CDXUTComboBox *_modelCB;  //show ground truth, test, or experiment
	CDXUTCheckBox *_showReference;
	CDXUTButton   *_openNewFile;
	CDXUTButton   *_openSaveFile;
	CDXUTButton   *_openMultipleNewFile;
	int models[5];
	
private:	
	int _model;
	int _referenceModel;     //reference models for practice only
	bool _isMouseMoving;
	int _seperationState;    //state for testing or adjust seperation
	
public:
	UsertInputUI()
	{
		_modelCB = NULL;
		_showReference = NULL;
		_openNewFile = NULL;
		_openSaveFile = NULL;
		_openMultipleNewFile = NULL;
		_model = SHOW_GROUND_TRUTH;
		models[0] = SHOW_GROUND_TRUTH;
		models[1] = EXPERIMENT;
		models[2] =	EXPERIMENT_ONLY_SLANT;
		models[3] = EXPERIMENT_ONLY_RORATION;
		models[4] = EXPERIMENT_3D_ROTATION;
		_isMouseMoving  = false;
		_referenceModel = REFERENCE_PRACTICE;
		_seperationState = TESTING;
	}
	
	void setReference2Practice()
	{
		_referenceModel = 0;
	}
	void setReference2ShowGroundTruth()
	{
		_referenceModel = 1;
	}
	void setReference2ShowGroundtruthAndReminder()
	{
		_referenceModel = 2;
	}

	bool isTesting()
	{
		return _seperationState == TESTING;
	}

	bool isAdjustSeperation()
	{
		return _seperationState == ADJUST_SEPERATION;
	}
	
	void nextSeperationState()
	{
		_seperationState += 1;
		_seperationState %=2;
	}

	bool isShowReference()
	{
		return _referenceModel == REFERENCE_SHOW_GROUNDTRUTH || _referenceModel == REFERENCE_SHOW_GROUNDTRUTH_AND_REMINDER;
	}
	bool isReferenceGroundTruth()  //load next trial in practice model
	{
		return _referenceModel == REFERENCE_SHOW_GROUNDTRUTH;
	}
	bool isReferenceRemindder()
	{
		return _referenceModel == REFERENCE_SHOW_GROUNDTRUTH_AND_REMINDER;
	}
	bool is3DRotation()
	{
		return _model == EXPERIMENT_3D_ROTATION;
	}
	void setMouseMoving()
	{
		_isMouseMoving = true;
	}
	void resetMouseMoving()
	{
		_isMouseMoving = false;
	}

	bool isMouseMoving()
	{
		return _isMouseMoving;
	}
	bool isShowGroundTruth()
	{
		return _model == SHOW_GROUND_TRUTH;
	}
	bool isSlantChange()
	{
		return _model == EXPERIMENT || _model == EXPERIMENT_ONLY_SLANT;
	}
	bool isRotationChange()
	{
		return _model == EXPERIMENT || _model == EXPERIMENT_ONLY_RORATION;
	}
	bool isExperiment()
	{
		return _model == EXPERIMENT || _model == EXPERIMENT_ONLY_SLANT || _model == EXPERIMENT_ONLY_RORATION || _model == EXPERIMENT_3D_ROTATION;
	}
	bool isSlantOnly()
	{
		return _model == EXPERIMENT_ONLY_SLANT;
	}
	bool isRotationOnly()
	{
		return _model == EXPERIMENT_ONLY_RORATION;
	}
	bool isBothSlantAndRotation()
	{
		return _model == EXPERIMENT;
	}

	void getModelValue()
	{
		DXUTComboBoxItem* pItem = _modelCB->GetSelectedItem();
		if(pItem)
		{
			_model = *(int *)(pItem->pData);
		}		
	}
};

#endif USER_INPUT_UI_H

