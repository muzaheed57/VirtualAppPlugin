// Copyright 2019 ByOwls. All Rights Reserved.

#include "MyActor.h"
#include "GameFramework/Pawn.h"

#define SHOW_TIME_MESSAGE  0.05f
static bool g_OneStartBeginPlay = true;

void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	if (g_OneStartBeginPlay == true)
	{
		for (TActorIterator < AActor > ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			if (obj1.s_GetCurrentItemLabel(false, false).ToString() == ActorItr->GetName())
			{
				currentActor = *ActorItr;
				break;
			}
		}
		ZeroCoordinate.X = currentActor->GetActorLocation().X;
		ZeroCoordinate.Y = currentActor->GetActorLocation().Y;
		ZeroCoordinate.Z = currentActor->GetActorLocation().Z;
		g_OneStartBeginPlay = false;
		StartTCPReceiver("SocketListener", obj1.s_GetIp(false), FCString::Atoi(*obj1.s_GetPort(false)));
	}
}

void AMyActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
		if (&TCPConnectionListenerTimerHandle != nullptr)
			GetWorld()->GetTimerManager().ClearTimer(TCPConnectionListenerTimerHandle);
		if (ConnectionSocket != NULL)
			ConnectionSocket->Close();
		if (ListenerSocket != NULL)
			ListenerSocket->Close();
	g_OneStartBeginPlay = true;
}

//Rama's Start TCP Receiver
bool AMyActor::StartTCPReceiver(
	const FString& YourChosenSocketName,
	const FString& TheIP,
	const int32 ThePort
) {
	//Rama's CreateTCPConnectionListener
	ListenerSocket = CreateTCPConnectionListener(YourChosenSocketName, TheIP, ThePort);

	//Not created?
	if (!ListenerSocket)
	{
		if (obj1.s_GetShowDebugMessage(false))
			GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::Red, FString::Printf(TEXT("StartTCPReceiver>> Listen socket could not be created! ~> %s %d"), *TheIP, ThePort));
		return false;
	}

	//Start the Listener!
	//thread this eventually
	GetWorld()->GetTimerManager().SetTimer(TCPConnectionListenerTimerHandle, this, &AMyActor::TCPConnectionListener, 0.02f, true);

	//AMyActor::TCPConnectionListener();

	if (obj1.s_GetShowDebugMessage(false))
		GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::Green, FString::Printf(TEXT("StartTCPReceiver>> Listen socket created")));
	return true;
}

FSocket* AMyActor::CreateTCPConnectionListener(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort, const int32 ReceiveBufferSize)
{
	uint8 IP4Nums[4];
	if (!FormatIP4ToNumber(TheIP, IP4Nums))
	{
		if (obj1.s_GetShowDebugMessage(false))
			GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::Red, FString::Printf(TEXT("FormatIP4ToNumber >> does not format ip")));
		return false; 
	}
	else
	{
		if (obj1.s_GetShowDebugMessage(false))
			GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::Green, FString::Printf(TEXT("FormatIP4ToNumber >> format ip true")));
	}

	//Create Socket
	FIPv4Endpoint Endpoint(FIPv4Address(IP4Nums[0], IP4Nums[1], IP4Nums[2], IP4Nums[3]), ThePort);
	FSocket* ListenSocket = FTcpSocketBuilder(*YourChosenSocketName)
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.Listening(8);

	////Set Buffer Size
	int32 NewSize = 0;
	ListenSocket->SetReceiveBufferSize(ReceiveBufferSize, NewSize);

	//Done!
	return ListenSocket;
}

bool AMyActor::FormatIP4ToNumber(const FString& TheIP, uint8(&Out)[4])
{
	//IP Formatting
	TheIP.Replace(TEXT(" "), TEXT(""));

	//String Parts
	TArray<FString> Parts;
	TheIP.ParseIntoArray(Parts, TEXT("."), true);
	if (Parts.Num() != 4)
		return false;

	//String to Number Parts
	for (int32 i = 0; i < 4; ++i)
	{
		Out[i] = FCString::Atoi(*Parts[i]);
	}
	return true;
}

std::string AMyActor::FindSymbolInStr(const std::string & str, std::string symbol)
{
	int posSymbol = str.find(symbol);
	int lengthStr = str.length();
	std::string buffStr = str.substr(posSymbol, lengthStr - posSymbol);

	buffStr = buffStr.substr(buffStr.find(':'), lengthStr - buffStr.find(':'));

	int posComma;
	posComma = buffStr.find(',');
	if (posComma == std::string::npos)
		posComma = buffStr.find('}');
	if (posComma != std::string::npos)
		buffStr.erase(posComma);

	return buffStr.substr(1, buffStr.length());
}

void AMyActor::TCPConnectionListener()
{
	if (!ListenerSocket) return;
	
	//Remote address
	TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool Pending;

	if (ListenerSocket->HasPendingConnection(Pending) && Pending)
	{
		//Already have a Connection? destroy previous
		if (ConnectionSocket)
		{
			ConnectionSocket->Close();
			ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ConnectionSocket);
		}
		
		//New Connection receive!
		ConnectionSocket = ListenerSocket->Accept(*RemoteAddress, TEXT("TCP Received Socket Connection"));
		
		if (obj1.s_GetShowDebugMessage(false))
			GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::Green, FString::Printf(TEXT("TCPConnectionListener >> TCP Received Socket Connection")));
		
		if (ConnectionSocket != NULL)
		{
			//Global cache of current Remote Address
			RemoteAddressForConnection = FIPv4Endpoint(RemoteAddress);

			AMyActor::TCPSocketListener();
		}
	}
}

FString AMyActor::StringFromBinaryArray(TArray<uint8> BinaryArray)
{
	//Create a string from a byte array!
	std::string cstr(reinterpret_cast<const char*>(BinaryArray.GetData()), BinaryArray.Num());

	return FString(cstr.c_str());
}

void AMyActor::TCPSocketListener()
{
	if (!ConnectionSocket) return;

	TArray<uint8> ReceivedData;
	static int32 delsize = 0;

	uint32 Size;

	while (ConnectionSocket->HasPendingData(Size))
	{
		ReceivedData.Init(FMath::Min(Size, 65507u), Size);

		int32 Read = 0;
		ConnectionSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);
	}

	if (ReceivedData.Num() <= 0) return;

	//String From Binary Array
	FString ReceivedUE4String = StringFromBinaryArray(ReceivedData);

	if (obj1.s_GetShowDebugMessage(false))
		GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::Blue, FString::Printf(TEXT("As String Data ~> %s"), *ReceivedUE4String));

	const std::string cstr(reinterpret_cast<const char*>(ReceivedData.GetData()), ReceivedData.Num());

	if (obj1.s_GetShowDebugMessage(false))
		GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::Green, currentActor->GetName());

	FVector LocationStringData = currentActor->GetActorLocation();
	FRotator RotationStringData = currentActor->GetActorRotation();
	FQuat RotationQuat = RotationStringData.Quaternion();


	if (LocationStringData.X + 1.f == (FMath::RoundToFloat(ZeroCoordinate.X + FCString::Atof(*(FString(FindSymbolInStr(cstr, "z").c_str()))) * obj1.s_GetCoordinateMul(false)) * (-1))
		|| LocationStringData.X == (FMath::RoundToFloat(ZeroCoordinate.X + FCString::Atof(*(FString(FindSymbolInStr(cstr, "z").c_str()))) * obj1.s_GetCoordinateMul(false)) * (-1))
		|| LocationStringData.Y + 1.f == (FMath::RoundToFloat(ZeroCoordinate.Y + FCString::Atof(*(FString(FindSymbolInStr(cstr, "x").c_str()))) * obj1.s_GetCoordinateMul(false)))
		|| LocationStringData.Y == (FMath::RoundToFloat(ZeroCoordinate.Y + FCString::Atof(*(FString(FindSymbolInStr(cstr, "x").c_str()))) * obj1.s_GetCoordinateMul(false)))
		|| LocationStringData.Z + 1.f == (FMath::RoundToFloat(ZeroCoordinate.Z + FCString::Atof(*(FString(FindSymbolInStr(cstr, "y").c_str()))) * obj1.s_GetCoordinateMul(false)))
		|| LocationStringData.Z == (FMath::RoundToFloat(ZeroCoordinate.Z + FCString::Atof(*(FString(FindSymbolInStr(cstr, "y").c_str()))) * obj1.s_GetCoordinateMul(false)))
		) delsize++;
	else if (LocationStringData.X + 2.f < (FMath::RoundToFloat(ZeroCoordinate.X + FCString::Atof(*(FString(FindSymbolInStr(cstr, "z").c_str()))) * obj1.s_GetCoordinateMul(false)) * (-1))
		|| LocationStringData.X + 2.f > (FMath::RoundToFloat(ZeroCoordinate.X + FCString::Atof(*(FString(FindSymbolInStr(cstr, "z").c_str()))) * obj1.s_GetCoordinateMul(false)) * (-1)))
		delsize = 0;

	if (delsize <= 20)
	{
		LocationStringData.X = (FMath::RoundToFloat(ZeroCoordinate.X + FCString::Atof(*(FString(FindSymbolInStr(cstr, "z").c_str()))) * obj1.s_GetCoordinateMul(false)) * (-1));
		LocationStringData.Y = (FMath::RoundToFloat(ZeroCoordinate.Y + FCString::Atof(*(FString(FindSymbolInStr(cstr, "x").c_str()))) * obj1.s_GetCoordinateMul(false)));
		LocationStringData.Z = (FMath::RoundToFloat(ZeroCoordinate.Z + FCString::Atof(*(FString(FindSymbolInStr(cstr, "y").c_str()))) * obj1.s_GetCoordinateMul(false)));

		currentActor->SetActorLocation(LocationStringData);


		RotationQuat.X = FMath::FloorToFloat(FCString::Atof(*(FString(FindSymbolInStr(cstr, "r").c_str()))) * 1000) / 1000; //p
		RotationQuat.Y = FMath::FloorToFloat(FCString::Atof(*(FString(FindSymbolInStr(cstr, "p").c_str()))) * 1000) / 1000 * (-1); //yaw
		RotationQuat.Z = FMath::FloorToFloat(FCString::Atof(*(FString(FindSymbolInStr(cstr, "yaw").c_str()))) * 1000) / 1000 * (-1); //r
		RotationQuat.W = FCString::Atof(*(FString(FindSymbolInStr(cstr, "ww").c_str())));
		
		currentActor->SetActorRotation(RotationQuat);
	}
		/*---------------------------------*/
		GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::Cyan, FString::SanitizeFloat(RotationQuat.W));
		GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::Blue, FString::SanitizeFloat(RotationQuat.Z));
		GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::Green, FString::SanitizeFloat(RotationQuat.Y));
		GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::Red, FString::SanitizeFloat(RotationQuat.X));
		
		GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::Emerald, "////////////////////////rotation");
		
		GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::Emerald, FString::SanitizeFloat(LocationStringData.Z));
		GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::White, FString::SanitizeFloat(LocationStringData.Y));
		GEngine->AddOnScreenDebugMessage(-1, SHOW_TIME_MESSAGE, FColor::Yellow, FString::SanitizeFloat(LocationStringData.X));
}