// Copyright 2019 ByOwls. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Networking.h"
#include "Engine.h"
#include <string>
#include "Kismet/KismetMathLibrary.h"
#include "Camera.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class CAMERA_API AMyActor : public AActor
{
	GENERATED_BODY()

private:
	FSocket* ListenerSocket;
	FSocket* ConnectionSocket;
	FIPv4Endpoint RemoteAddressForConnection;
	FVector ZeroCoordinate;
	int test = 0;
	FCameraModule obj1;
	AActor *currentActor;

	FTimerHandle TCPSocketListenerTimerHandle;
	FTimerHandle TCPConnectionListenerTimerHandle;

	bool StartTCPReceiver(
		const FString& YourChosenSocketName,
		const FString& TheIP,
		const int32 ThePort
	);

	FSocket* CreateTCPConnectionListener(
		const FString& YourChosenSocketName,
		const FString& TheIP,
		const int32 ThePort,
		const int32 ReceiveBufferSize = 2 * 1024 * 1024
	);

	//Timer functions, could be threads
	void TCPConnectionListener(); 	//can thread this eventually
	FString StringFromBinaryArray(TArray<uint8> BinaryArray);
	void TCPSocketListener();//can thread this eventually	
	bool FormatIP4ToNumber(const FString& TheIP, uint8(&Out)[4]);//Format String IP4 to number array
	std::string FindSymbolInStr(const std::string &str, std::string symbol);
public:
	UFUNCTION(BlueprintCallable, Category = "TCPServer")
		bool LaunchTCP();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
