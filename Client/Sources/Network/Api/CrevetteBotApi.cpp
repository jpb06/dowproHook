#include "CrevetteBotApi.hpp"

#include <Windows.h>
#include ".\..\Socket.hpp"
#include ".\..\..\Lua\LuaObject.hpp"
#include ".\..\..\StaticAssets.hpp"
#include ".\..\..\Errors\ApplicationError.hpp"
#include ".\..\..\Util\StringUtil.hpp"

wstring CrevetteBotApi::ParseGameResultParameters(std::unique_ptr<LuaObject> parsedGameResult, std::string hash)
{
	return L"{\"GameResult\"=" + parsedGameResult->ToJson() + L", \"Hash\"=\"" + StringUtil::ConvertToWide(hash) + L"\"";
}

GUID CrevetteBotApi::SendIdentity(std::wstring guid)
{
	GUID guidReference;
	HRESULT hCreateGuid = CoCreateGuid(&guidReference);

	if (hCreateGuid != S_OK)
		throw ApplicationError("Couldn't generate a guid");

	Socket socket(
		StaticAssets::ApiConfiguration.address,
		StaticAssets::ApiConfiguration.port
	);
	socket.SendString("/Identity", guid);

	return guidReference;
}

bool CrevetteBotApi::SendGameResult(std::wstring json)
{
	Socket socket(
		StaticAssets::ApiConfiguration.address, 
		StaticAssets::ApiConfiguration.port
	);
	return socket.SendString("/SendResult", json);
}

bool CrevetteBotApi::SendReplays(std::vector<std::wstring> filesPath)
{
	std::wstring archivePath = StaticAssets::SoulstormFiles.GetSoulstormRootDirectory() + L"\\Playback\\dphd.zip";
	
	StaticAssets::SoulstormFiles.ArchiveFiles(archivePath, filesPath);

	Socket socket(
		StaticAssets::ApiConfiguration.address,
		StaticAssets::ApiConfiguration.port
	);
	return socket.SendFile("/SendReplays", archivePath);
}