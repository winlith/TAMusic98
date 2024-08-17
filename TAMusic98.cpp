#include "stdafx.h"
#include "TAMusic98.h"

#if defined(_DEBUG)
void logToFile(char *msg)
{
	time_t t = time(NULL);
	tm *now = localtime(&t);
	std::ofstream outfile;
	outfile.open(logfile, std::ios::out | std::ios::app);
	outfile << 
		"[" << 
		now->tm_hour <<
		":" <<
		now->tm_min <<
		":" <<
		now->tm_sec <<
		"] " <<
		msg <<
		"\n";
	outfile.close();
}
#endif

// https://stackoverflow.com/a/14266139
std::vector<std::string> split(std::string s, std::string delimiter)
{
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;
	while((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
	{
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}
	res.push_back(s.substr(pos_start));
	return res;
}

unsigned short convertVolume(DWORD vol)
{
	return (unsigned short)(((float)(vol & 0x0000FFFF)) / 65535.0f * 1000.0f);
}

DWORD backConvertVolume(unsigned short vol)
{
	unsigned short singleVolume = (unsigned short)(((float)vol) / 1000.0f * 65535.0f);
	return (DWORD)singleVolume | ((DWORD)singleVolume << 16);
}

MMRESULT setVolume(unsigned short vol)
{
	char cmd[30];
	sprintf(cmd, "setaudio song volume to %d", vol);
#if defined(_DEBUG)
	logToFile(cmd);
#endif
	return mciSendString(cmd, NULL, 0, NULL);
}

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			mciSendStringA("close song", NULL, 0, NULL);
			break;
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

MCIERROR __stdcall impl_mciSendStringA( LPCTSTR lpszCommand,
										LPTSTR lpszReturnString,
										UINT cchReturn,
										HWND hwndCallback
									  )
{
#if defined(_DEBUG)
	logToFile((char *)lpszCommand);
#endif
	MCIERROR result = 0;
	std::vector<std::string> command = split(std::string(lpszCommand), " ");
	if(command[0].compare("play") == 0)
	{
		if(command.size() <= 3)
		{
			result = mciSendStringA("play song notify", NULL, 0, hwndCallback);
		}
		else
		{
			if(!msMode)
			{
				result = mciSendStringA("close song", NULL, 0, NULL);
				strcpy(currentTrack, command[3].c_str());
				std::string openCmd = "open \"music\\";
				openCmd += currentTrack;
				openCmd +=".mp3\" type mpegvideo alias song";
				result = mciSendStringA(openCmd.c_str(), NULL, 0, NULL);
				result = setVolume(volume);
				result = mciSendStringA("set song time format milliseconds", NULL, 0, NULL);
				char *length = (char *)malloc(sizeof(char)*64);
				result = mciSendStringA("status song length", length, 64, NULL);
				std::string playCmd = "play song from 0 to ";
				playCmd += length;
				playCmd += " notify";
				free(length);
				result = mciSendStringA(playCmd.c_str(), NULL, 0, hwndCallback);
			}
			else
			{
				std::string cmd = "play song from ";
				cmd += command[3];
				cmd += " to ";
				cmd += command[5];
				cmd += " notify";
				result = mciSendStringA(cmd.c_str(), NULL, 0, hwndCallback);
			}
		}
	}
	else if(command[0].compare("stop") == 0)
	{
		result = mciSendStringA("stop song", NULL, 0, hwndCallback);
		result = mciSendStringA("close song", NULL, 0, NULL);
		strcpy(currentTrack, "0");
	}
	else if(command[0].compare("pause") == 0)
	{
		result = mciSendStringA("pause song", NULL, 0, hwndCallback);
	}
	else if(command[0].compare("set") == 0)
	{
		if(command[4].compare("milliseconds") == 0)
		{
			msMode = true;
		}
		else
		{
			msMode = false;
		}
	}
	else if(command[0].compare("status") == 0)
	{
		if(command[2].compare("number") == 0)
		{
			strcpy(lpszReturnString, "17");
		}
		else if(command[2].compare("type") == 0)
		{
			strcpy(lpszReturnString, "other");
		}
		else if(command[2].compare("mode") == 0)
		{
			result = mciSendStringA("status song mode", lpszReturnString, cchReturn, hwndCallback);
		}
		else if(command[2].compare("current") == 0)
		{
			strcpy(lpszReturnString, currentTrack);
		}
		else if(command[2].compare("position") == 0)
		{
			if(command.size() > 3)
			{
				result = mciSendStringA("status song length", lpszReturnString, cchReturn, hwndCallback);
			}
			else
			{
				result = mciSendStringA("status song position", lpszReturnString, cchReturn, hwndCallback);
			}
		}
	}

#if defined(_DEBUG)
	if(lpszReturnString != NULL)
	{
		logToFile((char*)lpszReturnString);
	}
	else
	{
		logToFile("NULL return");
	}
	if(hwndCallback != 0)
	{
		logToFile("non-null hwnd");
	}
	else
	{
		logToFile("NULL hwnd");
	}
#endif

	return result;
}

MMRESULT __stdcall impl_auxGetVolume(UINT uDeviceID, LPDWORD pdwVolume)
{
#if defined(_DEBUG)
	char log[32];
	sprintf(log, "auxGetVolume %x", *pdwVolume);
	logToFile(log);
#endif
	*pdwVolume = backConvertVolume(volume);
	return 0;
}

MMRESULT __stdcall impl_auxSetVolume(UINT uDeviceID, DWORD dwVolume)
{
#if defined(_DEBUG)
	char log[32];
	sprintf(log, "auxSetVolume %x", dwVolume);
	logToFile(log);
#endif
	volume = convertVolume(dwVolume);
	return setVolume(volume);
}
