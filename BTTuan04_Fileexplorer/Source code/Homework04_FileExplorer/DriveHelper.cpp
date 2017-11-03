#include "stdafx.h"
#include "DriveHelper.h"

DriveHelper::DriveHelper()
{
	mDriveLetter = NULL;
	mVolumeLabel = NULL;
	mDriveType = NULL;
	mNumberOfDrive = 0;
}

DriveHelper::~DriveHelper()
{
	for (int i = 0; i < mNumberOfDrive; ++i)
	{
		delete[] mDriveLetter[i];
		delete[] mVolumeLabel[i];
		delete[] mDriveType[i];
	}

	delete[] mDriveLetter;
	delete[] mVolumeLabel;
	delete[] mDriveType;
	mNumberOfDrive = 0;
}



TCHAR* DriveHelper::getDriveType(int position)
{
	return mDriveType[position];
}

TCHAR* DriveHelper::getDriveLetter(const int &i)
{
	return mDriveLetter[i];
}


TCHAR* DriveHelper::getDisplayName(const int &i)
{
	return mVolumeLabel[i];
}

int DriveHelper::getCount()
{
	return mNumberOfDrive;
}



void DriveHelper::getSystemDrives()
{
	TCHAR buffer[MAX_BUFFER_LEN];
	TCHAR* volumeTempName = new TCHAR[VOLUME_NAME_SIZE];

	GetLogicalDriveStrings(MAX_BUFFER_LEN, buffer);

	mNumberOfDrive = 0;
	for (int i = 0; (buffer[i] != '\0') || (buffer[i - 1] != '\0'); i++)
	{
		if (buffer[i] == '\0')
		{
			mNumberOfDrive++;
		}
	}

	//Cấp phát bộ nhớ để chứa chuỗi tương ứng
	mDriveLetter = new TCHAR*[mNumberOfDrive];
	mVolumeLabel = new TCHAR*[mNumberOfDrive];
	mDriveType = new TCHAR*[mNumberOfDrive];
	mDriveSize = new CDriveSize*[mNumberOfDrive];

	for (int i = 0; i < mNumberOfDrive; ++i)
	{
		mDriveLetter[i] = new TCHAR[MAX_DRIVE_LETTER];
		mVolumeLabel[i] = new TCHAR[MAX_DISPLAY_NAME];
		mDriveType[i] = new TCHAR[MAX_TYPE_LEN];
	}

	int j, k;
	int index = 0;

	for (j = 0; j < mNumberOfDrive; ++j)
	{
		k = 0;
		while (buffer[index] != 0)
		{
			mDriveLetter[j][k++] = buffer[index++];
		}
		mDriveLetter[j][k] = '\0';
		++index;
	}

	//Get label name, type and size of every single volume
	int nType;
	for (int i = 0; i < mNumberOfDrive; ++i)
	{
		//Determines whether a disk drive is a removable, fixed, CD-ROM, RAM disk, or network drive
		nType = GetDriveType(mDriveLetter[i]);

		//Assign type
		switch (nType)
		{
		case DRIVE_FIXED:
			StrCpy(mDriveType[i], FIXED_DRIVE);
			break;
		case DRIVE_REMOVABLE:
			StrCpy(mDriveType[i], REMOVABLE_DRIVE);
			break;
		case DRIVE_REMOTE:
			StrCpy(mDriveType[i], REMOTE_DRIVE);
			break;
		case DRIVE_CDROM:
			StrCpy(mDriveType[i], CD_ROM);
			break;
		default:
			break;
		}

		//Get drive size
		mDriveSize[i] = getDriveSize(i);

		//Clear buffer
		StrCpy(buffer, _T(""));

		//If drive is Hard drives, USB flash drive, network drive
		if ((nType == DRIVE_FIXED) || ((nType == DRIVE_REMOVABLE)) || (nType == DRIVE_REMOTE))
		{
			//Retrieves information about the file system and volume associated with the specified root directory
			GetVolumeInformation(mDriveLetter[i], buffer, MAX_BUFFER_LEN, VOLUME_SERIAL_NUMBER, MAX_COMPONENT_LEN, FILE_SYSTEM_FLAG, FILE_SYSTEM_NAME_BUFF, FILE_SYSTEM_NAME_SIZE);
			StrCpy(volumeTempName, buffer);
		}
		else
			if (nType == DRIVE_CDROM)
			{
				GetVolumeInformation(mDriveLetter[i], buffer, MAX_BUFFER_LEN, VOLUME_SERIAL_NUMBER, MAX_COMPONENT_LEN, FILE_SYSTEM_FLAG, FILE_SYSTEM_NAME_BUFF, FILE_SYSTEM_NAME_SIZE);

				if (wcslen(buffer) == 0)
				{
					//If cant get CD-ROM label assign "CD Rom" to it
					StrCpy(volumeTempName, _T("CD-ROM"));
				}
				else
					StrCpy(volumeTempName, buffer);
			}
			else if (((i == 0) || (i == 1)) && (nType == DRIVE_REMOVABLE)) //i == 0 or i == 1 because loppy disk usually is A:\\ drive
			{
				StrCpy(volumeTempName, _T("We don't talk to floppy disk any more"));
			}

		//If drive dont have specific label name
		//Usually the C drive	
		if (wcslen(volumeTempName) == 0)
		{
			//Assign default "Local Disk"
			StrCpy(mVolumeLabel[i], _T("Local Disk"));
		}
		else
		{
			//Copy 
			StrCpy(mVolumeLabel[i], volumeTempName);
		}

		//Add the drive letter. Ex: (C:)
		StrCat(mVolumeLabel[i], _T(" ("));
		StrNCat(mVolumeLabel[i], mDriveLetter[i], 3);
		StrCat(mVolumeLabel[i], _T(")"));
	}
}



CDriveSize* DriveHelper::getDriveSize(int i)
{
	__int64 totalSize;
	__int64 freeSpace;

	SHGetDiskFreeSpaceEx(getDriveLetter(i), NULL, (PULARGE_INTEGER)&totalSize, (PULARGE_INTEGER)&freeSpace);

	CDriveSize* driveSize = new CDriveSize(totalSize, freeSpace);

	return driveSize;
}

LPWSTR DriveHelper::getFreeSpace(int i)
{
	return mDriveSize[i]->getFreeSpace();
}

LPWSTR DriveHelper::getTotalSize(int i)
{
	return mDriveSize[i]->getTotalSize();
}