// Fill out your copyright notice in the Description page of Project Settings.


#include "EasyLoggerStatics.h"

#include "Engine/Console.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(EasyLogger, Display, All);

void UEasyLoggerStatics::EZLog(const UObject* WorldContextObject, const FString& InString,
                               const EEZLogSeverity Severity, const bool bPrintToScreen, float Duration)
{
	EZLogInternal(false, WorldContextObject, InString, Severity, bPrintToScreen, Duration);
}

void UEasyLoggerStatics::EZLog(const UObject* WorldContextObject, const char* Origin, const FString& InString,
                               const EEZLogSeverity Severity, const bool bPrintToScreen, float Duration)
{
	EZLogInternal(true, WorldContextObject, FString(Origin) + "() " + InString, Severity, bPrintToScreen, Duration);
}

void UEasyLoggerStatics::EZLogInternal(const bool bFromCPP, const UObject* WorldContextObject, const FString& InString,
                                       const EEZLogSeverity Severity, const bool bPrintToScreen, float Duration)
{
	// Do not Print in Shipping or Test.
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)

	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	FString Prefix;
	if (World)
	{
		if (World->WorldType == EWorldType::PIE)
		{
			switch (World->GetNetMode())
			{
			case NM_Client:
				// GPlayInEditorID 0 is always the server, so 1 will be first client.
				// You want to keep this logic in sync with GeneratePIEViewportWindowTitle and UpdatePlayInEditorWorldDebugString.
				Prefix = FString::Printf(TEXT("[Client %d]"), GPlayInEditorID);
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
	}

	// If using from BP, print the blueprint object and function name.
	if (!FBlueprintContextTracker::Get().GetCurrentScriptStack().IsEmpty() && !bFromCPP)
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

	const FString FinalLogString = Prefix + InString;

	FLinearColor DisplayColor = FLinearColor::White;
	switch (Severity)
	{
	case EEZLogSeverity::Display:
		DisplayColor = FLinearColor(0.0, 0.66, 1.0);
		UE_LOG(EasyLogger, Display, TEXT("%s"), *FinalLogString);
		break;
	case EEZLogSeverity::Warning:
		DisplayColor = FLinearColor::Yellow;
		UE_LOG(EasyLogger, Warning, TEXT("%s"), *FinalLogString);
		break;
	case EEZLogSeverity::Error:
		DisplayColor = FLinearColor::Red;
		UE_LOG(EasyLogger, Error, TEXT("%s"), *FinalLogString);
		break;
	default:
		break;
	}

	const APlayerController* PC = (WorldContextObject
		                               ? UGameplayStatics::GetPlayerController(WorldContextObject, 0)
		                               : nullptr);
	const ULocalPlayer* LocalPlayer = (PC ? Cast<ULocalPlayer>(PC->Player) : nullptr);
	if (LocalPlayer && LocalPlayer->ViewportClient && LocalPlayer->ViewportClient->ViewportConsole)
	{
		// Log to the console.
		LocalPlayer->ViewportClient->ViewportConsole->OutputText(FinalLogString);
	}

	// Also output to the screen, if possible.
	if (bPrintToScreen)
	{
		if (GAreScreenMessagesEnabled)
		{
			if (Duration > 0)
			{
				GEngine->AddOnScreenDebugMessage(-1, Duration, DisplayColor.ToFColor(true), FinalLogString);
			}
		}
		else
		{
			UE_LOG(EasyLogger, VeryVerbose,
			       TEXT("Screen messages disabled (!GAreScreenMessagesEnabled).  Cannot print to screen."));
		}
	}
#endif
}
