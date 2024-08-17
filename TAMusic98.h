#if defined(_DEBUG)
const char *logfile = "win32log.txt";
void logToFile(char *msg);
#endif

bool msMode;
char currentTrack[3];
unsigned short volume;

std::vector<std::string> split(std::string s, std::string delimiter);

unsigned short convertVolume(DWORD vol);

DWORD backConvertVolume(unsigned short vol);

MMRESULT setVolume(unsigned short vol);

MCIERROR __stdcall impl_mciSendStringA( LPCTSTR lpszCommand,
										LPTSTR lpszReturnString,
										UINT cchReturn,
										HWND hwndCallback
									  );

MMRESULT __stdcall impl_auxGetVolume(UINT uDeviceID, LPDWORD pdwVolume);

MMRESULT __stdcall impl_auxSetVolume(UINT uDeviceID, DWORD dwVolume);
