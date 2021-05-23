// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Online/HTTP/Public/Http.h"
#include "Json.h"
#include "JsonUtilities.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SteamAttributionActorComponent.generated.h"

USTRUCT()
struct FRequest_PostTest {
	GENERATED_BODY()
		UPROPERTY() FString Data; // Note: json converted to lower case!! {"data":"stringcontenthere"}

	FRequest_PostTest() {}
};

// Below here you can create other "data structs"

//USTRUCT()
//struct FRequest_PostTest2 {
//	GENERATED_BODY()
//		UPROPERTY() FString Data2; // Note: json converted to lower case!! {"data":"stringcontenthere2"}
//
//	FRequest_PostTest2() {}
//};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEAMATTRIBUTION_API USteamAttributionActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USteamAttributionActorComponent();

private:
	FHttpModule* Http;
	FString ApiBaseUrl = "http://steam.gs/ca/";

	FString AuthorizationHeader = TEXT("Authorization");
	void PostResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void GetResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
	TSharedRef<IHttpRequest> RequestWithRoute(FString Subroute);
	void SetRequestHeaders(TSharedRef<IHttpRequest>& Request);

	TSharedRef<IHttpRequest> GetRequest(FString Subroute);
	TSharedRef<IHttpRequest> PostRequest(FString Subroute, FString ContentJsonString);
	TSharedRef<IHttpRequest> GetRequestFull(FString ApiUrl);
	TSharedRef<IHttpRequest> PostRequestFull(FString ApiUrl, FString ContentJsonString);
	void Send(TSharedRef<IHttpRequest>& Request);

	bool ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful);

	template <typename StructType>
	void GetJsonStringFromStruct(StructType FilledStruct, FString& StringOutput);
	template <typename StructType>
	void GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutput);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

template <typename StructType>
void USteamAttributionActorComponent::GetJsonStringFromStruct(StructType FilledStruct, FString& StringOutput)
{
	FJsonObjectConverter::UStructToJsonObjectString(StructType::StaticStruct(), &FilledStruct, StringOutput, 0, 0);
}

template <typename StructType>
void USteamAttributionActorComponent::GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutput)
{
	StructType StructData;
	FString JsonString = Response->GetContentAsString();
	FJsonObjectConverter::JsonObjectStringToUStruct<StructType>(JsonString, &StructOutput, 0, 0);
}