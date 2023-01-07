#include "console.h"

#include <format>
#include <iostream>
#include <windows.h>

void console::impl::on_attach(const char* title)
{
#ifdef _DEBUG
	LI_FN(AllocConsole)();

	freopen_s(reinterpret_cast<_iobuf**>(stdout), xs("conout$"), xs("w"), static_cast<_iobuf*>(stdout));

	if (title)
		LI_FN(SetConsoleTitleA)(title);
#endif
}

void console::impl::on_release()
{
#ifdef _DEBUG
	fclose(static_cast<_iobuf*>(stdout));

	LI_FN(FreeConsole)();
#endif
}
