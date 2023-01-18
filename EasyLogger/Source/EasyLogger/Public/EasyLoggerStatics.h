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
	 * @param	InString		The string to log out
	 * @param	Severity		Affects the color of the text when bPrintToScreen == true, as well as the severity displayed in the editor log.
	 * @param	bPrintToScreen	Whether or not to print the output to the screen
	 * @param	Duration		The display duration (if Print to Screen is True).
	 */
	UFUNCTION(BlueprintCallable, Category = "Log", meta = (WorldContext = "WorldContextObject"))
	static void EZLog(const UObject* WorldContextObject, const FString& InString,
	                  const EEZLogSeverity Severity = EEZLogSeverity::Display,
	                  const bool bPrintToScreen = true, float Duration = 20.f);

	/**
	 * CPP Variant of EZLog.
	 * Prints a string to the log under the EasyLogger category, and optionally to the screen.
	 * Prefixes the log with the class and function name provided by the Origin parameter.
	 *
	 * @param	Origin			Used in the prefix for the log, will almost always be __FUNCTION__
	 * @param	InString		The string to log out
	 * @param	Severity		Affects the color of the text when bPrintToScreen == true, as well as the severity displayed in the editor log.
	 * @param	bPrintToScreen	Whether or not to print the output to the screen
	 * @param	Duration		The display duration (if Print to Screen is True).
	 */
	static void EZLog(const UObject* WorldContextObject, const char* Origin, const FString& InString,
	                  const EEZLogSeverity Severity = EEZLogSeverity::Display,
	                  const bool bPrintToScreen = true, float Duration = 20.f);

private:
	static void EZLogInternal(const bool bFromCPP, const UObject* WorldContextObject, const FString& InString,
	                          const EEZLogSeverity Severity, const bool bPrintToScreen, float Duration);
};
