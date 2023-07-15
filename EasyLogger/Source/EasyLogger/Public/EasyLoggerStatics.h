// Copyright Nathan Ralph. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EasyLoggerStatics.generated.h"

/**
 * Determines the UE_LOG type and the on screen display color of EZLogs
 */
UENUM(BlueprintType)
enum class EEzLogSeverity : uint8
{
	Display,
	Warning,
	Error
};

/**
 * Determines how the log is recorded
 */
UENUM(BlueprintType)
enum class EEzLogPrintType : uint8
{
	ScreenAndLog,
	ScreenOnly,
	LogOnly
};

UCLASS()
class EASYLOGGER_API UEasyLoggerStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Prints a string to the log under the EasyLogger category, and optionally to the screen
	 * Prefixes the log with the class and function name
	 * When used from an Event Graph, will prefix BlueprintName::ExecuteUbergraph_BlueprintName()
	 *
	 * @param InString	The string to log out
	 * @param Severity	Affects the color of the text when printing to screen, as well as the severity displayed in the editor log
	 * @param PrintType	Determines if the log is displayed to screen, printed to the output log, or both
	 * @param Duration	The display duration (if Print to Screen is True)
	 */
	UFUNCTION(BlueprintCallable, Category = "Log", meta = (WorldContext = "WorldContextObject"))
	static void EzLog(const UObject* WorldContextObject,
	                  const FString& InString = "EzLog",
	                  const EEzLogSeverity Severity = EEzLogSeverity::Display,
	                  const EEzLogPrintType PrintType = EEzLogPrintType::ScreenAndLog,
	                  float Duration = 20.f);

	/**
	 * CPP Variant of EzLog
	 * Prints a string to the log under the EasyLogger category, and optionally to the screen
	 * Prefixes the log with the class and function name provided by the Origin parameter
	 *
	 * @param Origin	Used in the prefix for the log, will almost always be __FUNCTION__
	 * @param InString	The string to log out
	 * @param Severity	Affects the color of the text when printing to screen, as well as the severity displayed in the editor log
	 * @param PrintType	Determines if the log is displayed to screen, printed to the output log, or both
	 * @param Duration	The display duration (if Print to Screen is True)
	 */
	static void EzLog(const UObject* WorldContextObject,
	                  const char* Origin,
	                  const FString& InString = "EzLog",
	                  const EEzLogSeverity Severity = EEzLogSeverity::Display,
	                  const EEzLogPrintType PrintType = EEzLogPrintType::ScreenAndLog,
	                  float Duration = 20.f);

private:
	/** Internal only function used to add origin information for the CPP EzLog variant */
	static void EzLogInternal(const bool bFromCPP,
	                          const UObject* WorldContextObject,
	                          const FString& InString,
	                          const EEzLogSeverity Severity,
	                          const EEzLogPrintType PrintType,
	                          float Duration);
};
