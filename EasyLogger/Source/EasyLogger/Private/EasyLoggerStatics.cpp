// Copyright Nathan Ralph. All Rights Reserved.


#include "EasyLoggerStatics.h"

#include "Engine/Console.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(EasyLogger, Display, All);

void UEasyLoggerStatics::EzLog(const UObject* WorldContextObject, const FString& InString, const EEzLogSeverity Severity, const EEzLogPrintType PrintType, float Duration)
{
	EzLogInternal(false, WorldContextObject, InString, Severity, PrintType, Duration);
}

void UEasyLoggerStatics::EzLog(const UObject* WorldContextObject, const char* Origin, const FString& InString, const EEzLogSeverity Severity, const EEzLogPrintType PrintType, float Duration)
{
	EzLogInternal(true, WorldContextObject, FString(Origin) + "() " + InString, Severity, PrintType, Duration);
}

void UEasyLoggerStatics::EzLogInternal(const bool bFromCPP, const UObject* WorldContextObject, const FString& InString, const EEzLogSeverity Severity, const EEzLogPrintType PrintType, float Duration)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	FString Prefix;
	if (World)
	{
		switch (World->GetNetMode())
		{
		case NM_Client:
			// GPlayInEditorID 0 is always the server, so 1 will be first client
			// You want to keep this logic in sync with GeneratePIEViewportWindowTitle and UpdatePlayInEditorWorldDebugString
			Prefix = World->WorldType == EWorldType::PIE ? FString::Printf(TEXT("[Client %d]"), GPlayInEditorID) : "[Client]";
			break;
		case NM_DedicatedServer:
			Prefix = "[DedicatedServer]";
			break;
		case NM_ListenServer:
			Prefix = "[ListenServer]";
			break;
		case NM_Standalone:
			Prefix = "[Standalone]";
			break;
		default:
			break;
		}
	}

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	// If using from BP, print the blueprint object and function name
	if (!bFromCPP && !FBlueprintContextTracker::Get().GetCurrentScriptStack().IsEmpty())
	{
		const TArrayView<const FFrame* const> ScriptStack = FBlueprintContextTracker::Get().GetCurrentScriptStack();

		FString BPName = ScriptStack.Last()->Node->GetPackage()->GetPathName();

		int32 LastCharIndex = 0;
		BPName.FindLastChar('/', LastCharIndex);
		BPName = BPName.RightChop(LastCharIndex + 1);

		Prefix = FString::Printf(TEXT("%s%s::%s() "),
		                         *Prefix,
		                         *BPName,
		                         *ScriptStack.Last()->Node->GetName());
	}
#endif

	const FString FinalLogString = Prefix + InString;

	const APlayerController* PC = (WorldContextObject ? UGameplayStatics::GetPlayerController(WorldContextObject, 0) : nullptr);
	const ULocalPlayer* LocalPlayer = (PC ? Cast<ULocalPlayer>(PC->Player) : nullptr);
	if (LocalPlayer && LocalPlayer->ViewportClient && LocalPlayer->ViewportClient->ViewportConsole)
	{
		// Log to the console
		LocalPlayer->ViewportClient->ViewportConsole->OutputText(FinalLogString);
	}

	// Log to the output log
	if (PrintType == EEzLogPrintType::ScreenAndLog || PrintType == EEzLogPrintType::LogOnly)
	{
		switch (Severity)
		{
		case EEzLogSeverity::Display:
			UE_LOG(EasyLogger, Display, TEXT("%s"), *FinalLogString);
			break;
		case EEzLogSeverity::Warning:
			UE_LOG(EasyLogger, Warning, TEXT("%s"), *FinalLogString);
			break;
		case EEzLogSeverity::Error:
			UE_LOG(EasyLogger, Error, TEXT("%s"), *FinalLogString);
			break;
		default:
			break;
		}
	}

	// Do not print on-screen in Shipping or Test
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)

	// Also output to the screen, if possible
	if ((PrintType == EEzLogPrintType::ScreenAndLog || PrintType == EEzLogPrintType::ScreenOnly) && Duration > 0 && GAreScreenMessagesEnabled)
	{
		// Determine the on screen color of the log
		FLinearColor DisplayColor = FLinearColor::White;
		switch (Severity)
		{
		case EEzLogSeverity::Display:
			DisplayColor = FLinearColor(0.0, 0.66, 1.0);
			break;
		case EEzLogSeverity::Warning:
			DisplayColor = FLinearColor::Yellow;
			break;
		case EEzLogSeverity::Error:
			DisplayColor = FLinearColor::Red;
			break;
		default:
			break;
		}

		GEngine->AddOnScreenDebugMessage(-1, Duration, DisplayColor.ToFColor(true), FinalLogString);
	}
	else if (!GAreScreenMessagesEnabled)
	{
		UE_LOG(EasyLogger, VeryVerbose, TEXT("Screen messages disabled (!GAreScreenMessagesEnabled).  Cannot print to screen."));
	}
#endif
}
