#pragma once

DECLARE_LOG_CATEGORY_CLASS(LogHm, All, All);

#define EXEC_INFO_FORMAT "%s::%d: "
#define EXEC_INFO *FString(__FUNCTION__), __LINE__

#define _HM_LOG_IMPL(VERBOSITY, FORMAT, ...) \
UE_LOG(LogHm, VERBOSITY, \
TEXT(EXEC_INFO_FORMAT FORMAT), EXEC_INFO, __VA_ARGS__)

#define HM_FATAL(FORMAT, ...)		_HM_LOG_IMPL(Fatal,			FORMAT, __VA_ARGS__)
#define HM_ERROR(FORMAT, ...)		_HM_LOG_IMPL(Error,			FORMAT, __VA_ARGS__)
#define HM_WARN(FORMAT, ...)		_HM_LOG_IMPL(Warning,		FORMAT, __VA_ARGS__)
#define HM_DISPLAY(FORMAT, ...)		_HM_LOG_IMPL(Display,		FORMAT, __VA_ARGS__)
#define HM_LOG(FORMAT, ...)			_HM_LOG_IMPL(Log,			FORMAT, __VA_ARGS__)
#define HM_VERBOSE(FORMAT, ...)		_HM_LOG_IMPL(Verbose,		FORMAT, __VA_ARGS__)
#define HM_VVERBOSE(FORMAT, ...)	_HM_LOG_IMPL(VeryVerbose,	FORMAT, __VA_ARGS__)

#define S_EXEC_INFO_FORMAT "%s::%d %s: "
#define S_EXEC_INFO *FString(__FUNCTION__), __LINE__, *GetNameSafe(this)

#define _HMS_LOG_IMPL(VERBOSITY, FORMAT, ...) \
UE_LOG(LogHm, VERBOSITY, \
TEXT(S_EXEC_INFO_FORMAT FORMAT), S_EXEC_INFO, __VA_ARGS__)

#define HMS_FATAL(FORMAT, ...) \
	do { _HMS_LOG_IMPL(Fatal,			FORMAT, __VA_ARGS__); } while(0)
#define HMS_ERROR(FORMAT, ...) \
	do { _HMS_LOG_IMPL(Error,			FORMAT, __VA_ARGS__); } while(0)
#define HMS_WARN(FORMAT, ...) \
	do { _HMS_LOG_IMPL(Warning,			FORMAT, __VA_ARGS__); } while(0)
#define HMS_DISPLAY(FORMAT, ...) \
	do { _HMS_LOG_IMPL(Display,			FORMAT, __VA_ARGS__); } while(0)
#define HMS_LOG(FORMAT, ...) \
	do { _HMS_LOG_IMPL(Log,				FORMAT, __VA_ARGS__); } while(0)
#define HMS_VERBOSE(FORMAT, ...) \
	do { _HMS_LOG_IMPL(Verbose,			FORMAT, __VA_ARGS__); } while(0)
#define HMS_VVERBOSE(FORMAT, ...) \
	do { _HMS_LOG_IMPL(VeryVerbose,		FORMAT, __VA_ARGS__); } while(0)
