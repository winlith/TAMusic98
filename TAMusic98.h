#if defined(_DEBUG)
const char *logfile = "win32log.txt";
void logToFile(char *msg);
#endif

bool msMode;
char currentTrack[3];

std::vector<std::string> split(std::string s, std::string delimiter);

MCIERROR __stdcall impl_mciSendStringA( LPCTSTR lpszCommand,
										LPTSTR lpszReturnString,
										UINT cchReturn,
										HWND hwndCallback
									  );
