// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EasyLoggerStatics.generated.h"

/**
 * Determines the UE_LOG type and the on screen display color of EZLogs.
 */
UENUM(BlueprintType)
enum class EEZLogSeverity : uint8
{
	Display,
	Warning,
	Error
};

/**
 * Determines how the log is recorded.
 */
UENUM(BlueprintType)
enum class EEZLogPrintType : uint8
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
	 * Prints a string to the log under the EasyLogger category, and optionally to the screen.
	 * Prefixes the log with the class and function name.
	 * When used from an Event Graph, will prefix BlueprintName::ExecuteUbergraph_BlueprintName()
	 *
	 * @param	InString		The string to log out.
	 * @param	Severity		Affects the color of the text when printing to screen, as well as the severity displayed in the editor log.
	 * @param	PrintType		Determines if the log is displayed to screen, printed to the output log, or both.
	 * @param	Duration		The display duration (if Print to Screen is True).
	 */
	UFUNCTION(BlueprintCallable, Category = "Log", meta = (WorldContext = "WorldContextObject"))
	static void EZLog(const UObject* WorldContextObject, const FString& InString = "EZLog",
	                  const EEZLogSeverity Severity = EEZLogSeverity::Display,
	                  const EEZLogPrintType PrintType = EEZLogPrintType::ScreenAndLog,
	                  float Duration = 20.f);

	/**
	 * CPP Variant of EZLog.
	 * Prints a string to the log under the EasyLogger category, and optionally to the screen.
	 * Prefixes the log with the class and function name provided by the Origin parameter.
	 *
	 * @param	Origin			Used in the prefix for the log, will almost always be __FUNCTION__
	 * @param	InString		The string to log out.
	 * @param	Severity		Affects the color of the text when printing to screen, as well as the severity displayed in the editor log.
	 * @param	PrintType		Determines if the log is displayed to screen, printed to the output log, or both.
	 * @param	Duration		The display duration (if Print to Screen is True).
	 */
	static void EZLog(const UObject* WorldContextObject, const char* Origin, const FString& InString = "EZLog",
	                  const EEZLogSeverity Severity = EEZLogSeverity::Display,
	                  const EEZLogPrintType PrintType = EEZLogPrintType::ScreenAndLog,
	                  float Duration = 20.f);

private:
	static void EZLogInternal(const bool bFromCPP, const UObject* WorldContextObject, const FString& InString,
	                          const EEZLogSeverity Severity, const EEZLogPrintType PrintType, float Duration);
};
