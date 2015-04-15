/********************************************************
**                B T D   S Y S T E M S                **
**                =====================                **
** PeerCtrl Library                                    **
** ©2002 Ed Powley                                     **
** e@btd2001.freeserve.co.uk     http://btdsys.cjb.net **
*********************************************************
** You may use this header file and the associated     **
** library peerctrl.lib to create FREEWARE machines    **
** for Buzz, as long as you give me credit in the      **
** About dialog or documentation.                      **
** Email me with any queries or bug reports.           **
*********************************************************
** Created on 18-Aug-02                                **
********************************************************/

#ifndef __BTDSYS__PEERCTRL__
#define __BTDSYS__PEERCTRL__

#include "windows.h"
#include "../mdk/mdk.h"
#include <float.h>
#include <stdlib.h>
#include <math.h>

//NOTES:
//* Don't forget to link peerctrl.lib with your project! :)
//*	Param numbers are zero based for both global and track params (ie 0 refers
//	to both first global and first track
//* Parameter string format: "[G] 1: Cutoff"
//	ie "[%s] %i: %s", (Group == 1) ? "G" : "T", ParNum, ParName

#define GROUP_GLOBAL	1
#define GROUP_TRACK		2

#define ALLOW_SWITCH		1
#define ALLOW_BYTE			2
#define ALLOW_NOTE			4
#define ALLOW_WORD			8

#define ALLOW_GLOBAL		16
#define ALLOW_TRACK			32

#define ALLOW_ALL_TYPES		ALLOW_SWITCH | ALLOW_BYTE | ALLOW_NOTE | ALLOW_WORD
#define ALLOW_ALL_GROUPS	ALLOW_GLOBAL | ALLOW_TRACK



//The biggie... this class encapsulates all data/methods for controlling one
//parameter of one machine

class CPeerCtrl
{
public:
	//Call this in Init(). pmac = pCB->GetThisMachine(), pmi = this
	CPeerCtrl(CMachine *pmac, CMachineInterface *pmi);

	~CPeerCtrl();

	//Call in Init() as part of your custom data input (if any)
	void ReadFileData (CMachineDataInput  * const pi);

	//Call in Save() as part of your custom data output (if any)
	void WriteFileData(CMachineDataOutput * const po);

	//Populate dialog items with machines
	//hWnd = parent window (eg hDlg), ID = id of control (eg IDC_MACHINECOMBO)
	//ExcludeName = Name of calling machine if it cannot control itself, 
	//  or "" otherwise
	int  GetMachineNamesToCombo	(HWND hWnd, int ComboID, const char *ExcludeName);
	int  GetMachineNamesToList	(HWND hWnd, int ListID , const char *ExcludeName);

	//Populate dialog items with parameters on specified machine
	//hWnd = parent window (eg hDlg), ID = id of control (eg IDC_MACHINECOMBO)
	//TypesAllowed: see ALLOW_ constants above ('|' them together)
	int  GetParamNamesToCombo	(CMachine *pmac, HWND hWnd, int ComboID, int TypesAllowed);
	int  GetParamNamesToList	(CMachine *pmac, HWND hWnd, int ListID , int TypesAllowed);

	//Try to select current machine/param in dialog item
	//hWnd = parent window (eg hDlg), ID = id of control (eg IDC_MACHINECOMBO)
	bool SelectMachineInCombo	(HWND hWnd, int ComboID);
	bool SelectMachineInList	(HWND hWnd, int ListID);
	bool SelectParameterInCombo	(HWND hWnd, int ComboID);
	bool SelectParameterInList	(HWND hWnd, int ListID);

	//Create assignments
	//ParaName: see note on parameter strings above
	void AssignParameter(CMachine *pMachine, int Group, int Param);
	void AssignParameter(char *MacName, char *ParaName);

	//Forget the current assignment
	void UnassignParameter();

	//Get pointers to things relating to current machine/parameter
	//If there is no current assignment, all return NULL
	CMachine *GetMachine();
	char const *GetMachineName();
	CMachineInfo const *GetMInfo();
	CMachineInterface *GetMInterface();
	CMachineParameter const *GetParamInfo();
	char const *GetParamString();

	//Get a string describing the current assignment (like "Bass 3->Cutoff")
	char const *GetAssignmentString();

	//Call CheckMachine on EVERY tick!!!!!! VERY important!
	//Returns false if the machine isn't there (eg user deleted it)
	bool CheckMachine();

	//Surround each bit of code where peer control will happen with if(GotParam())
	//Also use it to check if there is a current assignment
	bool GotParam();

	//Change the current parameter to the specified value
	//Track is ignored if we have a global parameter
	//"Non-hacky" method, only changes on tick
	void ControlChange_NextTick (int track, int value);

	//"Hacky" method, changes immediately
	void ControlChange_Immediate(int track, int value);

	////////////////////////////////////////////////////////////////

	// changes from here down -jmmcd

public: // these aren't usually public - but i need to access them! -jmmcd

	CMachine *BusMachine;
	CMICallbacks *pCB;
	int BusParNo, BusGroup;

private:
	int GetParamNames(CMachine *pmac, HWND DlgItem, int AddMessage, int TypesAllowed);
	int  FindMachine(HWND DlgItem, int SelectStringMessage);
	bool FindParam(HWND DlgItem, int SelectStringMessage);
	
	CMachine *ParentMac;

	bool HavePar;
	bool JustLoaded;
	char JustLoadedMacName[128];

	const CMachineParameter *BusPar;
};

//////////////////////////////////////////////////////////

// CPeerCtrlNote class just contains pointers to several CPeerCtrl objects.

class CPeerCtrlNote {
public:
	CPeerCtrlNote(CPeerCtrl *note, CPeerCtrl *vol, CPeerCtrl *length, CPeerCtrl *slide);
	~CPeerCtrlNote();
	void ControlChange_Immediate(int track, int note, int volume, int length, int slide);
	CPeerCtrl *note_ctrl, *vol_ctrl, *length_ctrl, *slide_ctrl;
};



#endif
