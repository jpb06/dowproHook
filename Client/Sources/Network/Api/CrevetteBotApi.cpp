#include "CrevetteBotApi.hpp"

#include ".\..\Socket.hpp"
#include ".\..\..\Lua\LuaObject.hpp"
#include ".\..\..\StaticAssets.hpp"
#include ".\..\..\Errors\ApplicationError.hpp"
#include ".\..\..\Util\StringUtil.hpp"

wstring CrevetteBotApi::ParseGameResultParameters(std::unique_ptr<LuaObject> parsedGameResult, std::string hash)
{
	return L"{\"GameResult\"=" + parsedGameResult->ToJson() + L", \"Hash\"=\"" + StringUtil::ConvertToWide(hash) + L"\"";
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
	std::wstring archivePath = StaticAssets::SoulstormFiles.GetSoulstormRootDirectory() + L"\\dphd.zip";
	
	StaticAssets::SoulstormFiles.ArchiveFiles(archivePath, filesPath);

	Socket socket(
		StaticAssets::ApiConfiguration.address,
		StaticAssets::ApiConfiguration.port
	);
	return socket.SendFile("/SendReplays", archivePath);
}