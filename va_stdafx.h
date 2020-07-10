#define DEFINE_LOG_CATEGORY(LOG_CATEGORY) enum { LOG_CATEGORY };
enum UE_LOG_TYPE { NoLogging = 0, Fatal, Error, Warning, Display, Log, Verbose, VeryVerbose, All, NumVerbosity, VerbosityMask, SetColor, BreakOnLog };
void UE_LOG(int CatgeoryName, UE_LOG_TYPE Verbosity, TCHAR *Format, ...);
void UE_CLOG(int Condition, int CatgeoryName, UE_LOG_TYPE Verbosity, TCHAR *Format, ...);
