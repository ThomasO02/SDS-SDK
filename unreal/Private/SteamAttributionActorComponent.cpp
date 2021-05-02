// Fill out your copyright notice in the Description page of Project Settings.


#include "SteamAttributionActorComponent.h"

// Sets default values for this component's properties
USteamAttributionActorComponent::USteamAttributionActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void USteamAttributionActorComponent::PostResponse(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessful)
{
	if (!ResponseIsValid(Response, bWasSuccessful)) return;

	UE_LOG(LogTemp, Warning, TEXT("POST Response Content: %s"), *Response->GetContentAsString());
}

void USteamAttributionActorComponent::GetResponse(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessful)
{
	if (!ResponseIsValid(Response, bWasSuccessful)) return;
	UE_LOG(LogTemp, Warning, TEXT("GET Response Content: %s"), *Response->GetContentAsString());
}

TSharedRef<IHttpRequest> USteamAttributionActorComponent::RequestWithRoute(FString Subroute)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->SetURL(ApiBaseUrl + Subroute);
	SetRequestHeaders(Request);
	return Request;
}

void USteamAttributionActorComponent::SetRequestHeaders(TSharedRef<IHttpRequest>& Request)
{
	Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accepts"), TEXT("application/json"));
}

TSharedRef<IHttpRequest> USteamAttributionActorComponent::GetRequest(FString Subroute)
{
	TSharedRef<IHttpRequest> Request = RequestWithRoute(Subroute);
	Request->SetVerb("GET");
	return Request;	
}

TSharedRef<IHttpRequest> USteamAttributionActorComponent::PostRequest(FString Subroute, FString ContentJsonString)
{
	TSharedRef<IHttpRequest> Request = RequestWithRoute(Subroute);
	Request->SetVerb("POST");
	Request->SetContentAsString(ContentJsonString);
	return Request;
}

TSharedRef<IHttpRequest> USteamAttributionActorComponent::GetRequestFull(FString ApiUrl)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->SetURL(ApiUrl);
	Request->SetVerb("GET");
	SetRequestHeaders(Request);
	return Request;
}

TSharedRef<IHttpRequest> USteamAttributionActorComponent::PostRequestFull(FString ApiUrl,
	FString ContentJsonString)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->SetURL(ApiUrl);
	Request->SetVerb("POST");
	Request->SetContentAsString(ContentJsonString);
	SetRequestHeaders(Request);
	return Request;
}

void USteamAttributionActorComponent::Send(TSharedRef<IHttpRequest>& Request)
{
	Request->ProcessRequest();
}

bool USteamAttributionActorComponent::ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid()) return false;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode())) return true;
	else {
		UE_LOG(LogTemp, Warning, TEXT("Http Response returned error code: %d"), Response->GetResponseCode());
		return false;
	}
}


// Called when the game starts
void USteamAttributionActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// Store the Http module instance
	Http = &FHttpModule::Get();

	//// TEST a Post without any data
	//auto testPost = PostRequest("TestPost", {}); // Post without any data
	//testPost->OnProcessRequestComplete().BindUObject(this, &USteamAttributionActorComponent::PostResponse);
	//Send(testPost);

	///// Test a Post with data (see FRequest_PostTest struct)
	//FString ContentJsonString;
	//FRequest_PostTest TestData;
	//TestData.Data = "<<Hello - Insert test response data>>";
	//GetJsonStringFromStruct<FRequest_PostTest>(TestData, ContentJsonString);
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *ContentJsonString);
	//auto postTestData = PostRequest("TestPost", ContentJsonString); // Post with any data
	//postTestData->OnProcessRequestComplete().BindUObject(this, &USteamAttributionActorComponent::PostResponse);
	//Send(postTestData);

	//auto getTest = GetRequest("TestGet");
	//getTest->OnProcessRequestComplete().BindUObject(this, &USteamAttributionActorComponent::GetResponse);
	//Send(getTest);


	// license key goes here
	FString licenseKey = {"2e96eedce46544d9806de9dc3401c8007" };
	FString url = "http://ldns.co/ca/" + licenseKey + "/";
	auto steamPost = PostRequestFull("http://ldns.co/ca/" + licenseKey + "/", {}); // Post without any data
	steamPost->OnProcessRequestComplete().BindUObject(this, &USteamAttributionActorComponent::PostResponse);
	Send(steamPost);
}


// Called every frame
void USteamAttributionActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

