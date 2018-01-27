#include <windows.h>
#include ".\..\Errors\ApplicationError.hpp"
#include ".\..\Util\StringUtil.hpp"

namespace IdentityUtil
{
	std::string GenerateOne()
	{
		GUID guidReference;
		HRESULT hCreateGuid = CoCreateGuid(&guidReference);

		if (hCreateGuid != 0)
			throw ApplicationError("Couldn't generate a guid");

		wchar_t szGUID[64] = { 0 };
		StringFromGUID2(guidReference, szGUID, 64);

		return StringUtil::ConvertToNarrow(wstring(szGUID));
	}
}