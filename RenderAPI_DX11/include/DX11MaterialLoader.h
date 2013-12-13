#pragma once

#include "DX11Helper.h"

struct DX11MaterialLoader
{
	void LoadFromFile(Material& material, std::string materialFile);
};
